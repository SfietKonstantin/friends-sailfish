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

#ifndef OBJECTHELPER_P_H
#define OBJECTHELPER_P_H

#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QVariant>

inline QVariant getValue(const QObject *object, const char *property)
{
    const QMetaObject *metaObject = object->metaObject();
    int index = metaObject->indexOfProperty(property);
    if (index == -1) {
        return QVariant();
    }

    QMetaProperty metaProperty = metaObject->property(index);
    return metaProperty.read(object);
}

inline QMetaEnum getEnum(const QObject *object, const char *enumName)
{
    const QMetaObject *metaObject = object->metaObject();
    int index = metaObject->indexOfEnumerator(enumName);
    return metaObject->enumerator(index);
}

inline bool validateEnum(const QMetaEnum &enumerator, int value, const char *enumValue)
{
    return QByteArray(enumerator.valueToKey(value)) == QByteArray(enumValue);
}

inline bool validateFlag(const QMetaEnum &enumerator, int value, const char *enumValue)
{
    int flag = enumerator.keysToValue(enumValue);
    return (value & flag) == flag;
}

#endif // OBJECTHELPER_P_H
