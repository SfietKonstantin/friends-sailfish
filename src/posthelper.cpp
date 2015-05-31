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

#include "posthelper.h"
#include "footerhelper.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QRegularExpression>
#include <QtCore/QUrl>

static const int ELIDE_COUNT = 50;

static const char *ITEM_TEMPLATE = "item----%1";
//static const char *URL_TEMPLATE = "url----%1";
//static const char *URL_REGEXP = "((http://|https://|www.)[a-zA-Z0-9_\\.\\-~%/#  ?=&\\+]*)";

PostHelper::PostHelper(QObject *parent)
    : AbstractDisplayHelper(parent)
    , m_fancy(false)
    , m_fullHeader(true)
    , m_hasContent(false)
    , m_hasFooter(true)
{
}

bool PostHelper::isFancy() const
{
    return m_fancy;
}

void PostHelper::setFancy(bool fancy)
{
    if (m_fancy != fancy) {
        m_fancy = fancy;
        emit fancyChanged();
    }
}

bool PostHelper::isFullHeader() const
{
    return m_fullHeader;
}

QString PostHelper::profilePicture() const
{
    return m_profilePicture;
}

QString PostHelper::header() const
{
    return m_header;
}

QDateTime PostHelper::timestamp() const
{
    return m_timestamp;
}

QString PostHelper::footer() const
{
    return m_footer;
}

QString PostHelper::message() const
{
    return m_message;
}

QString PostHelper::via() const
{
    return m_via;
}

bool PostHelper::hasContent() const
{
    return m_hasContent;
}

bool PostHelper::hasFooter() const
{
    return m_hasFooter;
}

QString PostHelper::name() const
{
    return m_name;
}

QString PostHelper::caption() const
{
    return m_caption;
}

QString PostHelper::description() const
{
    return m_description;
}

void PostHelper::performCreationImpl()
{
    const QVariantList &actors = object().value("actors").toList();
    QVariant actor;
    QString name;
    QString profilePicture;
    if (!actors.isEmpty()) {
        actor = actors.first();
        name = getProperty(actor, "name").trimmed();
    }
    // Check if we should have a full header
    // We do not display full header when we have multiple actors, or when the actor and
    // the attached post actor is the same
    bool fullHeader = true;
    if (actors.count() > 1) {
        fullHeader = false;
    } else {
        const QVariantList attachedStoryActors = object().value("attachedStoryActors").toList();
        for (const QVariant &attachedStoryActor : attachedStoryActors) {
            if (name == getProperty(attachedStoryActor, "name")) {
                fullHeader = false;
                break;
            }
        }
    }

    if (m_fullHeader != fullHeader) {
        m_fullHeader = fullHeader;
        emit fullHeaderChanged();
    }

    if (fullHeader) {
        profilePicture = getProperty(actor, "profilePicture");
    }

    if (m_profilePicture != profilePicture) {
        m_profilePicture = profilePicture;
        emit profilePictureChanged();
    }

    performHeaderCreation(actor.toMap());

    QDateTime timestamp = QDateTime::fromTime_t(getVariantProperty(object(), "timestamp").toUInt());
    if (m_timestamp != timestamp) {
        m_timestamp = timestamp;
        emit timestampChanged();
    }

    QString message = getProperty(object(), "message").trimmed();
    message.replace("<", "&lt;");
    message.replace(">", "&gt;");
    message.replace("\n", "<br/>");

    if (!m_fancy) {
        message = elideText(message, 120);
    }
//    } else {
//        QList<QObject *> tags;
//        if (!m_story) {
//            tags = m_messageTags;
//        } else {
//            tags = m_storyTags;
//        }

//        QString remainingMessage = message;
//        message.clear();
//        std::sort(tags.begin(), tags.end(), tagLesser);

//        int previousOffset = 0;
//        foreach (QObject *tag, tags) {
//            int offset = getValue(tag, "offset").toInt();
//            int length = getValue(tag, "length").toInt();
//            QString identifier = getValue(tag, "userIdentifier").toString();
//            QString name = getValue(tag, "userName").toString();

//            message.append(remainingMessage.left(offset - previousOffset));
//            remainingMessage = remainingMessage.remove(0, offset + length - previousOffset);
//            previousOffset = offset + length;
//            message.append(decorate(name, QString(ITEM_TEMPLATE).arg(identifier)));
//        }
//        message.append(remainingMessage);

//        QRegularExpression urlRegExp(URL_REGEXP);
//        remainingMessage = message;
//        message.clear();

//        QRegularExpressionMatchIterator i = urlRegExp.globalMatch(remainingMessage);
//        int previousIndex = 0;
//        while (i.hasNext()) {
//            QRegularExpressionMatch match = i.next();
//            QString captured = match.captured(1);


//            QString url = captured;
//            if (!url.startsWith("http://")) {
//                url.prepend("http://");
//            }
//            message.append(remainingMessage.mid(previousIndex, match.capturedStart(1) - previousIndex));
//            previousIndex = match.capturedEnd(1);

//            message.append(decorate(captured, QString(URL_TEMPLATE).arg(url)));
//        }
//        message.append(remainingMessage.right(remainingMessage.length() - previousIndex));
//    }

    if (!message.isEmpty()) {
        message = standardize(message);
    }
    if (m_message != message) {
        m_message = message;
        emit messageChanged();
    }

//    // Has content ?
//    bool newHasContent = !getValue(object(), "picture").isNull() && !getValue(object(), "name").isNull();
//    if (m_hasContent != newHasContent) {
//        m_hasContent = newHasContent;
//        emit hasContentChanged();
//    }

    // Footer
    QVariant likesVariant = getVariantProperty(object(), "likes");
    bool hasFooter = !likesVariant.isNull();

    if (m_hasFooter != hasFooter) {
        m_hasFooter = hasFooter;
        emit hasFooterChanged();
    }

    int likes = likesVariant.toInt();
    int comments = getVariantProperty(object(), "comments").toInt();
    QString footer = FooterHelper::makeFooter(likes, comments);

    if (m_footer != footer) {
        m_footer = footer;
        emit footerChanged();
    }

//    // Name / description / content
//    bool havePicture = !getValue(object(), "picture").toUrl().isEmpty();
//    QString name = elideText(getValue(object(), "name").toString(), havePicture ? 30 : 70);
//    QString caption = elideText(getValue(object(), "caption").toString(), havePicture ? 50 : 120);
//    QString description = elideText(getValue(object(), "description").toString(), havePicture ? 50 : 120);

//    if (m_name != name) {
//        m_name = name;
//        emit nameChanged();
//    }
//    if (m_caption != caption) {
//        m_caption = caption;
//        emit captionChanged();
//    }
//    if (m_description != description) {
//        m_description = description;
//        emit descriptionChanged();
//    }


//    // Via
//    QString via;
//    QObject *application = getValue(object(), "application").value<QObject *>();
//    QString applicationName;
//    if (application) {
//        applicationName = getValue(application, "objectName").toString();
//    }
//    if (!applicationName.isEmpty()) {
//        //: Translate the "via <appication> footer that is used to indicate the application used to post this post. %1 is replaced by the name of the application.
//        //% "Via %1"
//        via = qtTrId("friends_posthelper_via").arg(applicationName);
//    }
//    if (m_via != via) {
//        m_via = via;
//        emit viaChanged();
//    }
}

