#ifndef BRAINFUCK_VISITORS_H
#define BRAINFUCK_VISITORS_H

#include "nodes.h"

typedef enum {
    PYTHON
} VisitorType;

typedef struct {
    unsigned int ident;
} PythonVisitor;

typedef struct {
    VisitorType type;
    union {
        PythonVisitor python;
    };
} Visitor;

void visit(Visitor visitor, SequenceNode node);
void visit_node(Visitor visitor, Node node);

#endif //BRAINFUCK_VISITORS_H
