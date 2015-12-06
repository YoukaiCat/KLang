/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "Lexeme.h"

#include "KLang_global.h"

class KLANG_LIBRARY_EXPORT Token// : public QObject
{
public:
    Token(const Lexeme & type, const QString & value, int indexBegin, int indexEnd);

    const Lexeme & getType() const;
    const QString & getValue() const;

    int getIndexBegin() const;
    int getIndexEnd() const;

    QString inspect() const;

private:
    Lexeme type;
    QString value;
    //position of an element in the source code
    int indexBegin;
    int indexEnd;
};

#endif // TOKEN_H
