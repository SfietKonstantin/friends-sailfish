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

#ifndef COMMENTFILTERINTERFACE_H
#define COMMENTFILTERINTERFACE_H

#include "filterinterface.h"

class CommentFilterInterfacePrivate;
class CommentFilterInterface : public FilterInterface
{
    Q_OBJECT
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
    Q_PROPERTY(bool isPost READ isPost WRITE setIsPost NOTIFY isPostChanged)
public:
    explicit CommentFilterInterface(QObject *parent = 0);
    QString identifier() const;
    void setIdentifier(const QString &identifier);
    int limit() const;
    void setLimit(int limit);
    bool isPost() const;
    void setIsPost(bool isPost);

    bool isAcceptable(QObject *item, SocialNetworkInterface *socialNetwork) const;
signals:
    void identifierChanged();
    void limitChanged();
//    void offsetChanged();
    void isPostChanged();
protected:
    bool performLoadRequestImpl(QObject *item, SocialNetworkInterface *socialNetwork,
                                LoadType loadType);
    bool performSetModelDataImpl(SocialNetworkModelInterface *model,
                                 SocialNetworkInterface *socialNetwork, const QByteArray &data,
                                 LoadType loadType, const QVariantMap &properties);
private:
    Q_DECLARE_PRIVATE(CommentFilterInterface)
};

#endif // COMMENTFILTERINTERFACE_H
