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

#ifndef TYPESOLVERINTERFACE_H
#define TYPESOLVERINTERFACE_H

#include "filterinterface.h"
#include "identifiablecontentiteminterface.h"
#include "facebook/facebookinterface.h"

class TypeSolverFilterInterfacePrivate;
class TypeSolverFilterInterface: public FilterInterface
{
    Q_OBJECT
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
public:
    explicit TypeSolverFilterInterface(QObject *parent = 0);

    // Properties
    QString identifier() const;
    void setIdentifier(const QString &identifier);

    // Non QML API
    // Used by items
    bool isAcceptable(QObject *item, SocialNetworkInterface *socialNetwork) const;
Q_SIGNALS:
    void identifierChanged();
    void fieldsChanged();

protected:
    bool performLoadRequestImpl(QObject *item, SocialNetworkInterface *socialNetwork,
                                LoadType loadType);
    bool performSetItemDataImpl(IdentifiableContentItemInterface *item,
                                SocialNetworkInterface *socialNetwork,
                                const QByteArray &data, LoadType loadType,
                                const QVariantMap &properties);
private:
    Q_DECLARE_PRIVATE(TypeSolverFilterInterface)
};


// This ICII is only provides the type of item that corresponds to
// an identifier. It can only be used with the specific TypeSolverFilterInterface
class TypeSolverInterfacePrivate;
class TypeSolverInterface : public IdentifiableContentItemInterface
{
    Q_OBJECT
    Q_PROPERTY(FacebookInterface::ContentItemType objectType READ objectType NOTIFY objectTypeChanged)
    Q_PROPERTY(QString objectTypeString READ objectTypeString NOTIFY objectTypeStringChanged)
public:
    explicit TypeSolverInterface(QObject *parent = 0);
    int type() const;

    // Accessors
    FacebookInterface::ContentItemType objectType() const;
    QString objectTypeString() const;
Q_SIGNALS:
    void objectTypeChanged();
    void objectTypeStringChanged();
private:
    Q_DECLARE_PRIVATE(TypeSolverInterface)
};

#endif // TYPESOLVERINTERFACE_H
