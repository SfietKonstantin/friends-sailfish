/*
 * Copyright (C) 2013 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "commentfilterinterface.h"
#include "filterinterface_p.h"
#include "facebook/facebookinterface.h"
#include "socialnetworkmodelinterface.h"
#include "facebook/facebookontology_p.h"
#include "identifiablecontentiteminterface_p.h"
#include "facebook/facebookcommentinterface.h"

static const char *QUERY = "{"\
    "\"query1\": \"SELECT id, fromid, text, time, likes, can_comment, "\
                  "comment_count, user_likes, parent_id FROM comment "\
                  "WHERE %1 = \\\"%2\\\" %3"\
                  "ORDER BY time DESC LIMIT %4\","\
    "\"query2\": \"SELECT uid,name FROM user WHERE uid in (SELECT fromid FROM #query1)\","\
    "\"query3\": \"SELECT page_id,name FROM page WHERE page_id in (SELECT fromid FROM #query1)\"}";
static const char *OBJECT_ID_KEY = "object_id";
static const char *POST_ID_KEY = "post_id";
static const char *QUERY_QUERY1 = "query1";
static const char *QUERY_QUERY2 = "query2";
static const char *QUERY_QUERY3 = "query3";
static const char *QUERY_DATA_KEY = "data";
static const char *QUERY_NAME_KEY = "name";
static const char *QUERY_RESULT_KEY = "fql_result_set";
static const char *QUERY_UID_KEY = "uid";
static const char *QUERY_PAGEID_KEY = "page_id";
static const int DEFAULT_LIMIT = 20;
static const char *ISPOST_KEY = "is_post";
static const char *LIMIT_KEY = "limit";
static const char *PREVIOUS_OFFSET_CURSOR_KEY = "previous_offset";
static const char *NEXT_TIMESTAMP_KEY = "next_timestamp";

class CommentFilterInterfacePrivate: public FilterInterfacePrivate
{
public:
    explicit CommentFilterInterfacePrivate(CommentFilterInterface *q);
    QString identifier;
    int limit;
    bool isPost;
};

CommentFilterInterfacePrivate::CommentFilterInterfacePrivate(CommentFilterInterface *q)
    : FilterInterfacePrivate(q), limit(DEFAULT_LIMIT), /*offset(0), */isPost(false)
{
}

CommentFilterInterface::CommentFilterInterface(QObject *parent) :
    FilterInterface(*(new CommentFilterInterfacePrivate(this)), parent)
{
}

QString CommentFilterInterface::identifier() const
{
    Q_D(const CommentFilterInterface);
    return d->identifier;
}

void CommentFilterInterface::setIdentifier(const QString &identifier)
{
    Q_D(CommentFilterInterface);
    if (d->identifier != identifier) {
        d->identifier = identifier;
        emit identifierChanged();
    }
}

int CommentFilterInterface::limit() const
{
    Q_D(const CommentFilterInterface);
    return d->limit;
}

void CommentFilterInterface::setLimit(int limit)
{
    Q_D(CommentFilterInterface);
    if (d->limit != limit) {
        d->limit = limit;
        emit limitChanged();
    }
}

bool CommentFilterInterface::isPost() const
{
    Q_D(const CommentFilterInterface);
    return d->isPost;
}

void CommentFilterInterface::setIsPost(bool isPost)
{
    Q_D(CommentFilterInterface);
    if (d->isPost != isPost) {
        d->isPost = isPost;
        emit isPostChanged();
    }
}

bool CommentFilterInterface::isAcceptable(QObject *item, SocialNetworkInterface *socialNetwork) const
{
    if (!testType<FacebookInterface>(socialNetwork)) {
        return false;
    }

    // This filter only works for models
    if (!testType<SocialNetworkModelInterface>(item)) {
        return false;
    }

    return true;
}

bool CommentFilterInterface::performLoadRequestImpl(QObject *item,
                                                    SocialNetworkInterface *socialNetwork,
                                                    LoadType loadType)
{
    Q_D(CommentFilterInterface);
    FacebookInterface *facebook = qobject_cast<FacebookInterface *>(socialNetwork);
    if (!facebook) {
        return false;
    }

    SocialNetworkModelInterface *model = qobject_cast<SocialNetworkModelInterface *>(item);
    if (!model) {
        return false;
    }

    QMap<QString, QString> arguments;
    QString query (QUERY);
    bool isPost = model->extraData().value(ISPOST_KEY, d->isPost).toBool();
    int limit = model->extraData().value(LIMIT_KEY, d->limit).toInt();
    QString previousOffset = model->extraData().value(PREVIOUS_OFFSET_CURSOR_KEY, 0).toString();
    QString nextTimestamp = model->extraData().value(NEXT_TIMESTAMP_KEY).toString();

    QString idKey = isPost ? POST_ID_KEY : OBJECT_ID_KEY;

    switch (loadType) {
    case FilterInterface::Load:
        query = query.arg(idKey, d->identifier, QString(), QString::number(limit));
        break;
    case FilterInterface::LoadNext:
        query = query.arg(idKey, d->identifier, QString("AND time > %1 ").arg(nextTimestamp),
                          QString::number(limit));
        break;
    case FilterInterface::LoadPrevious:
        query = query.arg(idKey, d->identifier, QString(),
                          QString("%1,%2").arg(previousOffset, QString::number(limit)));
        break;
    }
    arguments.insert("q", query);

    QVariantMap properties;
    properties.insert(LIMIT_KEY, limit);

    QObject *handle = facebook->get(this, QLatin1String("fql"), QString(), QString(), arguments);
    d->addHandleProperties(handle, properties);
    return d->addHandle(handle, item, socialNetwork, loadType);
}

