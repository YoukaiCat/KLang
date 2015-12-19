/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef PARSER_H
#define PARSER_H

#include "KLang_global.h"

#include <memory>

#include <QObject>
#include <QStack>

#include "Token.h"
#include "Node.h"
#include "Lexeme.h"

using std::shared_ptr;

class KLANG_LIBRARY_EXPORT Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(const shared_ptr<QList<Token>> tokens, QObject * parent = 0);
    shared_ptr<Node> parse();

private:
    shared_ptr<Node> begin();
    shared_ptr<Node> declarations();
    shared_ptr<Node> processDeclarations();
    shared_ptr<Node> declareId();
    shared_ptr<QList<shared_ptr<Node>>> declareIds();
    shared_ptr<Node> assignments();
    shared_ptr<Node> createAssignment();
    shared_ptr<Node> unaryMinus();
    shared_ptr<Node> additiveExpression();
    shared_ptr<Node> multiplicativeExpression();
    shared_ptr<Node> booleanBinaryExpression();
    shared_ptr<Node> notExpression();
    shared_ptr<Node> base();
    shared_ptr<Node> end();
    QString tokenToString(const Token & token);

    Lexeme lastDeclaredType;
    shared_ptr<QList<Token>> tokens;
    QStack<int> parenthesisIndices;
};

#endif // PARSER_H
