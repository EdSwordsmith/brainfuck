#include <string.h>

#include "visitors.h"
#include "nodes.h"

typedef struct {
    char *input_file;
    char *out_file;
} args_t;

void parse_args(int argc, char **argv) {
    enum { INPUT_FILE, OUTPUT_FILE, TARGET } current_arg = INPUT_FILE;

    for (int i = 1; i < argc; ++i) {
        switch (current_arg) {
            case INPUT_FILE:
                if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--target") == 0) {
                    current_arg = TARGET;
                } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
                    current_arg = OUTPUT_FILE;
                } else {
                    if (argv[i][0] == '-') {
                        fprintf(stderr, "Invalid input file name\n");
                        exit(EXIT_FAILURE);
                    }
                    printf("Input file: %s\n", argv[i]);
                }
                break;
            case OUTPUT_FILE:
                if (argv[i][0] == '-') {
                    fprintf(stderr, "Invalid output file name\n");
                    exit(EXIT_FAILURE);
                }
                printf("Output file: %s\n", argv[i]);
                current_arg = INPUT_FILE;
                break;
            case TARGET:
                printf("Target: %s\n", argv[i]);
                current_arg = INPUT_FILE;
                break;
        }
    }
}

int main(int argc, char **argv) {
    parse_args(argc, argv);
    return 0;

    Visitor visitor;
    visitor.type = PYTHON;
    visitor.python.ident = 0;

    Node program = build_ast(false);

    visit(visitor, program.sequence);
    free_sequence_node(program.sequence);

    return 0;
}
