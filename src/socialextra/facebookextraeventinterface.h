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

#ifndef FACEBOOKEXTRAEVENTINTERFACE_H
#define FACEBOOKEXTRAEVENTINTERFACE_H

#include "facebook/facebookeventinterface.h"
#include "facebook/facebookcoverinterface.h"

class FacebookExtraEventInterfacePrivate;
class FacebookExtraEventInterface : public FacebookEventInterface
{
    Q_OBJECT
    Q_ENUMS(RsvpStatus)
    Q_PROPERTY(FacebookCoverInterface * cover READ cover NOTIFY coverChanged)
    Q_PROPERTY(RsvpStatus rsvpStatus READ rsvpStatus NOTIFY rsvpStatusChanged)
    Q_PROPERTY(int allMembersCount READ allMembersCount NOTIFY allMembersCountChanged)
    Q_PROPERTY(int notRepliedCount READ notRepliedCount NOTIFY notRepliedCountChanged)
    Q_PROPERTY(int attendingCount READ attendingCount NOTIFY attendingCountChanged)
    Q_PROPERTY(int unsureCount READ unsureCount NOTIFY unsureCountChanged)
    Q_PROPERTY(int declinedCount READ declinedCount NOTIFY declinedCountChanged)
public:
    enum RsvpStatus {
        InvalidRsvpStatus,
        NotReplied,
        Attending,
        Unsure,
        Declined
    };
    explicit FacebookExtraEventInterface(QObject *parent = 0);

    // Accessors
    FacebookCoverInterface * cover() const;
    RsvpStatus rsvpStatus() const;
    int allMembersCount() const;
    int notRepliedCount() const;
    int attendingCount() const;
    int unsureCount() const;
    int declinedCount() const;
signals:
    void coverChanged();
    void rsvpStatusChanged();
    void allMembersCountChanged();
    void notRepliedCountChanged();
    void attendingCountChanged();
    void unsureCountChanged();
    void declinedCountChanged();
private:
    Q_DECLARE_PRIVATE(FacebookExtraEventInterface)
};

#endif // FACEBOOKEXTRAEVENTINTERFACE_H
