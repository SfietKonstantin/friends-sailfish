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
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QRegExp>
#include <QtCore/QUrl>

static const int ELIDE_COUNT = 50;

static const char *RICH_TEXT_NAME
    = "<a style=\"text-decoration:none; color:%1\" href=\"user----%2----%3\">%4</a>";

static const char *URL_REGEXP = "((http://|https://|www.)[a-zA-Z0-9_\\.\\-~%/#?]*)";
static const char *RICH_TEXT_URL
    = "<a style=\"text-decoration:none; color:#%1\" href=\"url----%2\">%3</a>";

inline QVariant get(QObject *object, const char *property)
{
    const QMetaObject *metaObject = object->metaObject();
    int index = metaObject->indexOfProperty(property);
    if (index == -1) {
        return QVariant();
    }

    QMetaProperty metaProperty = metaObject->property(index);
    return metaProperty.read(object);
}


//bool tagLesser(QFB::PostTag *tag1, QFB::PostTag *tag2)
//{
//    return tag1->offset() < tag2->offset();
//}

PostHelper::PostHelper(QObject *parent) :
    QObject(parent)
{
    m_post = 0;
    m_from = 0;
    m_to = 0;
    m_fancy = true;
    m_story = false;
    m_hasContent = false;
}

QObject * PostHelper::post() const
{
    return m_post;
}

void PostHelper::setPost(QObject *post)
{
    if (m_post != post) {
        if (m_post) {
            m_post->disconnect(this);
        }

        m_post = post;
        emit postChanged();

        connect(m_post, SIGNAL(likesCountChanged()), this, SLOT(createPost()));
        connect(m_post, SIGNAL(commentsCountChanged()), this, SLOT(createPost()));

        createPost();
    }
}

QObject * PostHelper::from() const
{
    return m_from;
}

void PostHelper::setFrom(QObject *from)
{
    if (m_from != from) {
        m_from = from;
        emit fromChanged();
        createPost();
    }
}

QObject * PostHelper::to() const
{
    return m_to;
}

void PostHelper::setTo(QObject *to)
{
    if (m_to != to) {
        m_to = to;
        emit toChanged();
        createPost();
    }
}

bool PostHelper::fancy() const
{
    return m_fancy;
}

void PostHelper::setFancy(bool fancy)
{
    if (m_fancy != fancy) {
        m_fancy = fancy;
        emit fancyChanged();
        createPost();
    }
}

QString PostHelper::userIdentifier() const
{
    return m_userIdentifier;
}

void PostHelper::setUserIdentifier(const QString &userIdentifier)
{
    if (m_userIdentifier != userIdentifier) {
        m_userIdentifier = userIdentifier;
        emit userIdentifierChanged();
        createPost();
    }
}

QString PostHelper::highlightColor() const
{
    return m_highlightColor;
}

void PostHelper::setHighlightColor(const QString &highlightColor)
{
    if (m_highlightColor != highlightColor) {
        m_highlightColor = highlightColor;
        emit highlightColorChanged();
        createPost();
    }
}

