/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef NODE_H
#define NODE_H

#include <QList>
#include <QImage>

#include "Lexeme.h"
#include "Token.h"

#include <graphviz/gvc.h>

class Node
{
public:
    Node(const Token & token);

    Node & addChild(const Node & node);
    Node & addChildren(const QList<Node> & nodes);

    const Node & at(int index) const;
    const Token & getToken() const;
    const QList<Node> & getChildren() const;

    QString inspect(int level = 1) const;
    QImage printAsGraph() const;

private:
    void nextNode(Agraph_t * graph, Agnode_t * parent) const;

    Token token;
    QList<Node> children;
};

#endif // NODE_H
