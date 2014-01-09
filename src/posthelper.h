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

#include <QtCore/QObject>
#include <QtCore/QDateTime>

class PostHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject * post READ post WRITE setPost NOTIFY postChanged)
    Q_PROPERTY(QObject * from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QObject * to READ to WRITE setTo NOTIFY toChanged)
    Q_PROPERTY(bool fancy READ fancy WRITE setFancy NOTIFY fancyChanged)
    Q_PROPERTY(QString userIdentifier READ userIdentifier WRITE setUserIdentifier
               NOTIFY userIdentifierChanged)
    Q_PROPERTY(QString highlightColor READ highlightColor WRITE setHighlightColor
               NOTIFY highlightColorChanged)

    Q_PROPERTY(QString header READ header NOTIFY headerChanged)
    Q_PROPERTY(QString footer READ footer NOTIFY footerChanged)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    Q_PROPERTY(QString via READ via NOTIFY viaChanged)
    Q_PROPERTY(bool story READ isStory NOTIFY storyChanged)
    Q_PROPERTY(bool hasContent READ hasContent NOTIFY hasContentChanged)

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString caption READ caption NOTIFY captionChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
public:
    explicit PostHelper(QObject *parent = 0);
    QObject * post() const;
    void setPost(QObject *post);
    QObject * from() const;
    void setFrom(QObject *from);
    QObject * to() const;
    void setTo(QObject *to);
    bool fancy() const;
    void setFancy(bool fancy);
    QString userIdentifier() const;
    void setUserIdentifier(const QString &userIdentifier);
    QString highlightColor() const;
    void setHighlightColor(const QString &highlightColor);

    QString header() const;
    QString footer() const;
    QString message() const;
    QString via() const;
    bool isStory() const;
    bool hasContent() const;
    QString name() const;
    QString caption() const;
    QString description() const;
public slots:
    void clearMessageTags();
    void addMessageTag(QObject *messageTag);
    void clearStoryTags();
    void addStoryTag(QObject *storyTag);
signals:
    void postChanged();
    void fromChanged();
    void toChanged();
    void fancyChanged();
    void userIdentifierChanged();
    void highlightColorChanged();
    void footerChanged();
    void headerChanged();
    void messageChanged();
    void viaChanged();
    void storyChanged();
    void hasContentChanged();
    void nameChanged();
    void captionChanged();
    void descriptionChanged();
protected:
    bool event(QEvent *e);
private slots:
    void createPost();
private:
    void performPostCreation();
    void performHeaderCreation();
    static QString elideText(const QString &text, int count);
    QObject *m_post;
    QObject *m_from;
    QObject *m_to;
    bool m_fancy;
    QString m_userIdentifier;
    QString m_highlightColor;
    QList<QObject *> m_messageTags;
    QList<QObject *> m_storyTags;
    QString m_header;
    QString m_footer;
    QString m_message;
    QString m_via;
    bool m_story;
    bool m_hasContent;
    QString m_name;
    QString m_caption;
    QString m_description;
};

#endif // POSTHELPER_H
