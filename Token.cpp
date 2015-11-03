/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Token.h"

#include <QDebug>

Token::Token(const Lexeme & type, const QString & value,  int indexBegin, int indexEnd)
    : type(type)
    , value(value)
    , indexBegin(indexBegin)
    , indexEnd(indexEnd)
{}

const Lexeme & Token::getType() const
{
    return type;
}

const QString & Token::getValue() const
{
    return value;
}

int Token::getIndexBegin() const
{
    return indexBegin;
}

int Token::getIndexEnd() const
{
    return indexEnd;
}

QString Token::inspect() const
{
    return "(:" + inspectLexeme(type) + ", " + value + ")";
}
