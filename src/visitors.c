#include <stdio.h>
#include "visitors.h"

void visit(FILE *output, Visitor visitor, SequenceNode node) {
    switch (visitor.type) {
        case PYTHON:
            fprintf(output, "#!/usr/bin/python3\n");
            fprintf(output, "tape = {}\n");
            fprintf(output, "ptr = 0\n\n");
            fprintf(output, "current_input = ''\n\n");
            fprintf(output, "def inc_value():\n");
            fprintf(output, "    if ptr not in tape:\n");
            fprintf(output, "        tape[ptr] = 0\n");
            fprintf(output, "    tape[ptr] += 1\n\n");
            fprintf(output, "def dec_value():\n");
            fprintf(output, "    if ptr not in tape:\n");
            fprintf(output, "        tape[ptr] = 0\n");
            fprintf(output, "    tape[ptr] -= 1\n\n");
            fprintf(output, "def read_stdin():\n");
            fprintf(output, "    global current_input\n");
            fprintf(output, "    if len(current_input) == 0:\n");
            fprintf(output, "        current_input = input()\n\n");
            fprintf(output, "    if len(current_input) > 0:\n");
            fprintf(output, "        tape[ptr] = ord(current_input[0])\n");
            fprintf(output, "        current_input = current_input[1:]\n\n");
            fprintf(output, "    else:\n");
            fprintf(output, "        tape[ptr] = 0\n\n");
            fprintf(output, "def should_loop():\n");
            fprintf(output, "    if ptr not in tape:\n");
            fprintf(output, "        return False\n");
            fprintf(output, "    return tape[ptr] != 0\n\n");

            break;
    }

    for (int i = 0; i < node.size; ++i) {
        visit_node(output, visitor, node.nodes[i]);
    }
}

void visit_node(FILE *output, Visitor visitor, Node node) {
    switch (visitor.type) {
        case PYTHON:
            for (int i = 0; i < visitor.python.ident; ++i) {
                fprintf(output, "    ");
            }
            
            switch (node.type) {
                case OPERATION_NODE:
                    switch (node.operation) {
                        case INC_VALUE:
                            fprintf(output, "inc_value()\n");
                            break;
                        case DEC_VALUE:
                            fprintf(output, "dec_value()\n");
                            break;
                        case INC_PTR:
                            fprintf(output, "ptr += 1\n");
                            break;
                        case DEC_PTR:
                            fprintf(output, "ptr -= 1\n");
                            break;
                        case PRINT:
                            fprintf(output, "print(chr(tape[ptr]), end='')\n");
                            break;
                        case READ:
                            fprintf(output, "read_stdin()\n");
                            break;
                    }
                    break;
                case LOOP_NODE:
                    fprintf(output, "while should_loop():\n");
                    visitor.python.ident++;

                    for (size_t i = 0; i < node.loop.sequence.size; ++i) {
                        visit_node(output, visitor, node.loop.sequence.nodes[i]);
                    }

                    // Make sure there aren't empty while loops
                    if (node.loop.sequence.size == 0) {
                        for (int i = 0; i < visitor.python.ident; ++i) {
                            fprintf(output, "    ");
                        }
                        fprintf(output, "pass\n");
                    }


                    visitor.python.ident--;
                    break;
                case SEQUENCE_NODE:
                    break;
            }
            break;
    }
}
