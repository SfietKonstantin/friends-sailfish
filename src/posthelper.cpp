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
#include "objecthelper_p.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QRegularExpression>
#include <QtCore/QUrl>

static const int ELIDE_COUNT = 50;

static const char *ITEM_TEMPLATE = "item----%1";
static const char *URL_TEMPLATE = "url----%1";
static const char *URL_REGEXP = "((http://|https://|www.)[a-zA-Z0-9_\\.\\-~%/#  ?=&]*)";

bool tagLesser(QObject *tag1, QObject *tag2)
{
    return getValue(tag1, "offset").toInt() < getValue(tag2, "offset").toInt();
}

PostHelper::PostHelper(QObject *parent)
    : AbstractDisplayHelper(parent)
    , m_to(0)
    , m_fancy(true)
    , m_story(false)
    , m_hasContent(false)
    , m_hasFooter(true)
{
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
        create();
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
        create();
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

void PostHelper::clearMessageTags()
{
    m_messageTags.clear();
    create();
}

void PostHelper::addMessageTag(QObject *messageTag)
{
    m_messageTags.append(messageTag);
    create();
}

void PostHelper::clearStoryTags()
{
    m_storyTags.clear();
    create();
}

void PostHelper::addStoryTag(QObject *storyTag)
{
    m_storyTags.append(storyTag);
    create();
}

void PostHelper::performCreationImpl()
{
    QString story = getValue(object(), "story").toString();
    if (!story.isEmpty()) {
        m_story = true;
        emit storyChanged();
    } else {
        m_story = false;
        emit storyChanged();
    }

    performHeaderCreation();

    QString message = getValue(object(), !m_story ? "message" : "story").toString();
    message.replace("<", "&lt;");
    message.replace(">", "&gt;");
    message.replace("\n", "<br/>");

    if (!m_fancy) {
        message = elideText(message, 120);
    } else {
        QList<QObject *> tags;
        if (!m_story) {
            tags = m_messageTags;
        } else {
            tags = m_storyTags;
        }

        QString remainingMessage = message;
        message.clear();
        std::sort(tags.begin(), tags.end(), tagLesser);

        int previousOffset = 0;
        foreach (QObject *tag, tags) {
            int offset = getValue(tag, "offset").toInt();
            int length = getValue(tag, "length").toInt();
            QString identifier = getValue(tag, "userIdentifier").toString();
            QString name = getValue(tag, "userName").toString();

            message.append(remainingMessage.left(offset - previousOffset));
            remainingMessage = remainingMessage.remove(0, offset + length - previousOffset);
            previousOffset = offset + length;
            message.append(decorate(name, QString(ITEM_TEMPLATE).arg(identifier)));
        }
        message.append(remainingMessage);

        QRegularExpression urlRegExp(URL_REGEXP);
        remainingMessage = message;
        message.clear();

        QRegularExpressionMatchIterator i = urlRegExp.globalMatch(remainingMessage);
        int previousIndex = 0;
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QString captured = match.captured(1);


            QString url = captured;
            if (!url.startsWith("http://")) {
                url.prepend("http://");
            }
            message.append(remainingMessage.mid(previousIndex, match.capturedStart(1) - previousIndex));
            previousIndex = match.capturedEnd(1);

            message.append(decorate(captured, QString(URL_TEMPLATE).arg(url)));
        }
        message.append(remainingMessage.right(remainingMessage.length() - previousIndex));
    }

    if (m_message != message) {
        m_message = message;
        emit messageChanged();
    }

    // Has content ?
    bool newHasContent = !getValue(object(), "picture").isNull() && !getValue(object(), "name").isNull();
    if (m_hasContent != newHasContent) {
        m_hasContent = newHasContent;
        emit hasContentChanged();
    }

    // Footer
    bool hasFooter = true;
    QVariantMap data = getValue(object(), "data").toMap();
    if (!data.contains("likes") || !data.contains("comments")) {
        hasFooter = false;
    }

    if (m_hasFooter != hasFooter) {
        m_hasFooter = hasFooter;
        emit hasFooterChanged();
    }

    int likesCount = getValue(object(), "likesCount").toInt();
    int commentsCount = getValue(object(), "commentsCount").toInt();
    QString footer = FooterHelper::makeFooter(likesCount, commentsCount);

    if (m_footer != footer) {
        m_footer = footer;
        emit footerChanged();
    }

    // Name / description / content
    bool havePicture = !getValue(object(), "picture").toUrl().isEmpty();
    QString name = elideText(getValue(object(), "name").toString(), havePicture ? 30 : 70);
    QString caption = elideText(getValue(object(), "caption").toString(), havePicture ? 50 : 120);
    QString description = elideText(getValue(object(), "description").toString(), havePicture ? 50 : 120);

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
    QObject *application = getValue(object(), "application").value<QObject *>();
    QString applicationName;
    if (application) {
        applicationName = getValue(application, "objectName").toString();
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

void PostHelper::performHeaderCreation()
{
    // Process from and to
    QString toIdentifier;
    QString toName;
    if (m_to) {
        toIdentifier = getValue(m_to, "objectIdentifier").toString();
        toName = getValue(m_to, "objectName").toString();
    }
    QString toHeader;
    if (!toIdentifier.isEmpty() && !toName.isEmpty()) {
        QString elidedTo = elideText(toName, ELIDE_COUNT);
        toHeader = decorate(elidedTo, QString(ITEM_TEMPLATE).arg(toIdentifier));
    }
    QString elidedFrom;
    QString fromName;
    QString fromIdentifier;

    QObject *from = getValue(object(), "from").value<QObject *>();
    fromName = getValue(from, "objectName").toString();
    fromIdentifier = getValue(from, "objectIdentifier").toString();
    if (!toIdentifier.isEmpty() && !toName.isEmpty()) {
        elidedFrom = elideText(fromName, ELIDE_COUNT);
    } else {
        elidedFrom = elideText(fromName, 2 * ELIDE_COUNT);
    }
    QString header = decorate(elidedFrom, QString(ITEM_TEMPLATE).arg(fromIdentifier));

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
