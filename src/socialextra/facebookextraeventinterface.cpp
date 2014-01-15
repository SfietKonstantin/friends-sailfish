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

#include "facebookextraeventinterface.h"
#include "facebook/facebookeventinterface_p.h"
#include <QtCore/QDebug>

static const char *COVER_KEY = "pic_cover";
static const char *RSVP_STATUS_KEY = "rsvp_status";
static const char *ALL_MEMBERS_COUNT_KEY = "all_members_count";
static const char *NOT_REPLIED_COUNT_KEY = "not_replied_count";
static const char *ATTENDING_COUNT_KEY = "attending_count";
static const char *UNSURE_COUNT_KEY = "unsure_count";
static const char *DECLINED_COUNT_KEY = "declined_count";

class FacebookExtraEventInterfacePrivate: public FacebookEventInterfacePrivate
{
public:
    explicit FacebookExtraEventInterfacePrivate(FacebookExtraEventInterface *q);
    void emitPropertyChangeSignals(const QVariantMap &oldData, const QVariantMap &newData);
    FacebookCoverInterface *cover;
    FacebookExtraEventInterface::RsvpStatus rsvpStatus;
private:
    Q_DECLARE_PUBLIC(FacebookExtraEventInterface)
};

FacebookExtraEventInterfacePrivate::FacebookExtraEventInterfacePrivate(FacebookExtraEventInterface *q)
    : FacebookEventInterfacePrivate(q)
    , cover(0)
    , rsvpStatus(FacebookExtraEventInterface::NotReplied)
{
}

void FacebookExtraEventInterfacePrivate::emitPropertyChangeSignals(const QVariantMap &oldData,
                                                                   const QVariantMap &newData)
{
    Q_Q(FacebookExtraEventInterface);
    QVariant oldRsvpStatus = oldData.value(RSVP_STATUS_KEY);
    QVariant newRsvpStatus = newData.value(RSVP_STATUS_KEY);
    QVariant oldAllMembersCount = oldData.value(ALL_MEMBERS_COUNT_KEY);
    QVariant newAllMembersCount = newData.value(ALL_MEMBERS_COUNT_KEY);
    QVariant oldNotRepliedCount = oldData.value(NOT_REPLIED_COUNT_KEY);
    QVariant newNotRepliedCount = newData.value(NOT_REPLIED_COUNT_KEY);
    QVariant oldAttendingCount = oldData.value(ATTENDING_COUNT_KEY);
    QVariant newAttendingCount = newData.value(ATTENDING_COUNT_KEY);
    QVariant oldUnsureCount = oldData.value(UNSURE_COUNT_KEY);
    QVariant newUnsureCount = newData.value(UNSURE_COUNT_KEY);
    QVariant oldDeclinedCount = oldData.value(DECLINED_COUNT_KEY);
    QVariant newDeclinedCount = newData.value(DECLINED_COUNT_KEY);

    if (oldAllMembersCount != newAllMembersCount)
        emit q->allMembersCountChanged();
    if (oldNotRepliedCount != newNotRepliedCount)
        emit q->notRepliedCountChanged();
    if (oldAttendingCount != newAttendingCount)
        emit q->attendingCountChanged();
    if (oldUnsureCount != newUnsureCount)
        emit q->unsureCountChanged();
    if (oldDeclinedCount != newDeclinedCount)
        emit q->declinedCountChanged();

    // Manage cover
    QVariantMap oldCoverMap = oldData.value(COVER_KEY).toMap();
    QVariantMap newCoverMap = newData.value(COVER_KEY).toMap();

    if (newCoverMap != oldCoverMap) {
        cover->setData(newCoverMap);
        emit q->coverChanged();
    }

    // Manage RSVP status
    if (oldRsvpStatus != newRsvpStatus) {
        QString rsvpStatusString = newRsvpStatus.toString();
        if (rsvpStatusString == "attending") {
            rsvpStatus = FacebookExtraEventInterface::Attending;
        } else if (rsvpStatusString == "unsure") {
            rsvpStatus = FacebookExtraEventInterface::Unsure;
        } else if (rsvpStatusString == "declined") {
            rsvpStatus = FacebookExtraEventInterface::Declined;
        } else {
            rsvpStatus = FacebookExtraEventInterface::NotReplied;
        }
        emit q->rsvpStatusChanged();
    }

    // Call super class implementation
    FacebookEventInterfacePrivate::emitPropertyChangeSignals(oldData, newData);
}

FacebookExtraEventInterface::FacebookExtraEventInterface(QObject *parent) :
    FacebookEventInterface(*(new FacebookExtraEventInterfacePrivate(this)), parent)
{
    Q_D(FacebookExtraEventInterface);
    d->cover = new FacebookCoverInterface(this);
}

FacebookCoverInterface * FacebookExtraEventInterface::cover() const
{
    Q_D(const FacebookExtraEventInterface);
    return d->cover;
}

FacebookExtraEventInterface::RsvpStatus FacebookExtraEventInterface::rsvpStatus() const
{
    Q_D(const FacebookExtraEventInterface);
    return d->rsvpStatus;
}

int FacebookExtraEventInterface::allMembersCount() const
{
    QString numberString = data().value(ALL_MEMBERS_COUNT_KEY).toString();
    bool ok;
    int number = numberString.toInt(&ok);
    if (ok) {
        return number;
    }
    return -1;
}

int FacebookExtraEventInterface::notRepliedCount() const
{
    QString numberString = data().value(NOT_REPLIED_COUNT_KEY).toString();
    bool ok;
    int number = numberString.toInt(&ok);
    if (ok) {
        return number;
    }
    return -1;
}

int FacebookExtraEventInterface::attendingCount() const
{
    QString numberString = data().value(ATTENDING_COUNT_KEY).toString();
    bool ok;
    int number = numberString.toInt(&ok);
    if (ok) {
        return number;
    }
    return -1;
}

int FacebookExtraEventInterface::unsureCount() const
{
    QString numberString = data().value(UNSURE_COUNT_KEY).toString();
    bool ok;
    int number = numberString.toInt(&ok);
    if (ok) {
        return number;
    }
    return -1;
}

int FacebookExtraEventInterface::declinedCount() const
{
    QString numberString = data().value(DECLINED_COUNT_KEY).toString();
    bool ok;
    int number = numberString.toInt(&ok);
    if (ok) {
        return number;
    }
    return -1;
}
