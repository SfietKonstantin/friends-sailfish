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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QtCore/QObject>
#include <QtCore/QUrl>

class ImageLoaderPrivate;
class ImageLoader : public QObject
{
    Q_OBJECT
public:
    explicit ImageLoader(QObject *parent = 0);
    virtual ~ImageLoader();
    Q_INVOKABLE static QUrl pictureUrl(const QString &id, const QString &token,
                                       const QString &type = QString());
    Q_INVOKABLE static QUrl pictureUrl(const QString &id, const QString &token,
                                       int width, int height);
public Q_SLOTS:
    void load(const QUrl &url);
Q_SIGNALS:
    void error(const QUrl &url);
    void loaded(const QUrl &url, const QString &path);
protected:
    bool event(QEvent *e);
    QScopedPointer<ImageLoaderPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(ImageLoader)
    Q_PRIVATE_SLOT(d_func(), void slotFinished())
    Q_PRIVATE_SLOT(d_func(), void slotError(QNetworkReply::NetworkError))
    Q_PRIVATE_SLOT(d_func(), void slotSslErrors(QList<QSslError>))
};

#endif // IMAGELOADER_H
