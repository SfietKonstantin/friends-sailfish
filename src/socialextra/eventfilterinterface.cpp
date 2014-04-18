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

#include "eventfilterinterface.h"
#include "filterinterface_p.h"
#include "facebook/facebookinterface.h"
#include "socialnetworkmodelinterface.h"
#include "facebook/facebookontology_p.h"
#include "identifiablecontentiteminterface_p.h"
#include "facebookextraeventinterface.h"

static const char *QUERY = "{"
    "\"query1\": \"SELECT eid, rsvp_status FROM event_member WHERE %1\", "\
    "\"query2\": \"SELECT eid, name, description, pic_cover, location, privacy, update_time, "\
                  "start_time, end_time, timezone, host, creator FROM event WHERE "\
                  "eid IN (SELECT eid FROM #query1)\"}";

static const char *QUERY_QUERY1 = "query1";
static const char *QUERY_QUERY2 = "query2";
static const char *QUERY_NAME_KEY = "name";
static const char *QUERY_RESULT_KEY = "fql_result_set";
static const char *QUERY_ALL = "uid = me()";
static const char *QUERY_SPECIFIC = "eid = %1 AND uid = me()";

class EventFilterInterfacePrivate: public FilterInterfacePrivate
{
public:
    explicit EventFilterInterfacePrivate(EventFilterInterface *q);
    static inline void preprocessData(const QVariantMap &parsed,
                                      QMap<QString, QString> &rsvpStatusMap,
                                      QVariantList &eventList)
    {
        QVariantList dataList = parsed.value(QLatin1String("data")).toList();
        foreach (const QVariant &data, dataList) {
            QVariantMap entry = data.toMap();
            if (entry.value(QUERY_NAME_KEY).toString() == QUERY_QUERY2) {
                eventList = entry.value(QUERY_RESULT_KEY).toList();
            }

            if (entry.value(QUERY_NAME_KEY).toString() == QUERY_QUERY1) {
                QVariantList attendingList = entry.value(QUERY_RESULT_KEY).toList();
                foreach (const QVariant &attending, attendingList) {
                    QVariantMap attendingEntry = attending.toMap();
                    // Casted from double without loss
                    QString eventId = QString::number(attendingEntry.value("eid").toULongLong());
                    rsvpStatusMap.insert(eventId, attendingEntry.value("rsvp_status").toString());
                }
            }
        }
    }
    static inline QVariantMap createEventData(const QVariantMap &data,
                                              const QMap<QString, QString> &rsvpStatusMap)
    {
        // Casted from double without loss
        QString eventId = QString::number(data.value("eid").toULongLong());

        QVariantMap eventData;
        eventData.insert(NEMOQMLPLUGINS_SOCIAL_CONTENTITEMID, eventId);
        eventData.insert(FACEBOOK_ONTOLOGY_METADATA_ID, eventId);

        // Owner
        QVariantMap ownerData;
        QString ownerId = QString::number(data.value("creator").toULongLong());
        QString fromName = data.value("host").toString();
        ownerData.insert(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTIDENTIFIER, ownerId);
        ownerData.insert(FACEBOOK_ONTOLOGY_OBJECTREFERENCE_OBJECTNAME, fromName);
        eventData.insert(FACEBOOK_ONTOLOGY_EVENT_OWNER, ownerData);

        // Name & description
        eventData.insert(FACEBOOK_ONTOLOGY_EVENT_NAME, data.value("name").toString());
        eventData.insert(FACEBOOK_ONTOLOGY_EVENT_DESCRIPTION, data.value("description").toString());

        // Location
        eventData.insert(FACEBOOK_ONTOLOGY_EVENT_LOCATION, data.value("location").toString());

        // Privacy
        eventData.insert(FACEBOOK_ONTOLOGY_EVENT_PRIVACY, data.value("privacy").toString());

        // Start / End time
        eventData.insert(FACEBOOK_ONTOLOGY_EVENT_STARTTIME, data.value("start_time").toString());
        eventData.insert(FACEBOOK_ONTOLOGY_EVENT_ENDTIME, data.value("end_time").toString());

        // Updated time
        eventData.insert(FACEBOOK_ONTOLOGY_EVENT_UPDATEDTIME, data.value("update_time").toString());

        // Cover
        eventData.insert("pic_cover", data.value("pic_cover").toMap());

        // RSVP status
        eventData.insert("rsvp_status", rsvpStatusMap.value(eventId));
        return eventData;
    }
    QString identifier;
};

EventFilterInterfacePrivate::EventFilterInterfacePrivate(EventFilterInterface *q)
    : FilterInterfacePrivate(q)
{
}

