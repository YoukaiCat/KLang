/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include "Lexeme.h"

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
    SyntaxHighlighter(QTextDocument * parent);
    void highlightBlock(const QString & text);
private:
    QTextCharFormat getFormatForLexemeType(const Lexeme & lexeme) const;
};

#endif // SYNTAXHIGHLIGHTER_H
