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

#include "changelogmodel.h"
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>
#include <QtCore/QDebug>

bool versionGreater(const QString &version1, const QString &version2)
{
    QStringList firstVersionList = version1.split(".", QString::SkipEmptyParts);
    QStringList secondVersionList = version2.split(".", QString::SkipEmptyParts);

    if (firstVersionList.count() != 3 || secondVersionList.count() != 3) {
        return false;
    }

    int first1 = firstVersionList.at(0).toInt();
    int first2 = firstVersionList.at(0).toInt();
    int first3 = firstVersionList.at(0).toInt();

    int second1 = secondVersionList.at(0).toInt();
    int second2 = secondVersionList.at(0).toInt();
    int second3 = secondVersionList.at(0).toInt();

    if (first1 != second1) {
        return first1 > second1;
    }

    if (first2 != second2) {
        return first2 > second2;
    }

    return first3 >= second3;

}

struct ChangeLogModelData
{
    QString version;
    QString type;
    QString text;
};

ChangeLogModel::ChangeLogModel(QObject *parent) :
    QAbstractListModel(parent), m_onlyCurrent(false)
{
    load();
}

ChangeLogModel::~ChangeLogModel()
{
    qDeleteAll(m_data);
}

QHash<int, QByteArray> ChangeLogModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(VersionRole, "version");
    roles.insert(TypeRole, "type");
    roles.insert(TextRole, "text");
    return roles;
}

int ChangeLogModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return m_data.count();
}

QVariant ChangeLogModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= rowCount()) {
        return QVariant();
    }

    const ChangeLogModelData *data = m_data.at(row);

    switch (role) {
    case VersionRole:
        return data->version;
        break;
    case TypeRole:
        return data->type;
        break;
    case TextRole:
        return data->text;
        break;
    default:
        return QVariant();
    }
}

int ChangeLogModel::count() const
{
    return rowCount();
}

bool ChangeLogModel::isOnlyCurrent() const
{
    return m_onlyCurrent;
}

void ChangeLogModel::setOnlyCurrent(bool onlyCurrent)
{
    if (m_onlyCurrent != onlyCurrent) {
        m_onlyCurrent = onlyCurrent;
        emit onlyCurrentChanged();
        load();
    }
}

void ChangeLogModel::load()
{
    if (!m_data.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        qDeleteAll(m_data);
        m_data.clear();
    }
#ifndef DESKTOP
    QString fileName = QString("%1/data/changelog.json").arg(DEPLOYMENT_PATH);
#else
    QString fileName = QString(":/data/changelog.json");
#endif
    QFile file (fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject object = document.object();
    if (m_onlyCurrent) {
        addVersion(VERSION, object.value(VERSION).toArray());
    } else {
        QStringList keys = object.keys();
        std::sort(keys.begin(), keys.end(), versionGreater);
        foreach (const QString &key, keys) {
            addVersion(key, object.value(key).toArray());
        }
    }
}

void ChangeLogModel::addVersion(const QString &version, const QJsonArray &data)
{
    foreach (const QJsonValue &value, data) {
        QJsonObject entry = value.toObject();
        ChangeLogModelData *data = new ChangeLogModelData;
        data->version = version;
        data->type = entry.value("type").toString();
        data->text = entry.value("text").toString();
        m_data.append(data);
    }
}
