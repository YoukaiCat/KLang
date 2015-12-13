/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef NODE_H
#define NODE_H

#include "KLang_global.h"

#include <memory>

#include <QList>

#include "Token.h"

using std::shared_ptr;

class KLANG_LIBRARY_EXPORT Node
{
public:
    Node();
    Node(const Token & token);

    Node * addChild(const shared_ptr<Node> node);
    Node * addChildren(const QList<shared_ptr<Node>> & nodes);

    const shared_ptr<Node> at(int index) const;
    const Token & getToken() const;
    const QList<shared_ptr<Node>> & getChildren() const;

    QString inspect(int level = 1) const;

private:
    Token token;
    QList<shared_ptr<Node>> children;
};

#endif // NODE_H
