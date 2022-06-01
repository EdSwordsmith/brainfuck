#include <stdlib.h>
#include "nodes.h"

#define SEQUENCE_START_SIZE 8

Node create_sequence_node() {
    Node node;
    node.type = SEQUENCE_NODE;
    node.sequence.nodes = malloc(SEQUENCE_START_SIZE * sizeof(Node));
    node.sequence.size = 0;
    node.sequence.capacity = SEQUENCE_START_SIZE;
    return node;
}

Node create_operation_node(OperationNode type) {
    Node node;
    node.type = OPERATION_NODE;
    node.operation = type;
    return node;
}

Node create_loop_node(SequenceNode sequenceNode) {
    Node node;
    node.type = LOOP_NODE;
    node.loop.sequence = sequenceNode;
    return node;
}

void sequence_push_node(SequenceNode *sequenceNode, Node node) {
    if (sequenceNode->size == sequenceNode->capacity) {
        sequenceNode->nodes = realloc(sequenceNode->nodes, sizeof(Node) * sequenceNode->capacity * 2);
        sequenceNode->capacity *= 2;
    }

    sequenceNode->nodes[sequenceNode->size] = node;
    sequenceNode->size++;
}

void free_sequence_node(SequenceNode sequenceNode) {
    for (size_t i = 0; i < sequenceNode.size; i++) {
        if (sequenceNode.nodes[i].type == LOOP_NODE)
            free_sequence_node(sequenceNode.nodes[i].loop.sequence);
        else if (sequenceNode.nodes[i].type == SEQUENCE_NODE)
            free_sequence_node(sequenceNode.nodes[i].sequence);
    }

    free(sequenceNode.nodes);
}

Node build_ast(FILE *source_file, bool is_loop) {
    Node sequence = create_sequence_node();

    int c;
    while ((c = fgetc(source_file)) != EOF) {
        switch (c) {
            case '+':
                sequence_push_node(&sequence.sequence, create_operation_node(INC_VALUE));
                break;
            case '-':
                sequence_push_node(&sequence.sequence, create_operation_node(DEC_VALUE));
                break;
            case '>':
                sequence_push_node(&sequence.sequence, create_operation_node(INC_PTR));
                break;
            case '<':
                sequence_push_node(&sequence.sequence, create_operation_node(DEC_PTR));
                break;

            case '.':
                sequence_push_node(&sequence.sequence, create_operation_node(PRINT));
                break;
            case ',':
                sequence_push_node(&sequence.sequence, create_operation_node(READ));
                break;

            case '[':
                sequence_push_node(&sequence.sequence, create_loop_node(build_ast(source_file, true).sequence));
                break;

            case ']':
                if (!is_loop) {
                    fprintf(stderr, "Error: unexpected ']' outside a loop\n");
                    exit(EXIT_FAILURE);
                }
                return sequence;

            default:
                break;
        }
    }

    if (is_loop) {
        fprintf(stderr, "Error: expected ']' to close loop\n");
        exit(EXIT_FAILURE);
    }

    return sequence;
}
