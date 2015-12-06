/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Lexeme.h"

QString inspectLexeme(const Lexeme & lexeme) {
    switch (lexeme) {
        case Lexeme::Begin:
            return QString("Begin");
        case Lexeme::SingleDeclaration:
            return QString("SingleDeclaration");
        case Lexeme::MultipleDeclaration:
            return QString("MultipleDeclaration");
        case Lexeme::End:
            return QString("End");
        case Lexeme::LeftParentheses:
            return QString("LeftParentheses");
        case Lexeme::RightParentheses:
            return QString("RightParentheses");
        case Lexeme::Equality:
            return QString("Equality");
        case Lexeme::Semicolon:
            return QString("Semicolon");
        case Lexeme::Comma:
            return QString("Comma");
        case Lexeme::Plus:
            return QString("Plus");
        case Lexeme::Minus:
            return QString("Minus");
        case Lexeme::Multiply:
            return QString("Multiply");
        case Lexeme::Divide:
            return QString("Divide");
        case Lexeme::And:
            return QString("And");
        case Lexeme::Or:
            return QString("Or");
        case Lexeme::Not:
            return QString("Not");
        case Lexeme::Id:
            return QString("Id");
        case Lexeme::Num:
            return QString("Num");
        case Lexeme::UnaryMinus:
            return QString("UnaryMinus");
        case Lexeme::Declarations:
            return QString("Declarations");
        case Lexeme::Assignments:
            return QString("Assignments");
        case Lexeme::Eof:
            return QString("Eof");
        default:
            return QString("Unknown lexeme: ") + (int)lexeme;
    }
}
