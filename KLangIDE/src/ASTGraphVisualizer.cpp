#include "include/ASTGraphVisualizer.h"

#include <QDebug>
#include <QUuid>
#include <QByteArray>

QImage ASTGraphVisualizer::renderASTAsGraph(const Node & astRoot)
{
    char ** image = (char **) malloc(1 * sizeof(char *));
    uint len = 0;

    Agraph_t * graph;
    Agnode_t * root;
    GVC_t * gvc;

    gvc = gvContext();
    graph = agopen((char *)"Graph", Agdirected, 0);
    agset(graph, (char *)"charset", (char *)"UTF-8");

    root = agnode(graph, (char *)"AST", 1);
    agsafeset(root, (char *)"shape", (char *)"box", (char *)"");
    agsafeset(root, (char *)"fontsize", (char *)"8", (char *)"");

    nextNode(astRoot, graph, root);

    agsafeset(root, (char *)"color", (char *)"red", (char *)"");
    gvLayout(gvc, graph, (char *)"dot");
    gvRenderData(gvc, graph, (char *)"png", image, &len);
    gvFreeLayout(gvc, graph);
    agclose(graph);

    return QImage::fromData((uchar *)*image, len, "png");
}

void ASTGraphVisualizer::nextNode(const Node & astNode, Agraph_t * graph, Agnode_t * parent)
{
    Agnode_t * node = agnode(graph, QUuid::createUuid().toString().toLatin1().data(), 1);
    agset(node, (char *)"label", astNode.getToken().inspect().toLocal8Bit().data());
    agsafeset(node, (char *)"shape", (char *)"box", (char *)"");
    agsafeset(node, (char *)"fontsize", (char *)"10", (char *)"");
    agsafeset(node, (char *)"margin", (char *)"0.3,0.005", (char *)"");
    agedge(graph, parent, node, 0, 1);
    for (auto child : astNode.getChildren()) {
        nextNode(child, graph, node);
    }
}
