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

#ifndef ABSTRACTPROXYMODEL_H
#define ABSTRACTPROXYMODEL_H

#include <QtCore/QAbstractListModel>
#include <socialcontentmodel.h>

class AbstractProxyModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(SocialContentModel * model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(SocialNetworkStatus::type status READ status NOTIFY statusChanged)
    Q_PROPERTY(SocialNetworkError::type error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(QString errorCode READ errorCode NOTIFY errorCodeChanged)
public:
    enum Roles {
        ObjectRole = Qt::UserRole + 1,
        SectionRole
    };
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    void classBegin() Q_DECL_OVERRIDE;
    void componentComplete() Q_DECL_OVERRIDE;
    int count() const;
    SocialContentModel * model() const;
    void setModel(SocialContentModel *model);
    SocialNetworkStatus::type status() const;
    SocialNetworkError::type error() const;
    QString errorMessage() const;
    QString errorCode() const;
public slots:
    void load();
    void loadNext();
signals:
    void countChanged();
    void modelChanged();
    void statusChanged();
    void errorChanged();
    void errorMessageChanged();
    void errorCodeChanged();
    void finished(bool ok);
protected:
    explicit AbstractProxyModel(QObject *parent = 0);
    void reload();
    virtual bool isAutoLoad() const = 0;
    virtual QList<QVariantMap> filterData(const QList<QVariantMap> &input) = 0;
    virtual QString section(const QVariantMap &object) const = 0;
private:
    void setStatus();
    void setProperties(bool ok);
    void clear();
    void filterAndAppend();
    SocialContentModel *m_model;
    QList<QVariantMap> m_data;
    int m_next;
    SocialNetworkStatus::type m_status;
    SocialNetworkError::type m_error;
    QString m_errorMessage;
    QString m_errorCode;
private slots:
    void slotFinished(bool ok);
};

#endif // ABSTRACTPROXYMODEL_H