EventFilterInterface::EventFilterInterface(QObject *parent) :
    FilterInterface(*(new EventFilterInterfacePrivate(this)), parent)
{
}

QString EventFilterInterface::identifier() const
{
    Q_D(const EventFilterInterface);
    return d->identifier;
}

void EventFilterInterface::setIdentifier(const QString &identifier)
{
    Q_D(EventFilterInterface);
    if (d->identifier != identifier) {
        d->identifier = identifier;
        emit identifierChanged();
    }
}

bool EventFilterInterface::isAcceptable(QObject *item, SocialNetworkInterface *socialNetwork) const
{
    if (!testType<FacebookInterface>(socialNetwork)) {
        return false;
    }

    // This filter only works for models or FacebookExtraEvent
    if (!testType<SocialNetworkModelInterface>(item) && !testType<FacebookExtraEventInterface>(item)) {
        return false;
    }

    return true;
}

bool EventFilterInterface::performLoadRequestImpl(QObject *item,
                                                  SocialNetworkInterface *socialNetwork,
                                                  LoadType loadType)
{
    Q_D(EventFilterInterface);
    FacebookInterface *facebook = qobject_cast<FacebookInterface *>(socialNetwork);
    if (!facebook) {
        return false;
    }

    QMap<QString, QString> arguments;
    QString query (QUERY);
    if (d->identifier.isEmpty()) {
        query = query.arg(QUERY_ALL);
    } else {
        query = query.arg(QString(QUERY_SPECIFIC).arg(d->identifier));
    }

    switch (loadType) {
    case FilterInterface::Load:
        break;
    default:
        // There is no next and no previous
        return false;
    }
    arguments.insert("q", query);

    QObject *handle = facebook->get(this, QLatin1String("fql"), QString(), QString(), arguments);
    return d->addHandle(handle, item, socialNetwork, loadType);
}

bool EventFilterInterface::performSetModelDataImpl(SocialNetworkModelInterface *model,
                                                   SocialNetworkInterface *socialNetwork,
                                                   const QByteArray &data, LoadType loadType,
                                                   const QVariantMap &properties)
{
    Q_UNUSED(properties)
    Q_UNUSED(loadType)
    bool ok = false;
    QVariantMap parsed = IdentifiableContentItemInterfacePrivate::parseReplyData(data, &ok);
    if (!ok) {
        QString errorMessage = QString(QLatin1String("Unable to parse downloaded data. "\
                                                     "Downloaded data: %1")).arg(QString(data));
        model->setError(SocialNetworkInterface::DataError, errorMessage);
        return false;
    }

    QVariantList dataList = parsed.value(QLatin1String("data")).toList();
    QVariantList eventList;
    QMap<QString, QString> rsvpStatusMap;
    EventFilterInterfacePrivate::preprocessData(parsed, rsvpStatusMap, eventList);
    QList<ContentItemInterface *> modelData;

    foreach (const QVariant &eventObject, eventList) {
        QVariantMap eventMap = eventObject.toMap();
        FacebookExtraEventInterface *event = new FacebookExtraEventInterface(model);
        event ->setSocialNetwork(socialNetwork);
        event ->setData(EventFilterInterfacePrivate::createEventData(eventMap, rsvpStatusMap));
        event ->classBegin();
        event ->componentComplete();
        modelData.append(event );
    }

    model->setModelData(modelData);
    model->setPagination(false, false);

    return true;
}

bool EventFilterInterface::performSetItemDataImpl(IdentifiableContentItemInterface *item,
                                                  SocialNetworkInterface *socialNetwork,
                                                  const QByteArray &data, LoadType loadType,
                                                  const QVariantMap &properties)
{
    Q_UNUSED(socialNetwork)
    Q_UNUSED(loadType)
    Q_UNUSED(properties)
    bool ok = false;
    QVariantMap parsed = IdentifiableContentItemInterfacePrivate::parseReplyData(data, &ok);
    if (!ok) {
        QString errorMessage = QString(QLatin1String("Unable to parse downloaded data. "\
                                                     "Downloaded data: %1")).arg(QString(data));
        item->setError(SocialNetworkInterface::DataError, errorMessage);
        return false;
    }

    QVariantList eventList;
    QMap<QString, QString> rsvpStatusMap;
    EventFilterInterfacePrivate::preprocessData(parsed, rsvpStatusMap, eventList);

    if (eventList.count() != 1) {
        QString errorMessage = QString(QLatin1String("Invalid data in downloaded data. "\
                                                     "Downloaded data: %1")).arg(QString(data));
        item->setError(SocialNetworkInterface::DataError, errorMessage);
        return false;
    }

    item->setData(EventFilterInterfacePrivate::createEventData(eventList.first().toMap(), rsvpStatusMap));
    return true;
}
