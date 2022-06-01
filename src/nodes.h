#ifndef BRAINFUCK_NODES_H
#define BRAINFUCK_NODES_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum {
    OPERATION_NODE,
    SEQUENCE_NODE,
    LOOP_NODE
} NodeType;

typedef enum {
    INC_VALUE,
    DEC_VALUE,
    INC_PTR,
    DEC_PTR,
    PRINT,
    READ
} OperationNode;

typedef struct Node Node;
typedef struct {
    Node *nodes;
    size_t size;
    size_t capacity;
} SequenceNode;

typedef struct {
    SequenceNode sequence;
} LoopNode;

struct Node {
    NodeType type;
    union {
        OperationNode operation;
        SequenceNode sequence;
        LoopNode loop;
    };
};

Node create_sequence_node();
Node create_operation_node(OperationNode type);
Node create_loop_node(SequenceNode sequenceNode);
void sequence_push_node(SequenceNode *sequenceNode, Node node);
void free_sequence_node(SequenceNode sequenceNode);

Node build_ast(FILE *source_file, bool is_loop);

#endif //BRAINFUCK_NODES_H
