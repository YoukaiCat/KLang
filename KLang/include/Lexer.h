/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef LEXER_H
#define LEXER_H

#include <QMap>
#include <QRegularExpression>

#include "Lexeme.h"
#include "Source.h"
#include "Token.h"
#include "Error.h"

#include "KLang_global.h"

class KLANG_LIBRARY_EXPORT Lexer : public QObject
{
    Q_OBJECT
public:
    explicit Lexer(const QString & source, QObject * parent = 0);
    explicit Lexer(const QString & source, bool continueDespiteErrors, QObject * parent = 0);
    QList<Token> tokenize();

private:
    void processSource();
    void processDigits(QString & current);
    void processWords(QString & current);
    void reportError(Error e);

    QMap<QString, Lexeme> symbols;
    QMap<QString, Lexeme> words;
    QList<Token> tokens;
    Source * src;

    bool continueDespiteErrors;
};

#endif // LEXER_H
