/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef PARSER_H
#define PARSER_H

#include "KLang_global.h"

#include <QObject>
#include <QStack>

#include "Token.h"
#include "Node.h"

class KLANG_LIBRARY_EXPORT Parser : public QObject
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
    Node unaryMinus();
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
