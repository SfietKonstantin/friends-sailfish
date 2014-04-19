/*
 * Copyright (C) 2014 Lucien XU <sfietkonstantin@free.fr>
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

#include "friendsdbusinterface.h"
#include <QtCore/QDebug>
#include <QtDBus/QDBusConnection>
#include <QtQuick/QQuickView>
#include "adaptor.h"

static const char *SERVICE = "harbour.friends";
static const char *PATH = "/";

class FriendsDBusInterfacePrivate
{
public:
    explicit FriendsDBusInterfacePrivate(FriendsDBusInterface *q);
    QQuickView *view;
protected:
    FriendsDBusInterface * const q_ptr;
};

FriendsDBusInterfacePrivate::FriendsDBusInterfacePrivate(FriendsDBusInterface *q)
    : view(0), q_ptr(q)
{
}

FriendsDBusInterface::FriendsDBusInterface(QObject *parent) :
    QObject(parent), d_ptr(new FriendsDBusInterfacePrivate(this))
{
    new FriendsAdaptor(this);
    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.registerService(SERVICE)) {
        qDebug() << "Failed to register DBus service";
        return;
    }
    if (!connection.registerObject(PATH, this)) {
        qDebug() << "Failed to register DBus object";
        return;
    }
}

FriendsDBusInterface::~FriendsDBusInterface()
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.unregisterObject(PATH);
    connection.unregisterService(SERVICE);
}

void FriendsDBusInterface::registerView(QQuickView *view)
{
    Q_D(FriendsDBusInterface);
    d->view = view;
}

void FriendsDBusInterface::openFacebookEntity(const QString &facebookId)
{
    Q_D(FriendsDBusInterface);
    if (d->view) {
        d->view->raise();
    }
    emit openFacebookEntityRequested(facebookId);
}

void FriendsDBusInterface::openNotifications()
{
    Q_D(FriendsDBusInterface);
    if (d->view) {
        d->view->raise();
    }
    emit openNotificationsRequested();
}
