#include <stdio.h>
#include "visitors.h"

void visit(Visitor visitor, SequenceNode node) {
    switch (visitor.type) {
        case PYTHON:
            printf("#!/usr/bin/python3\n");
            printf("tape = {}\n");
            printf("ptr = 0\n\n");
            printf("current_input = ''\n\n");
            printf("def inc_value():\n");
            printf("    if ptr not in tape:\n");
            printf("        tape[ptr] = 0\n");
            printf("    tape[ptr] += 1\n\n");
            printf("def dec_value():\n");
            printf("    if ptr not in tape:\n");
            printf("        tape[ptr] = 0\n");
            printf("    tape[ptr] -= 1\n\n");
            printf("def read_stdin():\n");
            printf("    global current_input\n");
            printf("    if len(current_input) == 0:\n");
            printf("        current_input = input()\n\n");
            printf("    if len(current_input) > 0:\n");
            printf("        tape[ptr] = ord(current_input[0])\n");
            printf("        current_input = current_input[1:]\n\n");
            printf("    else:\n");
            printf("        tape[ptr] = 0\n\n");
            printf("def should_loop():\n");
            printf("    if ptr not in tape:\n");
            printf("        return False\n");
            printf("    return tape[ptr] != 0\n\n");

            break;
    }

    for (int i = 0; i < node.size; ++i) {
        visit_node(visitor, node.nodes[i]);
    }
}

void visit_node(Visitor visitor, Node node) {
    switch (visitor.type) {
        case PYTHON:
            for (int i = 0; i < visitor.python.ident; ++i) {
                printf("    ");
            }
            
            switch (node.type) {
                case OPERATION_NODE:
                    switch (node.operation) {
                        case INC_VALUE:
                            printf("inc_value()\n");
                            break;
                        case DEC_VALUE:
                            printf("dec_value()\n");
                            break;
                        case INC_PTR:
                            printf("ptr += 1\n");
                            break;
                        case DEC_PTR:
                            printf("ptr -= 1\n");
                            break;
                        case PRINT:
                            printf("print(chr(tape[ptr]), end='')\n");
                            break;
                        case READ:
                            printf("read_stdin()\n");
                            break;
                    }
                    break;
                case LOOP_NODE:
                    printf("while should_loop():\n");
                    visitor.python.ident++;

                    for (size_t i = 0; i < node.loop.sequence.size; ++i) {
                        visit_node(visitor, node.loop.sequence.nodes[i]);
                    }

                    // Make sure there aren't empty while loops
                    if (node.loop.sequence.size == 0) {
                        for (int i = 0; i < visitor.python.ident; ++i) {
                            printf("    ");
                        }
                        printf("pass\n");
                    }


                    visitor.python.ident--;
                    break;
                case SEQUENCE_NODE:
                    break;
            }
            break;
    }
}
