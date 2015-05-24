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
    : QAbstractListModel(parent), m_model(0)
{
}

QHash<int, QByteArray> FriendsProxyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ObjectRole, "object");
    roles.insert(SectionRole, "section");
    return roles;
}

int FriendsProxyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant FriendsProxyModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= rowCount()) {
        return QVariant();
    }

    switch (role) {
    case ObjectRole:
        return QVariant::fromValue(m_data.value(row));
        break;
    case SectionRole:
    {
        QObject *object = m_data.value(row);
        return getFriendName(object);
        break;
    }
    default:
        return QVariant();
        break;
    }
}

void FriendsProxyModel::classBegin()
{
}

void FriendsProxyModel::componentComplete()
{
}

int FriendsProxyModel::count() const
{
    return rowCount();
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
        loadData();
    }
}

SocialContentModel * FriendsProxyModel::model() const
{
    return m_model;
}

void FriendsProxyModel::setModel(SocialContentModel *model)
{
    if (m_model != model) {
        if (m_model) {
            disconnect(m_model, &SocialContentModel::finished, this, &FriendsProxyModel::slotFinished);
        }
        m_model = model;
        if (m_model) {
            connect(m_model, &SocialContentModel::finished, this, &FriendsProxyModel::slotFinished);
        }
        emit modelChanged();
    }
}

SocialNetworkStatus::type FriendsProxyModel::status() const
{
    return m_status;
}

SocialNetworkError::type FriendsProxyModel::error() const
{
    return m_error;
}

QString FriendsProxyModel::errorMessage() const
{
    return m_errorMessage;
}

QString FriendsProxyModel::errorCode() const
{
    return m_errorCode;
}

void FriendsProxyModel::load()
{
    if (!m_model) {
        return;
    }

    if (m_model->status() == SocialNetworkStatus::Busy) {
        return;
    }

    clear();
    m_model->load();
    setStatus();
}

QString FriendsProxyModel::getFriendName(const QObject *object)
{
    if (!object) {
        return QString();
    }
    const QMetaObject *meta = object->metaObject();
    int propertyIndex = meta->indexOfProperty("name");
    if (propertyIndex == -1) {
        return QString();
    }
    const QMetaProperty &property = meta->property(propertyIndex);
    return property.read(object).toString();
}

QString FriendsProxyModel::normalizeString(const QString &string)
{
    QString returned = string.trimmed().normalized (QString::NormalizationForm_KD).toLower();
    returned.remove(QRegularExpression("[^a-z\\s]"));
    return returned;
}

bool FriendsProxyModel::sortObjects(const QObject *left, const QObject *right)
{
    return getFriendName(left) < getFriendName(right);
}

void FriendsProxyModel::setStatus()
{
    if (m_status != m_model->status()) {
        m_status = m_model->status();
        emit statusChanged();
    }
}

void FriendsProxyModel::setProperties(bool ok)
{
    setStatus();
    if (m_error != m_model->error()) {
        m_error = m_model->error();
        emit errorChanged();
    }

    if (m_errorMessage != m_model->errorMessage()) {
        m_errorMessage = m_model->errorMessage();
        emit errorMessageChanged();
    }

    if (m_errorCode != m_model->errorCode()) {
        m_errorCode = m_model->errorCode();
        emit errorCodeChanged();
    }

    emit finished(ok);
}

void FriendsProxyModel::clear()
{
    if (!m_data.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount());
        m_data.clear();
        emit countChanged();
        endRemoveRows();
    }
}

void FriendsProxyModel::loadData()
{
    clear();
    QList<QObject *> data;
    QString filter = normalizeString(m_filter);

    int count = m_model->count();
    for (int i = 0; i < count; ++i) {
        const QVariant &variant = m_model->data(m_model->index(i), SocialContentModel::ObjectRole);
        QObject *object = variant.value<QObject *>();
        QString name = normalizeString(getFriendName(object));
        if (name.contains(filter)) {
            data.append(object);
        }
    }

    if (!data.isEmpty()) {
        int count = data.count();
        beginInsertRows(QModelIndex(), 0, count - 1);
        std::sort(data.begin(), data.end(), sortObjects);
        m_data = data;
        emit countChanged();
        endInsertRows();
    }
}

void FriendsProxyModel::slotFinished(bool ok)
{
    if (!ok) {
        setProperties(ok);
        return;
    }

    if (m_model->hasNext()) {
        m_model->loadNext();
        return;
    }

    loadData();
    setProperties(ok);
}
