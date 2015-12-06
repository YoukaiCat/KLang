/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef LEXEME
#define LEXEME

#include <QString>

#include "KLang_global.h"

enum class Lexeme {
    //Keywords
    Begin,
    SingleDeclaration,
    MultipleDeclaration,
    End,
    //Symbols
    LeftParentheses,
    RightParentheses,
    Equality,
    Semicolon,
    Comma,
    Plus,
    Minus,
    Multiply,
    Divide,
    And,
    Or,
    Not,
    //Runtime
    Id,
    Num,
    UnaryMinus,
    Declarations,
    Assignments,
    //Special
    Eof
};

QString inspectLexeme(const Lexeme & lexeme);

#endif // LEXEME
