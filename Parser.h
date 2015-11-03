/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef PARSER_H
#define PARSER_H

#include "Lexeme.h"
#include "Source.h"
#include "Token.h"
#include "Node.h"
#include "Error.h"

#include <QStack>

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(const QList<Token> & tokens, QObject * parent = 0);
    Node parse();

private:
    Node begin();
    Node declarations();
    Node processDeclarations();
    Node declareId();
    QList<Node> declareIds();
    Node assignments();
    Node createAssignment();
    Node expression();
    Node additiveExpression();
    Node multiplicativeExpression();
    Node booleanBinaryExpression();
    Node notExpression();
    Node base();
    Node end();

    QList<Token> tokens;
    QStack<int> paranthesisIndices;
};

#endif // PARSER_H
