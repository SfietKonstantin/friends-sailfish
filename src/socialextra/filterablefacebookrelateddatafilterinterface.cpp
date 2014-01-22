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

#include "filterablefacebookrelateddatafilterinterface.h"
#include "facebook/facebookinterface_p.h"
#include "socialnetworkmodelinterface.h"

static const char *CONTENTTYPE_KEY = "content_type";

FilterableFacebookRelatedDataFilterInterface::FilterableFacebookRelatedDataFilterInterface(QObject *parent)
    : FacebookRelatedDataFilterInterface(parent), m_filterType(Any), m_lastModel(0)
    , m_lastSocialNetwork(0), m_lastContentType(FacebookInterface::NotInitialized)
{
}

FilterableFacebookRelatedDataFilterInterface::FilterType FilterableFacebookRelatedDataFilterInterface::filterType() const
{
    return m_filterType;
}

void FilterableFacebookRelatedDataFilterInterface::setFilterType(FilterType filterType)
{
    if (m_filterType != filterType) {
        m_filterType = filterType;
        emit filterTypeChanged();
        refilter();
    }
}

QString FilterableFacebookRelatedDataFilterInterface::filterField() const
{
    return m_filterField;
}
void FilterableFacebookRelatedDataFilterInterface::setFilterField(const QString &filterField)
{
    if (m_filterField != filterField) {
        m_filterField = filterField;
        emit filterFieldChanged();
        refilter();
    }
}

QString FilterableFacebookRelatedDataFilterInterface::filterValue() const
{
    return m_filterValue;
}
void FilterableFacebookRelatedDataFilterInterface::setFilterValue(const QString &filterValue)
{
    if (m_filterValue != filterValue) {
        m_filterValue = filterValue;
        emit filterValueChanged();
        refilter();
    }
}

QString FilterableFacebookRelatedDataFilterInterface::dataSection(const QVariantMap &data)
{
    if (sectionField().isEmpty()) {
        return QString();
    }

    QString section = data.value(sectionField()).toString();

    if (sectionField() == "rsvp_status") {
        QString trueSection;


        if (section == "attending") {
            //: A section header for people attending an event
            //% "Attending"
            trueSection = qtTrId("facebook_event_attending");
        } else if (section == "unsure") {
            //: A section header for people that are unsure about attending to an event
            //% "Unsure"
            trueSection = qtTrId("facebook_event_unsure");
        } else if (section == "declined") {
            //: A section header for people who declined an event
            //% "Declined"
            trueSection = qtTrId("facebook_event_declined");
        } else if (section == "not_replied") {
            //: A section header for people who did not replied yet
            //% "Not replied"
            trueSection = qtTrId("facebook_event_not_replied");
        }

        return trueSection;
    }

    return section.left(1);
}

bool FilterableFacebookRelatedDataFilterInterface::performSetModelDataImpl(SocialNetworkModelInterface *model,
                                                                           SocialNetworkInterface *socialNetwork,
                                                                           const QByteArray &data,
                                                                           LoadType loadType,
                                                                           const QVariantMap &properties)
{
    m_lastModel = model;
    m_lastSocialNetwork = socialNetwork;
    m_lastContentType = static_cast<FacebookInterface::ContentItemType>(properties.value(CONTENTTYPE_KEY).toInt());
    if (loadType == FilterInterface::Load) {
        m_lastModelData.clear();
    }

    return FacebookRelatedDataFilterInterface::performSetModelDataImpl(model, socialNetwork,
                                                                       data, loadType, properties);
}

bool FilterableFacebookRelatedDataFilterInterface::isDataAcceptable(const QVariantMap &data)
{
    if (m_lastModel && m_lastSocialNetwork && m_lastContentType != FacebookInterface::NotInitialized) {
        m_lastModelData.append(data);
    }
    return checkIsAcceptable(data);
}

bool FilterableFacebookRelatedDataFilterInterface::checkIsAcceptable(const QVariantMap &data)
{
    QString field = m_filterField.trimmed();
    QString value = m_filterValue.trimmed().toLower();
    if (field.isEmpty() || value.trimmed().isEmpty()) {
        return true;
    }

    QString unfilteredValue = data.value(field).toString().toLower();
    switch (m_filterType) {
    case Beginning:
        return unfilteredValue.startsWith(value);
        break;
    case End:
        return unfilteredValue.endsWith(value);
        break;
    default:
        return unfilteredValue.contains(value);
        break;
    }
}

void FilterableFacebookRelatedDataFilterInterface::refilter()
{

    if (!m_lastModel || !m_lastSocialNetwork || m_lastContentType == FacebookInterface::NotInitialized) {
        return;
    }

    QList<ContentItemInterface *> filteredData;
    foreach (QVariantMap data, m_lastModelData) {
        if (checkIsAcceptable(data)) {
            filteredData.append(FacebookInterfacePrivate::createItem(m_lastContentType, data,
                                                                     m_lastSocialNetwork, m_lastModel));
        }
    }

    m_lastModel->setModelData(filteredData);
}
