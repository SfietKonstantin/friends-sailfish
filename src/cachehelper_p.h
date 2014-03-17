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

#ifndef QFB_CACHEHELPER_P_H
#define QFB_CACHEHELPER_P_H

// Warning
//
// This file exists for the convenience
// of other qfb classes.
// This header file may change from version
// to version without notice or even be removed.

/**
 * @internal
 * @file cachehelper_p.h
 * @short Cache helper functions
 */

#include <QtCore/QString>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <QtGui/QDesktopServices>
#else
#include <QtCore/QStandardPaths>
#endif
#include <QtCore/QCoreApplication>
#include "defines_p.h"

/**
 * @internal
 * @brief Path to the cache folder
 * @return path to the cache folder.
 */
inline QString cacheFolderPath()
{
    QString oldOrganizationName = QCoreApplication::instance()->organizationName();
    QString oldApplicationName = QCoreApplication::instance()->applicationName();
    if (oldOrganizationName != ORGANIZATION_NAME) {
        QCoreApplication::instance()->setOrganizationName(ORGANIZATION_NAME);
    }

    if (oldApplicationName != APPLICATION_NAME) {
        QCoreApplication::instance()->setOrganizationName(APPLICATION_NAME);
    }


#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QString location = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
    QString location = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif

    if (oldOrganizationName != ORGANIZATION_NAME) {
        QCoreApplication::instance()->setOrganizationName(oldOrganizationName);
    }

    if (oldApplicationName != APPLICATION_NAME) {
        QCoreApplication::instance()->setOrganizationName(oldApplicationName);
    }

    return location;
}

#endif // QFB_CACHEHELPER_P_H
