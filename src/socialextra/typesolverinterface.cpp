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

#include "filterinterface_p.h"
#include "typesolverinterface.h"
#include "identifiablecontentiteminterface_p.h"
#include "facebook/facebookontology_p.h"

class TypeSolverFilterInterfacePrivate: public FilterInterfacePrivate
{
public:
    explicit TypeSolverFilterInterfacePrivate(TypeSolverFilterInterface *q);
private:
    Q_DECLARE_PUBLIC(TypeSolverFilterInterface)
    QString identifier;
};

TypeSolverFilterInterfacePrivate::TypeSolverFilterInterfacePrivate(TypeSolverFilterInterface *q)
    : FilterInterfacePrivate(q)
{
}

TypeSolverFilterInterface::TypeSolverFilterInterface(QObject *parent)
    : FilterInterface(*(new TypeSolverFilterInterfacePrivate(this)), parent)
{
}

QString TypeSolverFilterInterface::identifier() const
{
    Q_D(const TypeSolverFilterInterface);
    return d->identifier;
}

void TypeSolverFilterInterface::setIdentifier(const QString &identifier)
{
    Q_D(TypeSolverFilterInterface);
    if (d->identifier != identifier) {
        d->identifier = identifier;
        emit identifierChanged();
    }
}

bool TypeSolverFilterInterface::isAcceptable(QObject *item, SocialNetworkInterface *socialNetwork) const
{
    if (!testType<FacebookInterface>(socialNetwork)) {
        return false;
    }

    // This filter only works for items
    if (!testType<TypeSolverInterface>(item)) {
        return false;
    }

    return true;
}

bool TypeSolverFilterInterface::performLoadRequestImpl(QObject *item, SocialNetworkInterface *socialNetwork,
                                                       LoadType loadType)
{
    Q_D(TypeSolverFilterInterface);
    QString fields = "id";
    QMap<QString, QString> arguments;
    arguments.insert("metadata", "1");

    FacebookInterface *facebook = qobject_cast<FacebookInterface *>(socialNetwork);
    if (!facebook) {
        return false;
    }

    return d->addHandle(facebook->get(this, d->identifier, QString(), fields, arguments), item,
                        socialNetwork, loadType);
}

bool TypeSolverFilterInterface::performSetItemDataImpl(IdentifiableContentItemInterface *item,
                                                       SocialNetworkInterface *socialNetwork,
                                                       const QByteArray &data, LoadType loadType,
                                                       const QVariantMap &properties)
{
    Q_UNUSED(socialNetwork)
    Q_UNUSED(properties)
    Q_UNUSED(loadType)
    bool ok = false;
    QVariantMap dataMap = IdentifiableContentItemInterfacePrivate::parseReplyData(data, &ok);

    if (!ok) {
        QString errorMessage = QString(QLatin1String("Unable to parse downloaded data. "\
                                                     "Downloaded data: %1")).arg(QString(data));
        item->setError(SocialNetworkInterface::DataError, errorMessage);
        return false;
    }

    dataMap.insert(NEMOQMLPLUGINS_SOCIAL_CONTENTITEMID,
                   dataMap.value(FACEBOOK_ONTOLOGY_METADATA_ID).toString());
    item->setData(dataMap);
    return true;
}


class TypeSolverInterfacePrivate: public IdentifiableContentItemInterfacePrivate
{
public:
    explicit TypeSolverInterfacePrivate(TypeSolverInterface *q);
    void emitPropertyChangeSignals(const QVariantMap &oldData, const QVariantMap &newData);
    FacebookInterface::ContentItemType objectType;
};

TypeSolverInterfacePrivate::TypeSolverInterfacePrivate(TypeSolverInterface *q)
    : IdentifiableContentItemInterfacePrivate(q), objectType(FacebookInterface::Unknown)
{
}

void TypeSolverInterfacePrivate::emitPropertyChangeSignals(const QVariantMap &oldData,
                                                           const QVariantMap &newData)
{
    // TODO: identify more types
    QVariantMap metadata = newData.value("metadata").toMap();
    QString type = metadata.value("type").toString();
    qDebug() << type;

    // Call super class implementation
    QVariantMap oldDataWithId = oldData;
    oldDataWithId.insert(NEMOQMLPLUGINS_SOCIAL_CONTENTITEMID,
                         oldData.value(FACEBOOK_ONTOLOGY_METADATA_ID));
    QVariantMap newDataWithId = newData;
    newDataWithId.insert(NEMOQMLPLUGINS_SOCIAL_CONTENTITEMID,
                         newData.value(FACEBOOK_ONTOLOGY_METADATA_ID));
    IdentifiableContentItemInterfacePrivate::emitPropertyChangeSignals(oldDataWithId, newDataWithId);
}

TypeSolverInterface::TypeSolverInterface(QObject *parent) :
    IdentifiableContentItemInterface(*(new TypeSolverInterfacePrivate(this)), parent)
{
}

int TypeSolverInterface::type() const
{
    return FacebookInterface::ObjectReference;
}

FacebookInterface::ContentItemType TypeSolverInterface::objectType() const
{
    Q_D(const TypeSolverInterface);
    return d->objectType;
}
