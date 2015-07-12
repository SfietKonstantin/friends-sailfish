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
#include <QtCore/QSize>
#include <QtCore/QStringList>

class PostHelper : public AbstractDisplayHelper
{
    Q_OBJECT
    Q_PROPERTY(bool fancy READ isFancy WRITE setFancy NOTIFY fancyChanged)
    Q_PROPERTY(bool attachedStory READ isAttachedStory WRITE setAttachedStory NOTIFY attachedStoryChanged)
    Q_PROPERTY(bool fullHeader READ isFullHeader NOTIFY fullHeaderChanged)
    Q_PROPERTY(bool hasAttachedStory READ hasAttachedStory NOTIFY hasAttachedStoryChanged)
    Q_PROPERTY(QString profilePicture READ profilePicture NOTIFY profilePictureChanged)
    Q_PROPERTY(QString header READ header NOTIFY headerChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY timestampChanged)
    Q_PROPERTY(QString footer READ footer NOTIFY footerChanged)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    Q_PROPERTY(QString via READ via NOTIFY viaChanged)
    Q_PROPERTY(bool hasFooter READ hasFooter NOTIFY hasFooterChanged)
    Q_PROPERTY(QString attachment READ attachment NOTIFY attachmentChanged)
    Q_PROPERTY(QSize attachmentSize READ attachmentSize NOTIFY attachmentSizeChanged)
    Q_PROPERTY(QStringList subAttachments READ subAttachments NOTIFY subAttachmentsChanged)
    Q_PROPERTY(QString attachmentTitle READ attachmentTitle NOTIFY attachmentTitleChanged)
    Q_PROPERTY(QString attachmentDescription READ attachmentDescription NOTIFY attachmentDescriptionChanged)
    Q_PROPERTY(QString attachmentSource READ attachmentSource NOTIFY attachmentSourceChanged)
    Q_PROPERTY(LayoutType layoutType READ layoutType NOTIFY layoutTypeChanged)
    Q_ENUMS(LayoutType)
public:
    enum LayoutType
    {
        Unknown,
        None,
        Share,
        Photo,
        Album,
        Video,
        Avatar,
        Event
    };
    explicit PostHelper(QObject *parent = 0);
    bool isFancy() const;
    void setFancy(bool fancy);
    bool isAttachedStory() const;
    void setAttachedStory(bool attachedStory);
    bool isFullHeader() const;
    bool hasAttachedStory() const;
    QString profilePicture() const;
    QString header() const;
    QDateTime timestamp() const;
    QString footer() const;
    QString message() const;
    QString via() const;
    bool hasFooter() const;
    QString attachment() const;
    QSize attachmentSize() const;
    QStringList subAttachments() const;
    QString attachmentTitle() const;
    QString attachmentDescription() const;
    QString attachmentSource() const;
    LayoutType layoutType() const;
signals:
    void fancyChanged();
    void attachedStoryChanged();
    void fullHeaderChanged();
    void hasAttachedStoryChanged();
    void profilePictureChanged();
    void timestampChanged();
    void headerChanged();
    void footerChanged();
    void messageChanged();
    void viaChanged();
    void hasFooterChanged();
    void attachmentChanged();
    void attachmentSizeChanged();
    void subAttachmentsChanged();
    void attachmentTitleChanged();
    void attachmentDescriptionChanged();
    void attachmentSourceChanged();
    void layoutTypeChanged();

protected:
    void performCreationImpl() Q_DECL_OVERRIDE;
private:
    static QVariant getVariantProperty(const QVariant &object, const QString &key);
    static QVariant getVariantProperty(const QVariantMap &object, const QString &key);
    static QString getProperty(const QVariant &object, const QString &key);
    static QString getProperty(const QVariantMap &object, const QString &key);
    void performHeaderCreation(const QVariantMap &actor);
    static QString elideText(const QString &text, int count);
    QString makeField(const QString &fieldName);
    bool m_fancy;
    bool m_attachedStory;
    bool m_fullHeader;
    bool m_hasAttachedStory;
    QString m_profilePicture;
    QString m_header;
    QDateTime m_timestamp;
    QString m_footer;
    QString m_message;
    QString m_via;
    bool m_hasFooter;
    QString m_attachment;
    QSize m_attachmentSize;
    QStringList m_subAttachments;
    QString m_attachmentTitle;
    QString m_attachmentDescription;
    QString m_attachmentSource;
    LayoutType m_layoutType;
};

#endif // POSTHELPER_H
