/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "SyntaxHighlighter.h"

#include "Token.h"
#include "Lexer.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument * parent)
    : QSyntaxHighlighter(parent)
{}

void SyntaxHighlighter::highlightBlock(const QString & text)
{
    Lexer l(text, true);
    for (auto token : *l.tokenize()) {
        auto length = token.getIndexEnd() - token.getIndexBegin();
        auto format = getFormatForLexemeType(token.getType());
        setFormat(token.getIndexBegin(), length, format);
    }
}

QTextCharFormat SyntaxHighlighter::getFormatForLexemeType(const Lexeme & lexeme) const
{
    QTextCharFormat format;
    format.setFontWeight(QFont::Bold);
    switch (lexeme) {
        case Lexeme::Begin:
        case Lexeme::SingleDeclaration:
        case Lexeme::MultipleDeclaration:
        case Lexeme::End:
            format.setForeground(Qt::darkCyan);
            break;
        case Lexeme::LeftParentheses:
        case Lexeme::RightParentheses:
        case Lexeme::Equality:
        case Lexeme::Semicolon:
        case Lexeme::Comma:
            format.setForeground(Qt::darkGreen);
            break;
        case Lexeme::Plus:
        case Lexeme::UnaryMinus:
        case Lexeme::Minus:
        case Lexeme::Multiply:
        case Lexeme::Divide:
        case Lexeme::And:
        case Lexeme::Or:
        case Lexeme::Not:
            format.setForeground(Qt::darkRed);
            break;
        case Lexeme::Id:
            format.setForeground(Qt::darkBlue);
            break;
        case Lexeme::Num:
            format.setForeground(Qt::darkGray);
            break;
        default:
            format.setFontWeight(QFont::Normal);
            break;
    }
    return format;
}
