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
