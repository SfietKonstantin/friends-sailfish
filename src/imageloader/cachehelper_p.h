/****************************************************************************************
 * Copyright (C) 2012 Lucien XU <sfietkonstantin@free.fr>                               *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 3 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

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

/**
 * @internal
 * @brief Path to the cache folder
 * @return path to the cache folder.
 */
inline QString cacheFolderPath()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    return QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
#else
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
#endif
}

#endif // QFB_CACHEHELPER_P_H
