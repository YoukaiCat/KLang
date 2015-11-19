/**
 * Copyright (C) 2015 VLadislav Mileshkin
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include "Node.h"

#include <QDebug>
#include <QUuid>
#include <QByteArray>

Node::Node(const Token & token)
    : token(token)
{}

Node & Node::addChild(const Node & node)
{
    children.append(node);
    return *this;
}

Node & Node::addChildren(const QList<Node> & nodes)
{
    children.append(nodes);
    return *this;
}

const Node & Node::at(int index) const
{
    return children.at(index);
}

const Token & Node::getToken() const
{
    return token;
}

const QList<Node> & Node::getChildren() const
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
        str += space + children.at(i).inspect(level + 1);
    }
    return str;
}

QImage Node::printAsGraph() const
{
    char ** image = (char **) malloc(1 * sizeof(char *));
    uint len = 0;

    Agraph_t * graph;
    Agnode_t * root;
    GVC_t * gvc;

    gvc = gvContext();
    graph = agopen((char *)"Graph", Agdirected, 0);
    root = agnode(graph, (char *)"AST", 1);

    nextNode(graph, root);

    agsafeset(root, (char *)"color", (char *)"red", (char *)"");
    gvLayout(gvc, graph, (char *)"dot");
    gvRenderData(gvc, graph, (char *)"png", image, &len);
    gvFreeLayout(gvc, graph);
    agclose(graph);

    return QImage::fromData((uchar *)*image, len, "png");
}

void Node::nextNode(Agraph_t * graph, Agnode_t * parent) const
{
    Agnode_t * node = agnode(graph, QUuid::createUuid().toString().toLatin1().data(), 1);
    agset(node, (char *)"label", token.inspect().toLocal8Bit().data());
    agedge(graph, parent, node, 0, 1);
    for (auto child : children) {
        child.nextNode(graph, node);
    }
}
