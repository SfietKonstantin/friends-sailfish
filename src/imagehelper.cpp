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

#include "imagehelper.h"

ImageHelper::ImageHelper(QObject *parent) :
    QObject(parent), m_imageManager(0), m_cached(false), m_image(0)
{
}

ImageManager * ImageHelper::imageManager() const
{
    return m_imageManager;
}

void ImageHelper::setImageManager(ImageManager *imageManager)
{
    if (m_imageManager != imageManager) {
        m_imageManager = imageManager;
        emit imageManagerChanged();
    }
}

bool ImageHelper::isCached() const
{
    return m_cached;
}

void ImageHelper::setCached(bool cached)
{
    if (m_cached != cached) {
        m_cached = cached;
        emit cachedChanged();
    }
}

QQuickImageBase * ImageHelper::image() const
{
    return m_image;
}

void ImageHelper::setImage(QQuickImageBase *image)
{
    if (m_image != image) {

        if (m_image) {
            m_image->disconnect(this);
        }

        m_image = image;
        emit imageChanged();

        m_image->setSource(m_source);
        connect(m_image, &QQuickImageBase::statusChanged, this, &ImageHelper::slotStatusChanged);
    }
}

QUrl ImageHelper::source() const
{
    return m_source;
}

void ImageHelper::setSource(const QUrl &source)
{
    if (m_source != source) {
        m_source = source;
        emit sourceChanged();

        if (m_image) {
            QUrl realSource = source;
            if (m_imageManager) {
                realSource = m_imageManager->realSource(source);
            }

            m_image->setSource(realSource);
        }
    }
}

void ImageHelper::slotStatusChanged(QQuickImageBase::Status status)
{
    if (status != QQuickImageBase::Ready) {
        return;
    }

    if (m_imageManager && m_cached) {
        m_imageManager->save(m_image->source(), m_image->image());
    }
}
