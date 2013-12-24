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

#include "cookiejar.h"
#include <QtCore/QDebug>
#include <QtCore/QDir>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/QDesktopServices>
#else
#include <QtCore/QStandardPaths>
#include <QtNetwork/QNetworkCookie>
#endif

namespace QFB
{

class CookieJarPrivate
{
public:
    QList<QNetworkCookie> load();
    void save(const QList<QNetworkCookie> &cookies);
};

QList<QNetworkCookie> CookieJarPrivate::load()
{
    QList<QNetworkCookie> cookies;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif
    QDir dir (path);
    QFile file (dir.filePath("cookies"));
    if (!file.open(QIODevice::ReadOnly)) {
        return cookies;
    }

    while (!file.atEnd()) {
        QByteArray data = file.readLine();
        QList<QNetworkCookie> parsedCookies = QNetworkCookie::parseCookies(data);
        cookies.append(parsedCookies);
    }


    file.close();
    return cookies;
}

void CookieJarPrivate::save(const QList<QNetworkCookie> &cookies)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
    QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif
    QDir::root().mkpath(path);
    QDir dir (path);
    QFile file (dir.filePath("cookies"));
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    foreach (QNetworkCookie cookie, cookies) {
        file.write(cookie.toRawForm());
        file.write("\n");
    }

    file.close();

}

////// End of private class //////

CookieJar::CookieJar(QObject *parent) :
    QNetworkCookieJar(parent), d_ptr(new CookieJarPrivate)
{
    Q_D(CookieJar);
    setAllCookies(d->load());

}

CookieJar::~CookieJar()
{
}

bool CookieJar::setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url)
{
    Q_D(CookieJar);
    QList<QNetworkCookie> trueCookieList;
    foreach (QNetworkCookie cookie, allCookies()) {
        if (!cookie.isSessionCookie()) {
            trueCookieList.append(cookie);
        }
    }


    d->save(trueCookieList);
    return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

}
