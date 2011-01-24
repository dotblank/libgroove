/*
 * This file is part of libgroove
 *
 * Copyright (C) 2010 Robin Burchell <viroteck@viroteck.net>
 *
 * This software, including documentation, is protected by copyright.
 * All rights are reserved.
 *
 * Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of the author.
 *
 * This material may also contain confidential information which may not be
 * disclosed to others without the prior written consent of the author.
 */

#ifndef LIBGROOVE_GLOBAL_H
#define LIBGROOVE_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QDebug>

#if defined(LIBGROOVE_LIBRARY)
#  define LIBGROOVESHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBGROOVESHARED_EXPORT Q_DECL_IMPORT
#endif

#define GROOVE_VERIFY(x, y) (!(x) && ((qWarning() << Q_FUNC_INFO << y), Q_ASSERT(x), true))
#define GROOVE_VERIFY_OR_DIE(x, y) if (!(x)) { qFatal("%s%s", Q_FUNC_INFO, y); }

namespace Groove
{
    enum ConnectionError
    {
        /*!
           An error occured while communicating with Grooveshark
        */
        CommunicationError,

        /*!
          An error occured while starting a session
        */
        SessionError,
    };
}

#endif // LIBGROOVE_GLOBAL_H
