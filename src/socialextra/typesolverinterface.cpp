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

#include "filterinterface_p.h"
#include "typesolverinterface.h"
#include "identifiablecontentiteminterface_p.h"
#include "facebook/facebookontology_p.h"

static const char *FQL_KEY = "fql";
static const char *TYPE_KEY = "type";
static const char *ALBUM_FQL = "SELECT object_id FROM album WHERE aid = \"%1\"";
static const char *PHOTO_FQL = "SELECT object_id FROM photo WHERE pid = \"%1\"";

class TypeSolverFilterInterfacePrivate: public FilterInterfacePrivate
{
public:
    explicit TypeSolverFilterInterfacePrivate(TypeSolverFilterInterface *q);
private:
    Q_DECLARE_PUBLIC(TypeSolverFilterInterface)
    QString identifier;
    QString type;
    bool fql;
};

TypeSolverFilterInterfacePrivate::TypeSolverFilterInterfacePrivate(TypeSolverFilterInterface *q)
    : FilterInterfacePrivate(q), fql(false)
{
}

TypeSolverFilterInterface::TypeSolverFilterInterface(QObject *parent)
    : FilterInterface(*(new TypeSolverFilterInterfacePrivate(this)), parent)
{
}

QString TypeSolverFilterInterface::identifier() const
{
    Q_D(const TypeSolverFilterInterface);
    return d->identifier;
}

void TypeSolverFilterInterface::setIdentifier(const QString &identifier)
{
    Q_D(TypeSolverFilterInterface);
    if (d->identifier != identifier) {
        d->identifier = identifier;
        emit identifierChanged();
    }
}

QString TypeSolverFilterInterface::type() const
{
    Q_D(const TypeSolverFilterInterface);
    return d->type;
}

void TypeSolverFilterInterface::setType(const QString &type)
{
    Q_D(TypeSolverFilterInterface);
    if (d->type != type) {
        d->type = type;
        emit typeChanged();
    }
}

bool TypeSolverFilterInterface::isFql() const
{
    Q_D(const TypeSolverFilterInterface);
    return d->fql;
}

void TypeSolverFilterInterface::setFql(bool fql)
{
    Q_D(TypeSolverFilterInterface);
    if (d->fql != fql) {
        d->fql = fql;
        emit fqlChanged();
    }
}

bool TypeSolverFilterInterface::isAcceptable(QObject *item, SocialNetworkInterface *socialNetwork) const
{
    if (!testType<FacebookInterface>(socialNetwork)) {
        return false;
    }

    // This filter only works for items
    if (!testType<TypeSolverInterface>(item)) {
        return false;
    }

    return true;
}

bool TypeSolverFilterInterface::performLoadRequestImpl(QObject *item, SocialNetworkInterface *socialNetwork,
                                                       LoadType loadType)
{
    Q_D(TypeSolverFilterInterface);

    FacebookInterface *facebook = qobject_cast<FacebookInterface *>(socialNetwork);
    if (!facebook) {
        return false;
    }

    QVariantMap properties;
    properties.insert(FQL_KEY, d->fql);
    QMap<QString, QString> arguments;
    QObject *handle = 0;
    if (d->fql) {
        properties.insert(TYPE_KEY, d->type);
        if (d->type == QLatin1String("album")) {
            // Query album using FQL
            QString query = QString(ALBUM_FQL).arg(d->identifier);
            arguments.insert("q", query);
            handle = facebook->get(this, QLatin1String("fql"), QString(), QString(), arguments);
        } else if (d->type == QLatin1String("photo")) {
            // Query album using FQL
            QString query = QString(PHOTO_FQL).arg(d->identifier);
            arguments.insert("q", query);
            handle = facebook->get(this, QLatin1String("fql"), QString(), QString(), arguments);
        } else {
            return false;
        }
    } else {
        arguments.insert("metadata", "1");
        handle = facebook->get(this, d->identifier, QString(), QString(), arguments);
    }

    d->addHandleProperties(handle, properties);
    return d->addHandle(handle, item, socialNetwork, loadType);
}

