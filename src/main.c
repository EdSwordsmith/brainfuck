#include <string.h>

#include "visitors.h"
#include "nodes.h"

typedef struct {
    bool error;
    VisitorType type;
    char *source_file;
    char *output_file;
} args_t;

static PythonVisitor python_visitor = { .ident = 0 };

args_t parse_args(int argc, char **argv) {
    args_t args = { .type = PYTHON, .source_file = NULL, .output_file = NULL };

    if (argc < 2) {
        fprintf(stderr, "usage: %s source_file\n", argv[0]);
        fprintf(stderr, "       %s source_file output\n", argv[0]);
        fprintf(stderr, "       %s python source_file\n", argv[0]);
        fprintf(stderr, "       %s python source_file output\n", argv[0]);
        args.error = true;
    } else if (argc > 2 && strcmp(argv[1], "python") == 0) {
        args.source_file = argv[2];
        args.type = PYTHON;
        if (argc > 3)
            args.output_file = argv[3];
    } else if (argc > 2 && strcmp(argv[1], "c") == 0) {
        args.source_file = argv[2];
        args.type = C;
        if (argc > 3)
            args.output_file = argv[3];
    } else if (argc == 2) {
        // Default visitor and stdout
        args.source_file = argv[1];
    } else if (argc > 2) {
        // Default visitor and given output file
        args.source_file = argv[1];
        args.output_file = argv[2];
    }

    return args;
}

int main(int argc, char **argv) {
    args_t args = parse_args(argc, argv);
    if (args.error)
        return EXIT_FAILURE;

    Visitor visitor = { .type = args.type };
    switch (visitor.type) {
        case PYTHON:
            visitor.python = python_visitor;
            break;
        default:
            break;
    }

    // Parse source file and build the AST
    FILE *source_file = fopen(args.source_file, "r");
    if (source_file == NULL) {
        fprintf(stderr, "%s: %s: No such file\n", argv[0], args.source_file);
        return EXIT_FAILURE;
    }

    Node program = build_ast(source_file, false);
    fclose(source_file);

    // Visit the AST and generate output
    FILE *output = stdout;
    if (args.output_file != NULL) {
        output = fopen(args.output_file, "w");
    }

    if (output == NULL) {
        fprintf(stderr, "%s: Error generating output\n", argv[0]);
        return EXIT_FAILURE;
    }

    visit(output, visitor, program.sequence);
    free_sequence_node(program.sequence);
    if (output != stdout)
        fclose(output);

    return EXIT_SUCCESS;
}
