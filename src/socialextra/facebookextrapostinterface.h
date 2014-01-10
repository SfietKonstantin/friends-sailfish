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

#ifndef FACEBOOKEXTRAPOSTINTERFACE_H
#define FACEBOOKEXTRAPOSTINTERFACE_H

#include "facebook/facebookpostinterface.h"

#define MEDIA_KEY "media"
#define IS_VIDEO_KEY "is_video"
#define FACEBOOK_OBJECT_ID "fb_object_id"

class FacebookExtraPostInterfacePrivate;
class FacebookExtraPostInterface : public FacebookPostInterface
{
    Q_OBJECT
    Q_PROPERTY(QString facebookObjectId READ facebookObjectId NOTIFY facebookObjectIdChanged)
    Q_PROPERTY(QStringList media READ media NOTIFY mediaChanged)
    Q_PROPERTY(bool isVideo READ isVideo NOTIFY isVideoChanged)
public:
    explicit FacebookExtraPostInterface(QObject *parent = 0);

    // Accessors
    QString facebookObjectId() const;
    QStringList media() const;
    bool isVideo() const;
Q_SIGNALS:
    void facebookObjectIdChanged();
    void mediaChanged();
    void isVideoChanged();
private:
    Q_DECLARE_PRIVATE(FacebookExtraPostInterface)
};

#endif // FACEBOOKEXTRAPOSTINTERFACE_H
