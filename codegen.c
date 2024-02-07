#include "compiler.h"
#include "codegen.h"
#include "ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

HorizonCodeGen *horizon_alloc_codegen()
{
    HorizonCodeGen *codegen = calloc(1, sizeof(HorizonCodeGen));

    if (!codegen)
    {
        compiler_error("CodeGen: Failed to allocate codegen object");

        return NULL;
    }

    codegen->data_segments = calloc(1, sizeof(HorizonCodeGenDataSeg *));
    codegen->main_segment  = horizon_alloc_codegen_main_seg("");

    return codegen;
}

HorizonCodeGenDataSeg *horizon_alloc_codegen_data_seg(int type, char *name, char *data, int size)
{
    HorizonCodeGenDataSeg *data_seg = calloc(1, sizeof(HorizonCodeGenDataSeg));

    if (!data_seg)
    {
        compiler_error("CodeGen: Failed to allocate data_segment object");

        return NULL;
    }

    data_seg->type = type;
    data_seg->name = name;
    data_seg->data = data;
    data_seg->size = size;

    return data_seg;
}

HorizonCodeGenMainSeg *horizon_alloc_codegen_main_seg(char *code)
{
    HorizonCodeGenMainSeg *main_seg = calloc(1, sizeof(HorizonCodeGenMainSeg));

    if (!main_seg)
    {
        compiler_error("CodeGen: Failed to allocate main_segment object");
        
        return NULL;
    }

    main_seg->code = code;
    main_seg->size = 0;

    return main_seg;
}

void horizon_codegen_run(HorizonCodeGen *codegen, HorizonASTRootNode *root)
{
    if (!codegen || !root)
    {
        compiler_error("CodeGen: Failed to generate machine code because of NULL object(s)");
        
        return;
    }

    codegen->count = 0;

    /* First define the data section */

    for (int i = 0; i < root->node_count; i++)
    {
        if (root->nodes[i]->type == NODE_TYPE_DECLARATION)
        {
            /* Add to data segment */

            codegen->count++;

            codegen->data_segments = realloc(codegen->data_segments,
                                             codegen->count * sizeof(HorizonCodeGenDataSeg *));

            codegen->data_segments[codegen->count - 1] = horizon_alloc_codegen_data_seg(DATA_RW,
                                                                                         root->nodes[i]->declaration.name,
                                                                                         root->nodes[i]->declaration.value,
                                                                                         strlen(root->nodes[i]->declaration.value));
        }
        else
        {
            continue;
        }
    }

    /* TOOD: Remove iterated nodes from AST tree */

    /* Second define the main function */

    for (int i = 0; i < root->node_count; i++)
    {
        if (root->nodes[i]->type == NODE_TYPE_FUNCTION)
        {
            /* TODO: Add support for multiple functions beside _start/main */

            if (strcmp(root->nodes[i]->function.fname, "main") == 0)
            {
                for (int j = i; j < root->node_count; j++)
                {
                    codegen->main_segment->size = 0;
                    codegen->main_segment->code = calloc(1, sizeof(char));

                    if (root->nodes[j]->type == NODE_TYPE_EXIT)
                    {
                        const char *template = "_start:\n\tmov rax, 60\n\tmov rdi, %s\n\tsyscall";

                        char *buffer;

                        sprintf(buffer, template, root->nodes[j]->exit.value);

                        codegen->main_segment->size += strlen(buffer) + 2;
                        codegen->main_segment->code = realloc(codegen->main_segment->code, codegen->main_segment->size);

                        strcat(codegen->main_segment->code, buffer);

                        break;
                    }
                }
            }
            else
            {
                continue;
            }
        }
    }

    /* Generate final code */

    char *final_code = calloc(1, sizeof(char));

    /* Add globals */

    char *globals = "global _start\n\nsection .text\n\n";

    final_code = realloc(final_code, strlen(globals) + strlen(final_code) + 2);

    strcat(final_code, globals);

    /* Add main function code */

    final_code = realloc(final_code, strlen(codegen->main_segment->code) + strlen(final_code) + 2);

    strcat(final_code, codegen->main_segment->code);

    /* Add data segment identifier */

    char *data_iden = "\n\nsection .data\n";

    final_code = realloc(final_code, strlen(data_iden) + strlen(final_code) + 2);

    strcat(final_code, data_iden);

    /* Add data segment code */
    for (int i = 0; i < codegen->count; i++)
    {
        const char *template = "\t%s: db \"%s\", %d\n\t_%s: equ $ - %s\n";

        char *buffer = malloc(1024);

        HorizonCodeGenDataSeg *segment = codegen->data_segments[i];
       
        sprintf(buffer, template, segment->name, segment->data, segment->size, segment->name, segment->name);

        final_code = realloc(final_code, strlen(final_code) + strlen(buffer) + 2);

        strcat(final_code, buffer);
    }

    final_code = realloc(final_code, strlen(final_code) + 2);

    strcat(final_code, (char[]) {'\n', 0});

    codegen->code = final_code;
}