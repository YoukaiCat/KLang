/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Node.h"
#include "Lexeme.h"

Node::Node(const Token & token)
    : token(token)
{}

Node * Node::addChild(const shared_ptr<Node> node)
{
    children.append(node);
    return this;
}

Node * Node::addChildren(const QList<shared_ptr<Node>> & nodes)
{
    children.append(nodes);
    return this;
}

const shared_ptr<Node> Node::at(int index) const
{
    return children.at(index);
}

const Token & Node::getToken() const
{
    return token;
}

const QList<shared_ptr<Node>> & Node::getChildren() const
{
    return children;
}

//Няшно выводит в строку дерево, выделяя дочерние ноды отступом
QString Node::inspect(int level) const
{
    QString str;
    QString space;
    if (level < 1) {
        level = 1;
    }
    space.fill(' ', level * 2);
    str += space + token.inspect() + "\n";
    for (auto i = 0; i < children.size(); i++) {
        str += space + children.at(i)->inspect(level + 1);
    }
    return str;
}
