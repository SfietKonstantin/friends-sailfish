/****************************************************************************************
 * Copyright (C) 2012 Lucien XU <sfietkonstantin@free.fr>                               *
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

#include "tokenmanager.h"
#include <QtCore/QSettings>

static const char *TOKEN_KEY = "login/token";

TokenManager::TokenManager(QObject *parent) :
    QObject(parent)
{
    QSettings settings;
    setToken(settings.value(TOKEN_KEY, QString()).toString());
}

QString TokenManager::token() const
{
    return m_token;
}

void TokenManager::setToken(const QString &token)
{
    if (m_token != token) {
        m_token = token;
        emit tokenChanged();
        QSettings settings;
        settings.setValue(TOKEN_KEY, token);
    }
}