bool TypeSolverFilterInterface::performSetItemDataImpl(IdentifiableContentItemInterface *item,
                                                       SocialNetworkInterface *socialNetwork,
                                                       const QByteArray &data, LoadType loadType,
                                                       const QVariantMap &properties)
{
    Q_UNUSED(socialNetwork)
    Q_UNUSED(properties)
    Q_UNUSED(loadType)
    bool ok = false;
    QVariantMap dataMap = IdentifiableContentItemInterfacePrivate::parseReplyData(data, &ok);

    if (!ok) {
        QString errorMessage = QString(QLatin1String("Unable to parse downloaded data. "\
                                                     "Downloaded data: %1")).arg(QString(data));
        item->setError(SocialNetworkInterface::DataError, errorMessage);
        return false;
    }

    bool fql = properties.value(FQL_KEY, false).toBool();
    if (fql) {
        // Create an artificial data
        QString type = properties.value(TYPE_KEY).toString();
        QVariantMap newDataMap;
        QVariantMap object = dataMap.value(FACEBOOK_ONTOLOGY_METADATA_DATA).toList().first().toMap();
        newDataMap.insert(FACEBOOK_ONTOLOGY_METADATA_ID, object.value("object_id"));
        newDataMap.insert(FACEBOOK_ONTOLOGY_METADATA_TYPE, type);
        dataMap = newDataMap;
    }

    dataMap.insert(NEMOQMLPLUGINS_SOCIAL_CONTENTITEMID,
                   dataMap.value(FACEBOOK_ONTOLOGY_METADATA_ID).toString());
    item->setData(dataMap);
    return true;
}


class TypeSolverInterfacePrivate: public IdentifiableContentItemInterfacePrivate
{
public:
    explicit TypeSolverInterfacePrivate(TypeSolverInterface *q);
    void emitPropertyChangeSignals(const QVariantMap &oldData, const QVariantMap &newData);
    FacebookInterface::ContentItemType objectType;
    QString objectTypeString;
private:
    Q_DECLARE_PUBLIC(TypeSolverInterface)
};

TypeSolverInterfacePrivate::TypeSolverInterfacePrivate(TypeSolverInterface *q)
    : IdentifiableContentItemInterfacePrivate(q), objectType(FacebookInterface::NotInitialized)
{
}

void TypeSolverInterfacePrivate::emitPropertyChangeSignals(const QVariantMap &oldData,
                                                           const QVariantMap &newData)
{
    Q_Q(TypeSolverInterface);

    QVariantMap metadata = newData.value(FACEBOOK_ONTOLOGY_METADATA).toMap();
    QString type = metadata.value(FACEBOOK_ONTOLOGY_METADATA_TYPE).toString();
    if (type.isEmpty()) {
        type = newData.value(FACEBOOK_ONTOLOGY_METADATA_TYPE).toString();
    }

    FacebookInterface::ContentItemType newObjectType = FacebookInterface::Unknown;

    if (type == "album") {
        newObjectType = FacebookInterface::Album;
    }
    if (type == "comment") {
        newObjectType = FacebookInterface::Comment;
    }
    if (type == "group") {
        newObjectType = FacebookInterface::Group;
    }
    if (type == "notification") {
        newObjectType = FacebookInterface::Notification;
    }
    if (type == "photo") {
        newObjectType = FacebookInterface::Photo;
    }
    if (type == "post") {
        newObjectType = FacebookInterface::Post;
    }
    if (type == "user") {
        newObjectType = FacebookInterface::User;
    }
    if (type == "application") {
        newObjectType = FacebookInterface::Application;
    }
    if (type == "event") {
        newObjectType = FacebookInterface::Event;
    }

    // Other recognized as posts
    if (type == "checkin") {
        newObjectType = FacebookInterface::Post;
    }

    if (type == "link") {
        newObjectType = FacebookInterface::Post;
    }

    if (type == "status") {
        newObjectType = FacebookInterface::Post;
    }

    if (newObjectType == FacebookInterface::Unknown) {
        qWarning() << "Unknown type:" << type;
    }

    if (objectTypeString != type) {
        objectTypeString = type;
        emit q->objectTypeStringChanged();
    }

    if (objectType != newObjectType) {
        objectType = newObjectType;
        emit q->objectTypeChanged();
    }

    // Call super class implementation
    QVariantMap oldDataWithId = oldData;
    oldDataWithId.insert(NEMOQMLPLUGINS_SOCIAL_CONTENTITEMID,
                         oldData.value(FACEBOOK_ONTOLOGY_METADATA_ID));
    QVariantMap newDataWithId = newData;
    newDataWithId.insert(NEMOQMLPLUGINS_SOCIAL_CONTENTITEMID,
                         newData.value(FACEBOOK_ONTOLOGY_METADATA_ID));
    IdentifiableContentItemInterfacePrivate::emitPropertyChangeSignals(oldDataWithId, newDataWithId);
}

TypeSolverInterface::TypeSolverInterface(QObject *parent) :
    IdentifiableContentItemInterface(*(new TypeSolverInterfacePrivate(this)), parent)
{
}

int TypeSolverInterface::type() const
{
    return FacebookInterface::ObjectReference;
}

FacebookInterface::ContentItemType TypeSolverInterface::objectType() const
{
    Q_D(const TypeSolverInterface);
    return d->objectType;
}

QString TypeSolverInterface::objectTypeString() const
{
    Q_D(const TypeSolverInterface);
    return d->objectTypeString;
}
