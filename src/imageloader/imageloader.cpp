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

#include "imageloader.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QMap>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslError>
#include "cachehelper_p.h"
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtCore/QUrlQuery>
#endif

typedef QPair<QUrl, QString> UrlImage;
static const int MAX_COUNT = 4;

class ImageLoaderPrivate
{
public:
    explicit ImageLoaderPrivate(ImageLoader *q);
    static QString imageName(const QUrl &url);
    void createReply(const QUrl &url, const QUrl &originalUrl = QUrl());
    void slotFinished();
    void slotError(QNetworkReply::NetworkError error);
    void slotSslErrors(QList<QSslError> sslErrors);
    QNetworkAccessManager *networkAccessManager;
    QMap<QNetworkReply *, QUrl> replyToUrl;
    QList<QUrl> stack;
    QList<QPair<QUrl, QString> > eventImages;
protected:
    ImageLoader * const q_ptr;
private:
    Q_DECLARE_PUBLIC(ImageLoader)
};

ImageLoaderPrivate::ImageLoaderPrivate(ImageLoader *q):
    networkAccessManager(0), q_ptr(q)
{
}

QString ImageLoaderPrivate::imageName(const QUrl &url)
{
    QByteArray asciiUrl = url.toString().toLocal8Bit();
    QByteArray encodedUrl = QCryptographicHash::hash(asciiUrl, QCryptographicHash::Md5);
    return QString("img_%1.jpg").arg(QString(encodedUrl.toHex()));
}

void ImageLoaderPrivate::createReply(const QUrl &url, const QUrl &originalUrl)
{
    Q_Q(ImageLoader);
    QNetworkReply *reply = networkAccessManager->get(QNetworkRequest(url));
    if (originalUrl.isEmpty()) {
        replyToUrl.insert(reply, url);
    } else {
        replyToUrl.insert(reply, originalUrl);
    }
    QObject::connect(reply, SIGNAL(finished()), q, SLOT(slotFinished()));
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                     q, SLOT(slotError(QNetworkReply::NetworkError)));
    QObject::connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
                     q, SLOT(slotSslErrors(QList<QSslError>)));
}

void ImageLoaderPrivate::slotFinished()
{
    Q_Q(ImageLoader);
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(q->sender());
    if (!reply) {
        return;
    }

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }

    if (!replyToUrl.contains(reply)) {
        reply->deleteLater();
        return;
    }

    QUrl url = replyToUrl.take(reply);

    // Check redirect
    QVariant possibleRedirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    QUrl possibleRedirectUrl = possibleRedirect.toUrl();
    if (!possibleRedirectUrl.isEmpty() && possibleRedirectUrl != url) {
        reply->deleteLater();
        createReply(possibleRedirectUrl, url);
        return;
    }

    QString path = cacheFolderPath();
    QDir::root().mkpath(path);
    QDir dir = QDir(path);
    QString fileName = imageName(url);

    QByteArray data = reply->readAll();
    reply->deleteLater();
    QImage image;
    bool ok = false;
    if (image.loadFromData(data, "JPG")) {
        ok = true;
    }
    if (!ok) {
        if (image.loadFromData(data, "PNG")) {
            ok = true;
        }
    }
    if (!ok) {
        if (image.loadFromData(data, "GIF")) {
            ok = true;
        }
    }

    if (image.isNull()) {
        emit q->error(url);
    }

    QString imagePath = dir.absoluteFilePath(fileName);
    image.save(imagePath, "JPG");
#ifdef DESKTOP
    imagePath.prepend("file://");
#endif
    emit q->loaded(url, imagePath);

    while (replyToUrl.count() < MAX_COUNT && !stack.isEmpty()) {
        createReply(stack.takeLast());
    }
}

void ImageLoaderPrivate::slotError(QNetworkReply::NetworkError error)
{
    Q_Q(ImageLoader);
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(q->sender());
    if (!reply) {
        return;
    }

    if (!replyToUrl.contains(reply)) {
        return;
    }

    QUrl url = replyToUrl.take(reply);
    qDebug() << "Error: " << error << url;

    emit q->error(url);
}

void ImageLoaderPrivate::slotSslErrors(QList<QSslError> sslErrors)
{
    Q_Q(ImageLoader);
    Q_UNUSED(sslErrors)
    qDebug() << "SSL error";
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(q->sender());
    if (!reply) {
        return;
    }

    if (!replyToUrl.contains(reply)) {
        return;
    }

    emit q->error(replyToUrl.take(reply));
}

////// End of private class //////

ImageLoader::ImageLoader(QObject *parent) :
    QObject(parent), d_ptr(new ImageLoaderPrivate(this))
{
    Q_D(ImageLoader);
    d->networkAccessManager = new QNetworkAccessManager(this);
}

ImageLoader::~ImageLoader()
{
}

QUrl ImageLoader::pictureUrl(const QString &id, const QString &token, const QString &type)
{
    QUrl url = QUrl(QString("https://graph.facebook.com/%1/picture").arg(id));
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    url.addQueryItem("access_token", token);
    if (!type.isEmpty()) {
        url.addQueryItem("type", type);
    }
#else
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("access_token", token);
    if (!type.isEmpty()) {
        urlQuery.addQueryItem("type", type);
    }
    url.setQuery(urlQuery);
#endif
    return url;
}

QUrl ImageLoader::pictureUrl(const QString &id, const QString &token, int width, int height)
{
    QUrl url = QUrl(QString("https://graph.facebook.com/%1/picture").arg(id));
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    url.addQueryItem("access_token", token);
    url.addQueryItem("width", QString::number(width));
    url.addQueryItem("height", QString::number(height));
#else
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("access_token", token);
    urlQuery.addQueryItem("width", QString::number(width));
    urlQuery.addQueryItem("height", QString::number(height));
    url.setQuery(urlQuery);
#endif
    return url;
}

void ImageLoader::load(const QUrl &url)
{
    Q_D(ImageLoader);
    if (!url.isValid()) {
        return;
    }

    // Check the cache for existing image
    QDir dir (cacheFolderPath());
    QString fileName = d->imageName(url);

    if (dir.exists(fileName)) {
        QString imagePath = dir.absoluteFilePath(fileName);
#ifdef DESKTOP
        imagePath.prepend("file://");
#endif
        d->eventImages.append(UrlImage(url, imagePath));
        QCoreApplication::postEvent(this, new QEvent(QEvent::User));
        return;
    }

    if (d->replyToUrl.count() >= MAX_COUNT) {
        d->stack.append(url);
        return;
    }

    d->createReply(url);
}

bool ImageLoader::event(QEvent *e)
{
    Q_D(ImageLoader);
    if (e->type() == QEvent::User) {
        if (!d->eventImages.isEmpty()) {
            UrlImage pair = d->eventImages.takeFirst();
            emit loaded(pair.first, pair.second);

            QCoreApplication::postEvent(this, new QEvent(QEvent::User));
        }
        return true;
    } else {
        return QObject::event(e);
    }
}

#include "moc_imageloader.cpp"