bool CommentFilterInterface::performSetModelDataImpl(SocialNetworkModelInterface *model,
                                                     SocialNetworkInterface *socialNetwork,
                                                     const QByteArray &data, LoadType loadType,
                                                     const QVariantMap &properties)
{
    bool ok = false;
    QVariantMap parsed = IdentifiableContentItemInterfacePrivate::parseReplyData(data, &ok);
    if (!ok || !parsed.contains(QUERY_DATA_KEY)) {
        QString errorMessage = QString(QLatin1String("Unable to parse downloaded data. "\
                                                     "Downloaded data: %1")).arg(QString(data));
        model->setError(SocialNetworkInterface::DataError, errorMessage);
        return false;
    }

    QVariantList dataList = parsed.value(QLatin1String("data")).toList();

    // The FQL query will return 3 query results
    // The 1st one contains information about the comments on an item
    // The 2 others contains name that relates to the fromid (the id of the entity
    // who post the comment listed in the 1st query)

    // We should create a hashmap from the last 2 queries to make retrieving of
    // name easier.
    QMap<QString, QString> names;
    QVariantList mainData;
    foreach (const QVariant entry, dataList) {
        QVariantMap entryObject = entry.toMap();
        QString name = entryObject.value(QUERY_NAME_KEY).toString();
        if (name == QUERY_QUERY1) {
            mainData = entryObject.value(QUERY_RESULT_KEY).toList();
        } else if (name == QUERY_QUERY2
                   || name == QUERY_QUERY3) {
            QString key;
            if (name == QUERY_QUERY2) {
                key = QUERY_UID_KEY;
            } else if(name == QUERY_QUERY3) {
                key = QUERY_PAGEID_KEY;
            }

            QVariantList nameList = entryObject.value(QUERY_RESULT_KEY).toList();
            foreach (const QVariant &name, nameList) {
                QVariantMap nameObject = name.toMap();
                names.insert(nameObject.value(key).toString(),
                             nameObject.value(QUERY_NAME_KEY).toString());
            }
        }
    }

    QList<ContentItemInterface *> modelData;

    uint nextTimestamp = 0;
    if (!mainData.isEmpty()) {
        QVariantMap last = mainData.first().toMap();
        nextTimestamp = last.value("time").toUInt();
    }

    QString currentUserIdentifier = qobject_cast<FacebookInterface*>(socialNetwork)->currentUserIdentifier();

    foreach (const QVariant &commentVariant, mainData) {
        QVariantMap commentMap = commentVariant.toMap();
        QVariantMap commentData;

        commentData.insert(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTIDENTIFIER,
                           commentMap.value("id").toString());
        QString fromId = commentMap.value("fromid").toString();
        QString fromName = names.value(fromId);
        QVariantMap fromData;
        fromData.insert(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTIDENTIFIER, fromId);
        fromData.insert(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTNAME, fromName);
        commentData.insert(FACEBOOK_ONTOLOGY_COMMENT_FROM, fromData);
        commentData.insert(FACEBOOK_ONTOLOGY_COMMENT_MESSAGE, commentMap.value("text").toString());
        uint timestamp = commentMap.value("time").toUInt();
        QDateTime time = QDateTime::fromTime_t(timestamp);
        time = time.toTimeSpec(Qt::UTC);
        commentData.insert(FACEBOOK_ONTOLOGY_COMMENT_CREATEDTIME, time.toString(Qt::ISODate));

        QVariantMap likesData;
        QVariantMap likesSummary;
        likesSummary.insert(FACEBOOK_ONTOLOGY_METADATA_TOTALCOUNT,
                            commentMap.value("likes").toInt());
        QVariantList likesObjects;
        if (commentMap.value("user_likes").toBool()) {
            QVariantMap me;
            me.insert(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTIDENTIFIER, currentUserIdentifier);
            likesObjects.append(me);
        }
        likesData.insert(FACEBOOK_ONTOLOGY_METADATA_SUMMARY, likesSummary);
        likesData.insert(FACEBOOK_ONTOLOGY_METADATA_DATA, likesObjects);
        commentData.insert(FACEBOOK_ONTOLOGY_CONNECTIONS_LIKES, likesData);

        FacebookCommentInterface *comment = new FacebookCommentInterface(model);
        comment->setSocialNetwork(socialNetwork);
        comment->setData(commentData);
        comment->classBegin();
        comment->componentComplete();
        modelData.prepend(comment); // Sorted in different order
    }

    int limit = properties.value(LIMIT_KEY).toInt();

    int oldPreviousOffset = model->extraData().value(PREVIOUS_OFFSET_CURSOR_KEY).toInt();
    uint oldNextTimestamp = model->extraData().value(NEXT_TIMESTAMP_KEY).toUInt();

    int previousOffset = oldPreviousOffset + modelData.count();

    // Populate model depending on the type of load
    switch (loadType) {
    case FilterInterface::Load:
        model->setModelData(modelData);
        break;
    case FilterInterface::LoadPrevious:
        nextTimestamp = oldNextTimestamp;
        model->prependModelData(modelData);
        break;
    case FilterInterface::LoadNext:
        if (nextTimestamp == 0) {
            nextTimestamp = oldNextTimestamp;
        }
        previousOffset = oldPreviousOffset;
        model->appendModelData(modelData);
        break;
    default:
        break;
    }

    QVariantMap extraData;
    extraData.insert(LIMIT_KEY, limit);
    extraData.insert(PREVIOUS_OFFSET_CURSOR_KEY, previousOffset);
    extraData.insert(NEXT_TIMESTAMP_KEY, nextTimestamp);

    bool hasPrevious = modelData.count() == limit;

    model->setPagination(hasPrevious, true);
    model->setExtraData(extraData);

    return true;
}
