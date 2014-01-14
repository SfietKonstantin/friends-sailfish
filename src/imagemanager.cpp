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

#include "imagemanager.h"
#include "cachehelper_p.h"
#include <QtCore/QDebug>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDir>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtCore/QUrlQuery>
#endif

ImageManager::ImageManager(QObject *parent) :
    QObject(parent)
{
    manageCache();
}

QUrl ImageManager::realSource(const QUrl &url) const
{
    QString file = QString::fromLocal8Bit(QCryptographicHash::hash(url.toString().toLocal8Bit(),
                                                                   QCryptographicHash::Md5).toHex());

    QString trueFile = QString("%1.jpg").arg(file);
    QString first = file.left(1);
    if (m_existingImages.contains(trueFile)) {
        return QUrl(QString("file://%1/%2/%3").arg(cacheFolderPath(), first, trueFile));
    }

    return url;
}

void ImageManager::save(const QUrl &url, const QImage &image)
{
    QString file = QString::fromLocal8Bit(QCryptographicHash::hash(url.toString().toLocal8Bit(),
                                                                   QCryptographicHash::Md5).toHex());
    file = QString("%1.jpg").arg(file);
    QString first = file.left(1);

    QDir dir (cacheFolderPath());
    if (!dir.exists()) {
        QDir::root().mkpath(dir.absolutePath());

    }

    if (!dir.exists(first)) {
        dir.mkdir(first);
    }

    dir.cd(first);
    if (dir.exists(file)) {
        return;
    }

    m_existingImages.insert(file);
    image.save(dir.absoluteFilePath(file), "JPG");
}

QUrl ImageManager::pictureUrl(const QString &id, const QString &token, const QString &type)
{
    if (id.isEmpty()) {
        return QUrl();
    }

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

QUrl ImageManager::pictureUrl(const QString &id, const QString &token, int width, int height)
{
    if (id.isEmpty()) {
        return QUrl();
    }

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

void ImageManager::manageCache()
{
    QDir dir (cacheFolderPath());
    foreach (const QString &folder, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
        QDir cacheDir = dir;
        cacheDir.cd(folder);

        foreach (const QString &file, cacheDir.entryList(QDir::Files)) {
            m_existingImages.insert(file);
        }
    }

    // Clear old images (< 0.1.11)
    foreach (const QString &file, dir.entryList(QDir::Files)) {
        dir.remove(file);
    }
}
