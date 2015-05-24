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

#include "tokenmanager.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtCore/QUuid>
#include "defines_p.h"

static const char *TOKEN_KEY = "login/token";
static const char *USERID_KEY = "login/userId";
static const char *DEVICEID_KEY = "login/deviceId";
static const char *MACHINEID_KEY = "login/machineId";

TokenManager::TokenManager(QObject *parent) :
    QObject(parent)
{
    QSettings settings (ORGANIZATION_NAME, APPLICATION_NAME);
    setToken(settings.value(TOKEN_KEY, QString()).toString());
    setUserId(settings.value(USERID_KEY, QString()).toString());
    setDeviceId(settings.value(DEVICEID_KEY, generateMachineId()).toString());
    setMachineId(settings.value(MACHINEID_KEY, QString()).toString());
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

QString TokenManager::userId() const
{
    return m_userId;
}

void TokenManager::setUserId(const QString &userId)
{
    if (m_userId != userId) {
        m_userId = userId;
        emit userIdChanged();
        QSettings settings;
        settings.setValue(USERID_KEY, userId);
    }
}

QString TokenManager::deviceId() const
{
    return m_deviceId;
}

void TokenManager::setDeviceId(const QString &deviceId)
{
    if (m_deviceId != deviceId) {
        m_deviceId = deviceId;
        emit deviceIdChanged();
        QSettings settings;
        settings.setValue(DEVICEID_KEY, deviceId);
    }
}

QString TokenManager::machineId() const
{
    return m_machineId;
}

void TokenManager::setMachineId(const QString &machineId)
{
    if (m_machineId != machineId) {
        m_machineId = machineId;
        emit machineIdChanged();
        QSettings settings;
        settings.setValue(MACHINEID_KEY, machineId);
    }
}

void TokenManager::clear()
{
    setToken(QString());

    // We need to remove the cache TODO: move this as independant of app names ?
    QString sharePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    if (!sharePath.contains(QCoreApplication::instance()->applicationName())) {
        return;
    }

    if (!sharePath.contains(QCoreApplication::instance()->organizationName())) {
        return;
    }

    QDir dataDir (sharePath);
    dataDir.removeRecursively();
}

QString TokenManager::generateMachineId()
{
    return QUuid::createUuid().toString().remove("{").remove("}");
}