QVariant PostHelper::getVariantProperty(const QVariant &object, const QString &key)
{
    return getVariantProperty(object.toMap(), key);
}

QVariant PostHelper::getVariantProperty(const QVariantMap &object, const QString &key)
{
    return object.value(key);
}

QString PostHelper::getProperty(const QVariant &object, const QString &key)
{
    return getVariantProperty(object.toMap(), key).toString();
}

QString PostHelper::getProperty(const QVariantMap &object, const QString &key)
{
    return getVariantProperty(object, key).toString();
}

void PostHelper::performHeaderCreation(const QVariantMap &actor)
{
    QString header;
    QString title = getProperty(object(), "title");
    if (!title.isEmpty()) {
        header = standardize(title);
    } else {
        // Process from and to
        QString toIdentifier = getProperty(object(), "toId");
        QString toName = getProperty(object(), "toName");
        QString toHeader;
        bool toEmpty = toIdentifier.isEmpty() || toName.isEmpty();
        if (!toEmpty) {
            QString elidedTo = elideText(toName, ELIDE_COUNT);
            toHeader = decorate(elidedTo, QString(ITEM_TEMPLATE).arg(toIdentifier));
        }

        QString fromName = getProperty(actor, "name");
        QString fromIdentifier = getProperty(actor, "id");
        QString elidedFrom;
        if (!toEmpty) {
            elidedFrom = elideText(fromName, ELIDE_COUNT);
        } else {
            elidedFrom = elideText(fromName, 2 * ELIDE_COUNT);
        }
        header = decorate(elidedFrom, QString(ITEM_TEMPLATE).arg(fromIdentifier));

        if (!toEmpty) {
            header.append(" &gt; ");
            header.append(toHeader);
            header = standardize(header);
        }
    }

    if (m_header != header) {
        m_header = header;
        emit headerChanged();
    }
}


QString PostHelper::elideText(const QString &text, int count)
{
    if (text.size() <= count) {
        return text;
    }
    QString elidedText = text.left(count - 2);
    elidedText.append(QString::fromUtf8(" …"));
    return elidedText;
}
