/*
 * Copyright (C) 2014 Lucien XU <sfietkonstantin@free.fr>
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

#include "newsfeedproxymodel.h"
#include <QtCore/QMetaProperty>
#include <QtCore/QSet>

NewsFeedProxyModel::NewsFeedProxyModel(QObject *parent)
    : AbstractProxyModel(parent), m_filterAd(false)
{
}

bool NewsFeedProxyModel::filterAds() const
{
    return m_filterAd;
}

void NewsFeedProxyModel::setFilterAds(bool filterAd)
{
    if (m_filterAd != filterAd) {
        m_filterAd = filterAd;
        emit filterAdsChanged();
        reload();
    }
}

bool NewsFeedProxyModel::isAutoLoad() const
{
    return false;
}

QList<QVariantMap> NewsFeedProxyModel::filterData(const QList<QVariantMap> &input)
{
    QList<QVariantMap> data;

    for (const QVariantMap &object : input) {
        QVariantList actors = getActors(object);
        if (actors.isEmpty()) {
            continue;
        }
        if (filterAds()) {
            QSet<QString> feedbacks = getNegativeFeedback(object);
            if (feedbacks.contains("HIDE_ADVERTISER")) {
                continue;
            }
        }
        data.append(object);
    }

    return data;
}

QString NewsFeedProxyModel::section(const QVariantMap &object) const
{
    Q_UNUSED(object)
    return QString();
}

QVariantList NewsFeedProxyModel::getActors(const QVariantMap &object)
{
    return object.value("actors").toList();
}

QSet<QString> NewsFeedProxyModel::getNegativeFeedback(const QVariantMap &object)
{
    QVariantList entries = object.value("negativeFeedback").toList();
    QSet<QString> result;
    for (const QVariant &entry : entries) {
        QString type = entry.toMap().value("type").toString();
        if (!type.isEmpty()) {
            result.insert(type);
        }
    }
    return result;
}
