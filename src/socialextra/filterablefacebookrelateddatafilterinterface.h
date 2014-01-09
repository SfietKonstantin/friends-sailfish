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

#ifndef FILTERABLEFACEBOOKRELATEDDATAFILTERINTERFACE_H
#define FILTERABLEFACEBOOKRELATEDDATAFILTERINTERFACE_H

#include "facebook/facebookrelateddatafilterinterface.h"

// Note that this filter should be used on only one SNMI
// It can be buggy otherwise
class ContentItemInterface;
class FilterableFacebookRelatedDataFilterInterfacePrivate;
class FilterableFacebookRelatedDataFilterInterface: public FacebookRelatedDataFilterInterface
{
    Q_OBJECT
    Q_ENUMS(FilterType)
    Q_PROPERTY(FilterType filterType READ filterType WRITE setFilterType NOTIFY filterTypeChanged)
    Q_PROPERTY(QString filterField READ filterField WRITE setFilterField NOTIFY filterFieldChanged)
    Q_PROPERTY(QString filterValue READ filterValue WRITE setFilterValue NOTIFY filterValueChanged)
public:
    enum FilterType {
        Any,
        Beginning,
        End
    };
    explicit FilterableFacebookRelatedDataFilterInterface(QObject *parent = 0);
    FilterType filterType() const;
    void setFilterType(FilterType filterType);
    QString filterField() const;
    void setFilterField(const QString &filterField);
    QString filterValue() const;
    void setFilterValue(const QString &filterValue);
signals:
    void filterTypeChanged();
    void filterFieldChanged();
    void filterValueChanged();
protected:
    bool performSetModelDataImpl(SocialNetworkModelInterface *model,
                                 SocialNetworkInterface *socialNetwork, const QByteArray &data,
                                 LoadType loadType, const QVariantMap &properties);
    bool isDataAcceptable(const QVariantMap &data);
private:
    bool checkIsAcceptable(const QVariantMap &data);
    void refilter();
    FilterType m_filterType;
    QString m_filterField;
    QString m_filterValue;
    SocialNetworkModelInterface *m_lastModel;
    SocialNetworkInterface *m_lastSocialNetwork;
    FacebookInterface::ContentItemType m_lastContentType;
    QList<QVariantMap> m_lastModelData;
};

#endif // FILTERABLEFACEBOOKRELATEDDATAFILTERINTERFACE_H
