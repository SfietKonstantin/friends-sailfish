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

#ifndef QFB_COOKIEJAR_H
#define QFB_COOKIEJAR_H

#include <QtNetwork/QNetworkCookieJar>

namespace QFB
{

class CookieJarPrivate;
class CookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    explicit CookieJar(QObject *parent = 0);
    virtual ~CookieJar();
    bool setCookiesFromUrl(const QList<QNetworkCookie> &cookieList, const QUrl &url);
protected:
    QScopedPointer<CookieJarPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(CookieJar)
};

}

#endif // QFB_COOKIEJAR_H
