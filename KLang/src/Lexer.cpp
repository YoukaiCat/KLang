/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <QRegularExpression>

#include "Lexer.h"

Lexer::Lexer(const QString & source, QObject * parent)
    : QObject(parent)
    , tokens(std::make_shared<QList<Token>>(QList<Token>()))
    , src(Source(source))
    , continueDespiteErrors(false)
{
    symbols["("] = Lexeme::LeftParentheses;
    symbols[")"] = Lexeme::RightParentheses;
    symbols["="] = Lexeme::Equality;
    symbols[";"] = Lexeme::Semicolon;
    symbols[","] = Lexeme::Comma;
    symbols["+"] = Lexeme::Plus;
    symbols["-"] = Lexeme::Minus;
    symbols["*"] = Lexeme::Multiply;
    symbols["/"] = Lexeme::Divide;

    words["начало"] = Lexeme::Begin;
    words["окончание"] = Lexeme::End;
    words["анализ"] = Lexeme::SingleDeclaration;
    words["синтез"] = Lexeme::MultipleDeclaration;
    words["и"] = Lexeme::And;
    words["или"] = Lexeme::Or;
    words["не"] = Lexeme::Not;
}

Lexer::Lexer(const QString & source, bool continueDespiteErrors, QObject * parent)
    : Lexer(source, parent)
{
    this->continueDespiteErrors = continueDespiteErrors;
}

shared_ptr<QList<Token>> Lexer::tokenize()
{
    processSource();
    tokens->append(Token(Lexeme::Eof, "Конец ввода", src.getIndex(), src.getIndex()));
    return tokens;
}

void Lexer::processSource()
{
    while (!src.isEmpty()) {
        auto current = src.shift();
        if (current.contains(QRegularExpression("^(\\s|\\n)?$"))) {
            //Пробелы, переводы строк и пустые строки пропускаются
        } else if (symbols.contains(current)) {
            tokens->append(Token(symbols[current], current, src.getIndex(), src.getIndex() + 1));
        } else if (current.contains(QRegularExpression("^\\d$"))) {
            processDigits(current);
        } else if (current.contains(QRegularExpression("^[А-Яа-я]$"))) {
            processWords(current);
        } else {
            reportError(Error(1, "Неизвестный символ: '" + current + "'.\nДопустимы только буквы русского алфавита, вещественные числа и знаки операций.", src.getIndex(), src.getIndex() + 1));
        }
    }
}

void Lexer::processDigits(QString & current)
{
    auto index = src.getIndex();
    while (src.first().contains(QRegularExpression("^(\\d|\\.)$"))) {
        current += src.shift();
    }
    if (current.contains(QRegularExpression("^\\d+\\.\\d+$"))) {
        tokens->append(Token(Lexeme::Num, current, index, src.getIndex() + 1));
    } else {
        reportError(Error(2, "Допустимы только вещественные числа.", index, src.getIndex() + 1));
    }
}

void Lexer::processWords(QString & current)
{
    auto index = src.getIndex();
    while (src.first().contains(QRegularExpression("^([А-Яа-я]|\\d)$"))) {
        current += src.shift();
    }
    if (words.contains(current.toLower())) {
        tokens->append(Token(words[current.toLower()], current, index, src.getIndex() + 1));
    } else if (current.contains(QRegularExpression("^[А-Яа-я]\\d{0,3}$"))) {
        tokens->append(Token(Lexeme::Id, current, index, src.getIndex() + 1));
    } else {
        reportError(Error(3, "Недопустимый идентификатор: '" + current + "'.\nКлючевые слова языка: Начало, Анализ, Синтез, Окончание. Переменные могут содержать только букву и до трех цифр после неё.", index, src.getIndex() + 1));
    }
}

void Lexer::reportError(Error e)
{
    if (!continueDespiteErrors) {
        throw e;
    }
}
