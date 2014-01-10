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

#include "facebookextrapostinterface.h"
#include "facebook/facebookpostinterface_p.h"

class FacebookExtraPostInterfacePrivate: public FacebookPostInterfacePrivate
{
public:
    explicit FacebookExtraPostInterfacePrivate(FacebookExtraPostInterface *q);
    void emitPropertyChangeSignals(const QVariantMap &oldData, const QVariantMap &newData);
    QStringList media;
    bool isVideo;
private:
    Q_DECLARE_PUBLIC(FacebookExtraPostInterface)
};

void FacebookExtraPostInterfacePrivate::emitPropertyChangeSignals(const QVariantMap &oldData,
                                                                  const QVariantMap &newData)
{
    Q_Q(FacebookExtraPostInterface);
    QVariant oldMedia = oldData.value(MEDIA_KEY);
    QVariant newMedia = newData.value(MEDIA_KEY);
    QVariant oldIsVideo = oldData.value(IS_VIDEO_KEY);
    QVariant newIsVideo = newData.value(IS_VIDEO_KEY);
    QVariant oldFacebookObjecId = oldData.value(FACEBOOK_OBJECT_ID);
    QVariant newFacebookObjecId = newData.value(FACEBOOK_OBJECT_ID);

    if (oldMedia != newMedia)
        emit q->mediaChanged();
    if (oldIsVideo != newIsVideo)
        emit q->isVideoChanged();
    if (oldFacebookObjecId != newFacebookObjecId)
        emit q->facebookObjectIdChanged();

    // Call super class implementation
    FacebookPostInterfacePrivate::emitPropertyChangeSignals(oldData, newData);
}

FacebookExtraPostInterfacePrivate::FacebookExtraPostInterfacePrivate(FacebookExtraPostInterface *q)
    : FacebookPostInterfacePrivate(q)
    , isVideo(false)
{
}

FacebookExtraPostInterface::FacebookExtraPostInterface(QObject *parent) :
    FacebookPostInterface(*(new FacebookExtraPostInterfacePrivate(this)), parent)
{
}

QStringList FacebookExtraPostInterface::media() const
{
    QStringList mediaList = data().value(MEDIA_KEY).toStringList();
    if (mediaList.isEmpty()) {
        QString singlePicture = picture().toString();
        if (singlePicture.isEmpty()) {
            return QStringList();
        }

        QStringList mediumList;
        mediumList.append(singlePicture);
        return mediumList;
    }

    return mediaList;
}

bool FacebookExtraPostInterface::isVideo() const
{
    return data().value(IS_VIDEO_KEY).toBool();
}

QString FacebookExtraPostInterface::facebookObjectId() const
{
    return data().value(FACEBOOK_OBJECT_ID).toString();
}
