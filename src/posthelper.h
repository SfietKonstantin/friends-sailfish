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

#ifndef POSTHELPER_H
#define POSTHELPER_H

#include "abstractdisplayhelper.h"
#include <QtCore/QDateTime>

class PostHelper : public AbstractDisplayHelper
{
    Q_OBJECT
    Q_PROPERTY(QObject * to READ to WRITE setTo NOTIFY toChanged)
    Q_PROPERTY(bool fancy READ fancy WRITE setFancy NOTIFY fancyChanged)

    Q_PROPERTY(QString header READ header NOTIFY headerChanged)
    Q_PROPERTY(QString footer READ footer NOTIFY footerChanged)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    Q_PROPERTY(QString via READ via NOTIFY viaChanged)
    Q_PROPERTY(bool story READ isStory NOTIFY storyChanged)
    Q_PROPERTY(bool hasContent READ hasContent NOTIFY hasContentChanged)
    Q_PROPERTY(bool hasFooter READ hasFooter NOTIFY hasFooterChanged)

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString caption READ caption NOTIFY captionChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
public:
    explicit PostHelper(QObject *parent = 0);
    QObject * to() const;
    void setTo(QObject *to);
    bool fancy() const;
    void setFancy(bool fancy);

    QString header() const;
    QString footer() const;
    QString message() const;
    QString via() const;
    bool isStory() const;
    bool hasContent() const;
    bool hasFooter() const;
    QString name() const;
    QString caption() const;
    QString description() const;
public slots:
    void clearMessageTags();
    void addMessageTag(QObject *messageTag);
    void clearStoryTags();
    void addStoryTag(QObject *storyTag);
signals:
    void toChanged();
    void fancyChanged();
    void footerChanged();
    void headerChanged();
    void messageChanged();
    void viaChanged();
    void storyChanged();
    void hasContentChanged();
    void hasFooterChanged();
    void nameChanged();
    void captionChanged();
    void descriptionChanged();
protected:
    void performCreationImpl();
private:
    void performHeaderCreation();
    static QString elideText(const QString &text, int count);
    QObject *m_to;
    bool m_fancy;
    QList<QObject *> m_messageTags;
    QList<QObject *> m_storyTags;
    QString m_header;
    QString m_footer;
    QString m_message;
    QString m_via;
    bool m_story;
    bool m_hasContent;
    bool m_hasFooter;
    QString m_name;
    QString m_caption;
    QString m_description;
};

#endif // POSTHELPER_H
