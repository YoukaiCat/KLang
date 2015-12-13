/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Error.h"

Error::Error(int errorCode, const QString & message, int begin, int end)
    : errorCode(errorCode)
    , message(message)
    , begin(begin)
    , end(end)
{}

void Error::raise() const
{
    throw *this;
}

Error * Error::clone() const
{
    return new Error(*this);
}

int Error::getErrorCode()
{
    return errorCode;
}

const QString & Error::getMessage() const
{
    return message;
}

int Error::getBegin() const
{
    return begin;
}

int Error::getEnd() const
{
    return end;
}
