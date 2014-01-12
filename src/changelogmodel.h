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

#ifndef CHANGELOGMODEL_H
#define CHANGELOGMODEL_H

#include <QtCore/QAbstractListModel>

class QJsonArray;
struct ChangeLogModelData;
class ChangeLogModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool all READ isAll WRITE setAll NOTIFY allChanged)
    Q_PROPERTY(QString fromVersion READ fromVersion WRITE setFromVersion NOTIFY fromVersionChanged)
public:
    enum Roles {
        VersionRole,
        TypeRole,
        TextRole
    };
public:
    explicit ChangeLogModel(QObject *parent = 0);
    virtual ~ChangeLogModel();

    QHash<int, QByteArray> roleNames() const;

    int rowCount(const QModelIndex &index = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

    int count() const;
    bool isAll() const;
    void setAll(bool all);
    QString fromVersion() const;
    void setFromVersion(const QString &fromVersion);
Q_SIGNALS:
    void countChanged();
    void allChanged();
    void fromVersionChanged();
private:
    void load();
    void addVersion(const QString &version, const QJsonArray &data);
    void addVersionAndType(const QString &version, const QString &type, const QJsonArray &data);
    bool m_all;
    QString m_fromVersion;
    QList<ChangeLogModelData *> m_data;
};

#endif // CHANGELOGMODEL_H
