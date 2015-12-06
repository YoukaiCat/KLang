/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Lexer.h"

#include <QDebug>

Lexer::Lexer(const QString & source, QObject * parent) : QObject(parent)
{
    src = new Source(source, this);

    symbols["("] = Lexeme::LeftParentheses;
    symbols[")"] = Lexeme::RightParentheses;
    symbols["="] = Lexeme::Equality;
    symbols[";"] = Lexeme::Semicolon;
    symbols[","] = Lexeme::Comma;
    symbols["+"] = Lexeme::Plus;
    symbols["-"] = Lexeme::Minus;
    symbols["*"] = Lexeme::Multiply;
    symbols["/"] = Lexeme::Divide;

    words["Начало"] = Lexeme::Begin;
    words["Окончание"] = Lexeme::End;
    words["Анализ"] = Lexeme::SingleDeclaration;
    words["Синтез"] = Lexeme::MultipleDeclaration;
    words["И"] = Lexeme::And;
    words["ИЛИ"] = Lexeme::Or;
    words["НЕ"] = Lexeme::Not;
}

QList<Token> Lexer::tokenize()
{
    processSource();
    tokens.append(Token(Lexeme::Eof, "Конец ввода", src->getIndex(), src->getIndex()));
    return tokens;
}

void Lexer::processSource()
{
    while (!src->isEmpty()) {
        auto current = src->shift();
        if (current.contains(QRegularExpression("^(\\s|\\n)?$"))) {
            //Пробелы, переводы строк и пустые строки пропускаются
        } else if (symbols.contains(current)) {
            tokens.append(Token(symbols[current], current, src->getIndex(), src->getIndex() + 1));
        } else if (current.contains(QRegularExpression("^\\d$"))) {
            processDigits(current);
        } else if (current.contains(QRegularExpression("^[А-Яа-я]$"))) {
            processWords(current);
        } else {
            throw Error("Неизвестный символ: '" + current + "'.\nДопустимы только буквы русского алфавита, вещественные числа и знаки операций.", src->getIndex(), src->getIndex() + 1);
        }
    }
}

void Lexer::processDigits(QString & current)
{
    auto index = src->getIndex();
    while (src->first().contains(QRegularExpression("^(\\d|\\.)$"))) {
        current += src->shift();
    }
    if (current.contains(QRegularExpression("^\\d+\\.\\d+$"))) {
        tokens.append(Token(Lexeme::Num, current, index, src->getIndex() + 1));
    } else {
        throw Error("Допустимы только вещественные числа.", index, src->getIndex() + 1);
    }
}

void Lexer::processWords(QString & current)
{
    auto index = src->getIndex();
    while (src->first().contains(QRegularExpression("^([А-Яа-я]|\\d)$"))) {
        current += src->shift();
    }
    if (words.contains(current)) {
        tokens.append(Token(words[current], current, index, src->getIndex() + 1));
    } else if (current.contains(QRegularExpression("^[А-Яа-я]\\d{0,3}$"))) {
        tokens.append(Token(Lexeme::Id, current, index, src->getIndex() + 1));
    } else {
        throw Error("Недопустимый идентификатор: '" + current + "'.\nКлючевые слова языка: Начало, Анализ, Синтез, Окончание. Переменные могут содержать только букву и до трех цифр после неё.", index, src->getIndex() + 1);
    }
}