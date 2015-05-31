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

#include "friendsproxymodel.h"
#include <QtCore/QMetaProperty>
#include <QtCore/QRegularExpression>

FriendsProxyModel::FriendsProxyModel(QObject *parent)
    : AbstractProxyModel(parent)
{
}

QString FriendsProxyModel::filter() const
{
    return m_filter;
}

void FriendsProxyModel::setFilter(const QString &filter)
{
    if (m_filter != filter) {
        m_filter = filter;
        emit filterChanged();
        reload();
    }
}

bool FriendsProxyModel::isAutoLoad() const
{
    return true;
}

QList<QVariantMap> FriendsProxyModel::filterData(const QList<QVariantMap> &input)
{
    QList<QVariantMap> data;
    QString filter = normalizeString(m_filter);

    for (const QVariantMap &object : input) {
        QString name = normalizeString(getName(object));
        if (name.contains(filter)) {
            data.append(object);
        }
    }

    std::sort(data.begin(), data.end(), sortObjects);
    return data;
}

QString FriendsProxyModel::section(const QVariantMap &object) const
{
    return getName(object);
}

QString FriendsProxyModel::getName(const QVariantMap &object)
{
    return object.value("name").toString();
}

QString FriendsProxyModel::normalizeString(const QString &string)
{
    QString returned = string.trimmed().normalized (QString::NormalizationForm_KD).toLower();
    returned.remove(QRegularExpression("[^a-z\\s]"));
    return returned;
}

bool FriendsProxyModel::sortObjects(const QVariantMap &left, const QVariantMap &right)
{
    return getName(left) < getName(right);
}
