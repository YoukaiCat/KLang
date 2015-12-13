/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Token.h"

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

//"Техническая" инспеция для отладки парсера
QString Token::inspect() const
{
    return "(:" + inspectLexeme(type) + ", " + value + ")";
}

//Пользовательская инспекция для отладки пользовательского кода
QString Token::toString() const
{
    switch (type) {
        case Lexeme::Begin:
        case Lexeme::SingleDeclaration:
        case Lexeme::MultipleDeclaration:
        case Lexeme::End:
            return QString("получено ключевое слово '%1'").arg(value);
        case Lexeme::LeftParentheses:
            return QString("получена открывающая скобка");
        case Lexeme::RightParentheses:
            return QString("получена закрывающая скобка");
        case Lexeme::Equality:
        case Lexeme::Semicolon:
        case Lexeme::Comma:
            return QString("получен символ '%1'").arg(value);
        case Lexeme::Plus:
        case Lexeme::Minus:
        case Lexeme::Multiply:
        case Lexeme::Divide:
        case Lexeme::And:
        case Lexeme::Or:
        case Lexeme::Not:
            return QString("получен оператор '%1'").arg(value);
        case Lexeme::Id:
            return QString("получена переменная '%1'").arg(value);
        case Lexeme::Num:
            return QString("получено число '%1'").arg(value);
        case Lexeme::UnaryMinus:
            return QString("получен унарный минус");
        case Lexeme::Declarations:
            return QString("получены объявления переменных");
        case Lexeme::Assignments:
            return QString("получены присвоения");
        case Lexeme::Eof:
            return QString("получен конец ввода");
        default:
            return QString("получен неизвестный тип лексемы (%1)").arg(inspectLexeme(type));
    }
}