QString PostHelper::header() const
{
    return m_header;
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

bool PostHelper::isStory() const
{
    return m_story;
}

bool PostHelper::hasContent() const
{
    return m_hasContent;
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

void PostHelper::clearMessageTags()
{
    m_messageTags.clear();
}

void PostHelper::appendMessageTag(QObject *messageTag)
{
    m_messageTags.append(messageTag);
    createPost();
}

bool PostHelper::event(QEvent *e)
{
    if (e->type() == QEvent::User) {
        performPostCreation();
        return true;
    }
    return QObject::event(e);
}

void PostHelper::createPost()
{
    QCoreApplication::instance()->postEvent(this, new QEvent(QEvent::User));
}


void PostHelper::performPostCreation()
{
    if (!m_post) {
        return;
    }

    if (!m_from) {
        return;
    }

    QString story = get(m_post, "story").toString();
    if (!story.isEmpty()) {
        m_story = true;
        emit storyChanged();
    } else {
        m_story = false;
        emit storyChanged();
    }

    performHeaderAndMessageCreation();

    // Process message
//    QString message;
//    if (m_fancy) {
//        QString endMessage = get(m_post, "message").toString();
////        QList<QFB::PostTag *> messageTags = m_post->messageTags();
////        qSort(messageTags.begin(), messageTags.end(), tagLesser);

//        int previousOffset = 0;
//        foreach (QFB::PostTag *tag, messageTags) {
//            message.append(endMessage.left(tag->offset() - previousOffset));
//            endMessage = endMessage.remove(0, tag->offset() + tag->length() - previousOffset);
//            previousOffset = tag->offset() + tag->length();
//            message.append(QString(RICH_TEXT_NAME).arg(tag->facebookId(), tag->name(),
//                                                       tag->name()));
//        }
//        message.append(endMessage);

//        // Parse links
//        endMessage = message;
//        message.clear();

//        QRegExp urlRegExp(URL_REGEXP);
//        int nextUrlIndex = endMessage.indexOf(urlRegExp);
//        while (nextUrlIndex != -1) {
//            QString captured = urlRegExp.cap(1);
//            QString url = captured;
//            if (!url.startsWith("http://")) {
//                url.prepend("http://");
//            }
//            message.append(endMessage.left(nextUrlIndex));
//            endMessage = endMessage.remove(0, nextUrlIndex + captured.size());
//            message.append(QString(RICH_TEXT_URL).arg(url, captured));
//            nextUrlIndex = endMessage.indexOf(urlRegExp);
//        }
//        message.append(endMessage);
//    } else {

    // Has content ?
    bool newHasContent = !get(m_post, "picture").isNull() && !get(m_post, "name").isNull();
    if (m_hasContent != newHasContent) {
        m_hasContent = newHasContent;
        emit hasContentChanged();
    }

    // Footer
    int likesCount = get(m_post, "likesCount").toInt();
    int commentsCount = get(m_post, "commentsCount").toInt();
    QString footer = FooterHelper::makeFooter(likesCount, commentsCount);

    if (m_footer != footer) {
        m_footer = footer;
        emit footerChanged();
    }

    // Name / description / content
    bool havePicture = !get(m_post, "picture").toUrl().isEmpty();
    QString name = elideText(get(m_post, "name").toString(), havePicture ? 30 : 70);
    QString caption = elideText(get(m_post, "caption").toString(), havePicture ? 50 : 120);
    QString description = elideText(get(m_post, "description").toString(), havePicture ? 50 : 120);

    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
    if (m_caption != caption) {
        m_caption = caption;
        emit captionChanged();
    }
    if (m_description != description) {
        m_description = description;
        emit descriptionChanged();
    }


    // Via
    QString via;
    QObject *application = get(m_post, "application").value<QObject *>();
    QString applicationName;
    if (application) {
        applicationName = get(application, "objectName").toString();
    }
    if (!applicationName.isEmpty()) {
        //: Translate the "via <appication> footer that is used to indicate the application used to post this post. %1 is replaced by the name of the application.
        //% "Via %1"
        via = qtTrId("friends_posthelper_via").arg(applicationName);
    }
    if (m_via != via) {
        m_via = via;
        emit viaChanged();
    }

}

void PostHelper::performHeaderAndMessageCreation()
{
    QString message = get(m_post, !m_story ? "message" : "story").toString();
    if (!m_fancy) {
        message = elideText(message, 120);
    }

    message.replace("<", "&lt;");
    message.replace(">", "&gt;");
    message.replace("\n", "<br/>");

    if (m_message != message) {
        m_message = message;
        emit messageChanged();
    }

    // Process from and to
    QString toIdentifier;
    QString toName;
    if (m_to) {
        toIdentifier = get(m_to, "objectIdentifier").toString();
        toName = get(m_to, "objectName").toString();
    }
    QString toHeader = RICH_TEXT_NAME;
    if (!toIdentifier.isEmpty() && !toName.isEmpty()) {
        QString elidedTo = elideText(toName, ELIDE_COUNT);
        toHeader = toHeader.arg(m_highlightColor, toIdentifier, toName, elidedTo);
    }
    QString elidedFrom;
    QString fromName;
    QString fromIdentifier;
    fromName = get(m_from, "objectName").toString();
    fromIdentifier = get(m_from, "objectIdentifier").toString();
    if (!toIdentifier.isEmpty() && !toName.isEmpty()) {
        elidedFrom = elideText(fromName, ELIDE_COUNT);
    } else {
        elidedFrom = elideText(fromName, 2 * ELIDE_COUNT);
    }
    QString header = RICH_TEXT_NAME;
    header = header.arg(m_highlightColor, fromIdentifier, fromName, elidedFrom);

    if (!toIdentifier.isEmpty() && !toName.isEmpty()) {
        header.append(" &gt; ");
        header.append(toHeader);
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
