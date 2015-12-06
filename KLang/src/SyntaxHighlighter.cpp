/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "SyntaxHighlighter.h"

#include "Lexer.h"
#include "Lexeme.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument * parent)
    : QSyntaxHighlighter(parent)
{}

//Простая подсветка на регулярках
//Сделать через лексер не получается,
//так как Qt не любит исключения и они не ловятся в этом методе.
//Нужно будет возвращать "неизвестные токены" и список ошибок.
void SyntaxHighlighter::highlightBlock(const QString & text)
{
    QTextCharFormat keywordsFormat;
    keywordsFormat.setFontWeight(QFont::Bold);
    keywordsFormat.setForeground(Qt::darkCyan);
    QString keywordsPattern = "\\b(Начало|Анализ|Синтез|Окончание)\\b";
    setFormatForPatternInText(keywordsFormat, keywordsPattern, text);

    QTextCharFormat numbersFormat;
    numbersFormat.setFontWeight(QFont::Bold);
    numbersFormat.setForeground(Qt::darkGray);
    QString numbersPattern = "\\b\\d+\\.\\d+\\b";
    setFormatForPatternInText(numbersFormat, numbersPattern, text);

    QTextCharFormat symbolsFormat;
    symbolsFormat.setFontWeight(QFont::Bold);
    symbolsFormat.setForeground(Qt::darkGreen);
    QString symbolsPattern = "(\\=|\\;|\\,|\\(|\\))";
    setFormatForPatternInText(symbolsFormat, symbolsPattern, text);

    QTextCharFormat operatorsFormat;
    operatorsFormat.setFontWeight(QFont::Bold);
    operatorsFormat.setForeground(Qt::darkRed);
    QString operatorsPattern = "(\\+|\\-|\\*|\\/|И|ИЛИ|НЕ)";
    setFormatForPatternInText(operatorsFormat, operatorsPattern, text);

    QTextCharFormat idsFormat;
    idsFormat.setFontWeight(QFont::Bold);
    idsFormat.setForeground(Qt::darkBlue);
    QString idsPattern = "\\b[А-Яа-я]\\d{0,3}\\b";
    setFormatForPatternInText(idsFormat, idsPattern, text);
}

void SyntaxHighlighter::setFormatForPatternInText(const QTextCharFormat & format,
                                                  const QString & pattern,
                                                  const QString & text)
{
    QRegExp expression(pattern);
    int index = text.indexOf(expression);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, format);
        index = text.indexOf(expression, index + length);
    }
}
