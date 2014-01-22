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

#include "datehelper.h"
#include <QtCore/QDebug>

DateHelper::DateHelper(QObject *parent)
    : QObject(parent)
{
}

QDateTime DateHelper::fromString(const QString &date, bool *isDateOnly)
{
    QDate dateOnly = QDate::fromString(date, "yyyy-MM-dd");

    if (isDateOnly) {
        *isDateOnly = !dateOnly.isNull();
    }

    QDateTime newDateTime;
    if (!dateOnly.isNull()) {
        newDateTime = QDateTime(dateOnly);
    } else {
        QDateTime dateTime = QDateTime::fromString(date, Qt::ISODate);
        newDateTime = QDateTime(dateTime.date(), dateTime.time(), Qt::UTC); // Ensuring UTC time
        qDebug() << date << dateTime << newDateTime;
    }
    return newDateTime.toTimeSpec(Qt::LocalTime);
}

QString DateHelper::formatDateTime(const QString &date, Qt::DateFormat format) {
    bool isDateOnly = false;
    QDateTime dateTime = fromString(date, &isDateOnly);
    QString dateString = dateTime.date().toString(format);
    if (isDateOnly) {
        return dateString;
    }

    return QString("%1, %2").arg(dateString, dateTime.time().toString(format));
}
