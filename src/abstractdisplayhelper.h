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

#ifndef ABSTRACTDISPLAYHELPER_H
#define ABSTRACTDISPLAYHELPER_H

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <QtQml/QQmlParserStatus>

class AbstractDisplayHelper : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QVariantMap object READ object WRITE setObject NOTIFY objectChanged)
    Q_PROPERTY(QString userIdentifier READ userIdentifier WRITE setUserIdentifier
               NOTIFY userIdentifierChanged)
    Q_PROPERTY(QString primaryColor READ primaryColor WRITE setPrimaryColor
               NOTIFY primaryColorChanged)
    Q_PROPERTY(QString highlightColor READ highlightColor WRITE setHighlightColor
               NOTIFY highlightColorChanged)
public:
    void classBegin() Q_DECL_OVERRIDE;
    void componentComplete() Q_DECL_OVERRIDE;
    const QVariantMap & object() const;
    void setObject(const QVariantMap &object);
    QString userIdentifier() const;
    void setUserIdentifier(const QString &userIdentifier);
    QString primaryColor() const;
    void setPrimaryColor(const QString &primaryColor);
    QString highlightColor() const;
    void setHighlightColor(const QString &highlightColor);
signals:
    void objectChanged();
    void userIdentifierChanged();
    void primaryColorChanged();
    void highlightColorChanged();
protected:
    explicit AbstractDisplayHelper(QObject *parent = 0);
    QString decorate(const QString &text, const QString &url);
    QString standardize(const QString &text);
    virtual void performCreationImpl() = 0;
protected slots:
    void create();
private:
    enum ComponentStatus {
        NotReady,
        NotReadyCreationPending,
        Ready
    };
    ComponentStatus m_componentStatus;
    QVariantMap m_object;
    QString m_userIdentifier;
    QString m_primaryColor;
    QString m_highlightColor;
};

#endif // ABSTRACTDISPLAYHELPER_H
