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

#include "settingsmanager.h"
#include <QtCore/QSettings>

static const char *WELCOME_DONE_KEY = "welcome/done";
static const char *VERSION_KEY = "version/version";
static const char *FILTERADS_KEY = "newsfeed/filterads";

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent), m_welcomeDone(false)
{
    QSettings settings;
    setWelcomeDone(settings.value(WELCOME_DONE_KEY, false).toBool());
    setVersion(settings.value(VERSION_KEY, QString()).toString());
    setFilterAds(settings.value(FILTERADS_KEY, false).toBool());
}

bool SettingsManager::welcomeDone() const
{
    return m_welcomeDone;
}

void SettingsManager::setWelcomeDone(bool welcomeDone)
{
    if (m_welcomeDone != welcomeDone) {
        m_welcomeDone = welcomeDone;
        emit welcomeDoneChanged();
        QSettings settings;
        settings.setValue(WELCOME_DONE_KEY, welcomeDone);
    }
}

QString SettingsManager::version() const
{
    return m_version;
}

void SettingsManager::setVersion(const QString &version)
{
    if (m_version != version) {
        m_version = version;
        emit versionChanged();
        QSettings settings;
        settings.setValue(VERSION_KEY, version);
    }
}

bool SettingsManager::filterAds() const
{
    return m_filterAds;
}

void SettingsManager::setFilterAds(bool filterAds)
{
    if (m_filterAds != filterAds) {
        m_filterAds = filterAds;
        emit filterAdsChanged();
        QSettings settings;
        settings.setValue(FILTERADS_KEY, filterAds);
    }
}
