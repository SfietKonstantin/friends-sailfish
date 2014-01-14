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

#include "threadhelper.h"
#include "objecthelper_p.h"
#include <QtCore/QDebug>

ThreadHelper::ThreadHelper(QObject *parent) :
    AbstractDisplayHelper(parent), m_valid(true)
{
}

bool ThreadHelper::isValid() const
{
    return m_valid;
}

QString ThreadHelper::header() const
{
    return m_header;
}

QString ThreadHelper::message() const
{
    return m_message;
}

QString ThreadHelper::pictureIdentifier() const
{
    return m_pictureIdentifier;
}

void ThreadHelper::clearTo()
{
    m_to.clear();
    create();
}

void ThreadHelper::addTo(QObject *to)
{
    m_to.append(to);
    create();
}

void ThreadHelper::performCreationImpl()
{
    // Manage to
    QList<QObject *> toList;
    foreach (QObject *to, m_to) {
        if (getValue(to, "objectIdentifier").toString() != userIdentifier()) {
            toList.append(to);
        }
    }

    bool valid = true;
    QString header;
    QString pictureIdentifier;
    if (toList.count() > 2) {
        const QObject *first = toList.first();
        QString name = getValue(first, "data").toMap().value("first_name").toString();
        pictureIdentifier = getValue(first, "objectIdentifier").toString();
        //: Display one name and a number of other persons. Like "Jack and 7 others". %1 is replaced by the first name, %n by the number of others.
        //% "%1 and %n others"
        header = qtTrId("friends_threadhelper_header_many", toList.count() - 1).arg(decorate(name, pictureIdentifier));
    } else if (toList.count() == 2) {
        const QObject *first = toList.first();
        const QObject *second = toList.at(1);
        QString name1 = getValue(first, "data").toMap().value("first_name").toString();
        pictureIdentifier = getValue(first, "objectIdentifier").toString();
        QString name2 = getValue(second, "data").toMap().value("first_name").toString();
        QString identifier2 = getValue(second, "objectIdentifier").toString();
        //: Display two names. Like "Jack and Jane". %1 is replaced by the first name, %2 by the second
        //% "%1 and %2"
        header = qtTrId("friends_threadhelper_header_two", toList.count() - 1).arg(decorate(name1, pictureIdentifier),
                                                                                   decorate(name2, identifier2));
    } else if (toList.count() == 1) {
        const QObject *first = toList.first();
        QString name = getValue(first, "objectName").toString();
        pictureIdentifier = getValue(first, "objectIdentifier").toString();
        header = decorate(name, pictureIdentifier);
    } else {
        valid = false;
    }

    if (m_header != header) {
        m_header = header;
        emit headerChanged();
    }

    if (m_pictureIdentifier != pictureIdentifier) {
        m_pictureIdentifier = pictureIdentifier;
        emit pictureIdentifierChanged();
    }

    // Message
    QString message;
    QVariantList messageData = getValue(object(), "data").toMap().value("comments").toMap().value("data").toList();
    if (!messageData.isEmpty()) {
        message = messageData.first().toMap().value("message").toString();
    } else {
        valid = false;
    }

    if (m_message != message) {
        m_message = message;
        emit messageChanged();
    }

    if (m_valid != valid) {
        m_valid = valid;
        emit validChanged();
    }
}
