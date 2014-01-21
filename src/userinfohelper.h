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

#ifndef USERINFOHELPER_H
#define USERINFOHELPER_H

#include "abstractdisplayhelper.h"

class UserInfoHelper : public AbstractDisplayHelper
{
    Q_OBJECT
    Q_PROPERTY(QString gender READ gender NOTIFY genderChanged)
    Q_PROPERTY(QString interestedIn READ interestedIn NOTIFY interestedInChanged)
    Q_PROPERTY(QString birthday READ birthday NOTIFY birthdayChanged)
    Q_PROPERTY(QString religion READ religion NOTIFY religionChanged)
    Q_PROPERTY(QString political READ political NOTIFY politicalChanged)
    Q_PROPERTY(QString location READ location NOTIFY locationChanged)
    Q_PROPERTY(QString hometown READ hometown NOTIFY hometownChanged)
public:
    explicit UserInfoHelper(QObject *parent = 0);
    Q_INVOKABLE QString colorText(const QString &text);
    QString gender() const;
    QString interestedIn() const;
    QString birthday() const;
    QString religion() const;
    QString political() const;
    QString location() const;
    QString hometown() const;
signals:
    void genderChanged();
    void interestedInChanged();
    void birthdayChanged();
    void religionChanged();
    void politicalChanged();
    void locationChanged();
    void hometownChanged();
protected:
    void performCreationImpl();
private:
    QString labeledText(const QString &label, const QString &text);
    QString m_gender;
    QString m_interestedIn;
    QString m_birthday;
    QString m_religion;
    QString m_political;
    QString m_location;
    QString m_hometown;
};

#endif // USERINFOHELPER_H
