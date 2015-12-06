/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
    SyntaxHighlighter(QTextDocument * parent);
    void highlightBlock(const QString & text);
private:
    void setFormatForPatternInText(const QTextCharFormat & format,
                                   const QString & pattern,
                                   const QString & text);
};

#endif // SYNTAXHIGHLIGHTER_H
