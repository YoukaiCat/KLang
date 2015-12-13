#ifndef ASTASGRAPHVISUALIZER_H
#define ASTASGRAPHVISUALIZER_H

#include <graphviz/gvc.h>

#include <QImage>

#include "Node.h"

class ASTGraphVisualizer
{
public:
    static QImage renderASTAsGraph(const shared_ptr<Node> ast);
private:
    static void nextNode(const shared_ptr<Node> astNode, Agraph_t * graph, Agnode_t * parent);
};

#endif // ASTASGRAPHVISUALIZER_H
