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

#ifndef QFB_LOGIN_GLOBAL_H
#define QFB_LOGIN_GLOBAL_H

/**
 * @file login_global.h
 * @short Global qfblogin library header
 */

#include <QtCore/qglobal.h>

/**
 * \page login Login library
 *
 * The %QFB login library is used to perform a login to Facebook service.
 * It needs a web view to display the login screen and other dialogs that
 * Facebook uses for the OAuth 2 authentification process. At the end of
 * the process, a token is retrieved, and should be used in Facebook API
 * calls.
 *
 * In order to perform the operation, and get the token, QFB::LoginManager
 * should be used. It plugs into a web view (either C++ or QML) and handles
 * all the operations.Please read the documentation page for
 * QFB::LoginManager to learn how to use that class.
 *
 * @todo continue this page
 * @todo add disconnect, cookie disabling and other stuff
 */

/**
 * \def QFBLOGIN_EXPORT
 * @short Library export or import
 */

#if defined(QFBLOGIN_LIBRARY)
#  define QFBLOGIN_EXPORT Q_DECL_EXPORT
#else
#  define QFBLOGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // QFB_LOGIN_GLOBAL_H
