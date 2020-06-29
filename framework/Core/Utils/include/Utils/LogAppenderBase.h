//  -------------------------------------------------------------------------
//  Copyright (C) 2013 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef RAMSES_LOGAPPENDERBASE_H
#define RAMSES_LOGAPPENDERBASE_H

namespace ramses_internal
{
    class LogMessage;

    class LogAppenderBase
    {
    public:
        virtual ~LogAppenderBase() = default;
        virtual void log(const LogMessage& logMessage) = 0;
    };
}

#endif
