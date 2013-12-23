/****************************************************************************************
 * Copyright (C) 2013 Lucien XU <sfietkonstantin@free.fr>                               *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 3 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

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
