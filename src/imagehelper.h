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

#ifndef IMAGEHELPER_H
#define IMAGEHELPER_H

#include <QtCore/QObject>
#include <QtQuick/private/qquickimagebase_p.h>
#include <QtQuick/private/qquickimage_p.h>
#include "imagemanager.h"

class ImageHelper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ImageManager * imageManager READ imageManager WRITE setImageManager
               NOTIFY imageManagerChanged)
    Q_PROPERTY(bool cached READ isCached WRITE setCached NOTIFY cachedChanged)
    Q_PROPERTY(QQuickImageBase *image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
public:
    explicit ImageHelper(QObject *parent = 0);
    ImageManager * imageManager() const;
    void setImageManager(ImageManager *imageManager);
    bool isCached() const;
    void setCached(bool cached);
    QQuickImageBase * image() const;
    void setImage(QQuickImageBase *image);
    QUrl source() const;
    void setSource(const QUrl &source);
signals:
    void imageManagerChanged();
    void cachedChanged();
    void imageChanged();
    void sourceChanged();
private:
    ImageManager *m_imageManager;
    bool m_cached;
    QQuickImageBase *m_image;
    QUrl m_source;
private slots:
    void slotStatusChanged(QQuickImageBase::Status status);
};

#endif // IMAGEHELPER_H
