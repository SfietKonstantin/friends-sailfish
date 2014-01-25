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

#include "abstractdisplayhelper.h"
#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>

static const char *NON_INTERACTIVE_TEXT = "<span style=\"color:%1\">%2</span>";
static const char *INTERACTIVE_TEXT = "<a style=\"text-decoration:none; color:%1\" href=\"%2\">%3</a>";

AbstractDisplayHelper::AbstractDisplayHelper(QObject *parent) :
    QObject(parent), m_object(0)
{
}

QObject * AbstractDisplayHelper::object() const
{
    return m_object;
}

void AbstractDisplayHelper::setObject(QObject *object)
{
    if (m_object != object) {
        if (m_object) {
            m_object->disconnect(this);
        }
        m_object = object;
        connect(m_object, SIGNAL(loaded(bool)), this, SLOT(create()));
        emit objectChanged();
        create();
    }
}

QString AbstractDisplayHelper::userIdentifier() const
{
    return m_userIdentifier;
}

void AbstractDisplayHelper::setUserIdentifier(const QString &userIdentifier)
{
    if (m_userIdentifier != userIdentifier) {
        m_userIdentifier = userIdentifier;
        emit userIdentifierChanged();
        create();
    }
}

QString AbstractDisplayHelper::primaryColor() const
{
    return m_primaryColor;
}

void AbstractDisplayHelper::setPrimaryColor(const QString &primaryColor)
{
    if (m_primaryColor != primaryColor) {
        m_primaryColor = primaryColor;
        emit primaryColorChanged();
        create();
    }
}

QString AbstractDisplayHelper::highlightColor() const
{
    return m_highlightColor;
}

void AbstractDisplayHelper::setHighlightColor(const QString &highlightColor)
{
    if (m_highlightColor != highlightColor) {
        m_highlightColor = highlightColor;
        emit highlightColorChanged();
        create();
    }
}

QString AbstractDisplayHelper::interactive(const QString &text, const QString &url)
{
    return QString(INTERACTIVE_TEXT).arg(m_primaryColor, url, text);
}

QString AbstractDisplayHelper::nonInteractive(const QString &text)
{
    return QString(NON_INTERACTIVE_TEXT).arg(m_highlightColor, text);
}

bool AbstractDisplayHelper::event(QEvent *e)
{
    if (e->type() == QEvent::User) {
        performCreation();
        return true;
    }
    return QObject::event(e);
}

void AbstractDisplayHelper::create()
{
    QCoreApplication::instance()->postEvent(this, new QEvent(QEvent::User));
}

void AbstractDisplayHelper::performCreation()
{
    if (!m_object) {
        return;
    }

    performCreationImpl();
}

