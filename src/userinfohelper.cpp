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

#include "userinfohelper.h"
#include "objecthelper_p.h"

#include <QtCore/QStringList>
#include <QtCore/QDebug>

static const char *COLORED_TEXT = "<span style=\"text-decoration:none; color:%1\">%2</span>";
static const char *LABELLED_TEXT = "<span style=\"text-decoration:none; color:%1\">%2</span>: %3";

UserInfoHelper::UserInfoHelper(QObject *parent) :
    AbstractDisplayHelper(parent)
{
}

QString UserInfoHelper::colorText(const QString &text)
{
    QString changedText = text;
    changedText.replace("<", "&lt;");
    changedText.replace(">", "&gt;");
    changedText.replace("\n", "<br/>");

    return QString(COLORED_TEXT).arg(highlightColor(), changedText);
}

QString UserInfoHelper::gender() const
{
    return m_gender;
}

QString UserInfoHelper::interestedIn() const
{
    return m_interestedIn;
}

QString UserInfoHelper::birthday() const
{
    return m_birthday;
}

QString UserInfoHelper::religion() const
{
    return m_religion;
}

QString UserInfoHelper::political() const
{
    return m_political;
}

QString UserInfoHelper::location() const
{
    return m_location;
}

QString UserInfoHelper::hometown() const
{
    return m_hometown;
}

void UserInfoHelper::performCreationImpl()
{
    QMetaEnum genderEnum = getEnum(object(), "Gender");
    QString gender;
    if (validateEnum(genderEnum, getValue(object(), "gender").toInt(), "Male")) {
        //: Male gender. It is following ":" so shouldn't be uppercased.
        //% "male"
        gender = qtTrId("friends_user_info_male");
    } else if (validateEnum(genderEnum, getValue(object(), "gender").toInt(), "Female")) {
        //: Female gender. It is following ":" so shouldn't be uppercased.
        //% "female"
        gender = qtTrId("friends_user_info_female");
    }

    if (!gender.isEmpty()) {
        //: Label indicating the gender of the user. It appears before the actual gender.
        //% "Gender"
        gender = labeledText(qtTrId("friends_user_info_gender"), gender);
    }

    if (m_gender != gender) {
        m_gender = gender;
        emit genderChanged();
    }

    int interestedInValue = getValue(object(), "interestedIn").toInt();
    bool interestedInMen = validateFlag(genderEnum, interestedInValue, "Male");
    bool interestedInWomen = validateFlag(genderEnum, interestedInValue, "Female");
    QString interestedIn;
    if (interestedInMen && interestedInWomen) {
        //: Label indicating that the user is interested by men and women. It is following ":" so shouldn't be uppercased.
        //% "men and women"
        interestedIn = qtTrId("friends_user_info_interested-in-men-women");
    } else if (interestedInMen && !interestedInWomen) {
        //: Label indicating that the user is interested by men. It is following ":" so shouldn't be uppercased.
        //% "men"
        interestedIn = qtTrId("friends_user_info_interested-in-men");
    } else if (!interestedInMen && interestedInWomen) {
        //: Label indicating that the user is interested by women. It is following ":" so shouldn't be uppercased.
        //% "women"
        interestedIn = qtTrId("friends_user_info_interested-in-women");
    }

    if (!interestedIn.isEmpty()) {
        //: Label indicating which people the user is interested in. It appears before the actual interest
        //% "Interested in"
        interestedIn = labeledText(qtTrId("friends_user_info_interested-in"), interestedIn);
    }

    if (m_interestedIn != interestedIn) {
        m_interestedIn = interestedIn;
        emit interestedInChanged();
    }

    //: Label indicating the birthday of the user. It appears before the actual birthday
    //% "Birthday"
    QString birthday = labeledText(qtTrId("friends_user_info_birthday"),
                                   getValue(object(), "birthday").toString());
    if (m_birthday != birthday) {
        m_birthday = birthday;
        emit birthdayChanged();
    }

    //: Label indicating the religious views of the user. It appears before the actual views
    //% "Religious views"
    QString religion = labeledText(qtTrId("friends_user_info_religion"),
                                   getValue(object(), "religion").toString());
    if (m_religion != religion) {
        m_religion = religion;
        emit religionChanged();
    }

    //: Label indicating the political views of the user. It appears before the actual views
    //% "Political views"
    QString political = labeledText(qtTrId("friends_user_info_political"),
                                    getValue(object(), "political").toString());
    if (m_political != political) {
        m_political = political;
        emit politicalChanged();
    }

    QObject *locationObject = getValue(object(), "location").value<QObject *>();
    if (locationObject) {
        QString identifier = getValue(locationObject, "objectIdentifier").toString();
        QString name = getValue(locationObject, "objectName").toString();
        //: Label indicating the current location of the user. It appears before the actual location
        //% "Current location"
        QString location = labeledText(qtTrId("friends_user_info_location"),
                                       decorate(name, identifier));

        if (m_location != location) {
            m_location = location;
            emit locationChanged();
        }
    }

    QObject *hometownObject = getValue(object(), "hometown").value<QObject *>();
    if (hometownObject) {
        QString identifier = getValue(hometownObject, "objectIdentifier").toString();
        QString name = getValue(hometownObject, "objectName").toString();
        //: Label indicating the hometown of the user. It appears before the actual location
        //% "Hometown"
        QString hometown = labeledText(qtTrId("friends_user_info_hometown"),
                                       decorate(name, identifier));

        if (m_hometown != hometown) {
            m_hometown = hometown;
            emit hometownChanged();
        }
    }
}

QString UserInfoHelper::labeledText(const QString &label, const QString &text)
{
    return QString(LABELLED_TEXT).arg(highlightColor(), label, text);
}
