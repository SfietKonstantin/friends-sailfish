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

#include "abstractproxymodel.h"
#include <QtCore/QSet>

AbstractProxyModel::AbstractProxyModel(QObject *parent)
    : QAbstractListModel(parent), m_model(0), m_next(-1)
{
}

void AbstractProxyModel::reload()
{
    clear();
    filterAndAppend();
}

QHash<int, QByteArray> AbstractProxyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ObjectRole, "object");
    roles.insert(SectionRole, "section");
    return roles;
}

int AbstractProxyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant AbstractProxyModel::data(const QModelIndex &index, int role) const
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
        const QVariantMap &object = m_data.value(row);
        return section(object);
        break;
    }
    default:
        return QVariant();
        break;
    }
}

void AbstractProxyModel::classBegin()
{
}

void AbstractProxyModel::componentComplete()
{
}

int AbstractProxyModel::count() const
{
    return rowCount();
}

SocialContentModel * AbstractProxyModel::model() const
{
    return m_model;
}

void AbstractProxyModel::setModel(SocialContentModel *model)
{
    if (m_model != model) {
        if (m_model) {
            disconnect(m_model, &SocialContentModel::finished, this, &AbstractProxyModel::slotFinished);
        }
        m_model = model;
        if (m_model) {
            connect(m_model, &SocialContentModel::finished, this, &AbstractProxyModel::slotFinished);
        }
        emit modelChanged();
    }
}

SocialNetworkStatus::type AbstractProxyModel::status() const
{
    return m_status;
}

SocialNetworkError::type AbstractProxyModel::error() const
{
    return m_error;
}

QString AbstractProxyModel::errorMessage() const
{
    return m_errorMessage;
}

QString AbstractProxyModel::errorCode() const
{
    return m_errorCode;
}

void AbstractProxyModel::load()
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

void AbstractProxyModel::loadNext()
{
    if (!m_model) {
        return;
    }

    if (m_model->status() == SocialNetworkStatus::Busy) {
        return;
    }

    m_model->loadNext();
    setStatus();
}

void AbstractProxyModel::setStatus()
{
    if (m_status != m_model->status()) {
        m_status = m_model->status();
        emit statusChanged();
    }
}

void AbstractProxyModel::setProperties(bool ok)
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

void AbstractProxyModel::clear()
{
    m_next = 0;
    if (!m_data.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, rowCount());
        m_data.clear();
        emit countChanged();
        endRemoveRows();
    }
}

void AbstractProxyModel::filterAndAppend()
{
    QList<QVariantMap> input;
    int count = m_model->count();
    for (int i = m_next; i < count; ++i) {
        const QVariant &variant = m_model->data(m_model->index(i), SocialContentModel::ObjectRole);
        const QVariantMap &object = variant.toMap();
        input.append(object);
    }
    const QList<QVariantMap> &data = filterData(input);
    if (!data.isEmpty()) {
        int count = data.count();
        beginInsertRows(QModelIndex(), rowCount(), rowCount() + count - 1);
        m_data.append(data);
        emit countChanged();
        endInsertRows();
    }
    m_next = count;
}

void AbstractProxyModel::slotFinished(bool ok)
{
    if (!ok) {
        setProperties(ok);
        return;
    }

    if (isAutoLoad()) {
        if (m_model->hasNext()) {
            m_model->loadNext();
            return;
        }
    }

    filterAndAppend();
    setProperties(ok);
}
