#include "compiler.h"
#include "codegen.h"
#include "horizon.h"
#include "token.h"
#include "lexer.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

char *compiler_get_filedata(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        compiler_error("Failed to open: %s", filename);
    }

    char *__buffer = calloc(1, sizeof(char));
    char *__line = NULL;
    
    size_t length = 0;
    ssize_t read;

    while ((read = getline(&__line, &length, file)) != -1)
    {
        __buffer = realloc(__buffer, (strlen(__buffer) + strlen(__line) + 2));

        strcat(__buffer, __line);
    }

    fclose(file);

    return __buffer;
}

int compiler_get_filesize(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        compiler_debug("Failed to get file size: %s", filename);

        return 0;
    }

    fseek(file, 0L, SEEK_END);

    int __ret = ftell(file);

    fclose(file);

    return __ret;
}

HorizonCompiler *compiler_bootup(const char *filename)
{
    HorizonCompiler *compiler = calloc(1, sizeof(HorizonCompiler));

    if (!compiler)
    {
        compiler_error("Failed to bootup compiler");
    }

    compiler->srcname   = filename;
    compiler->srcdata   = compiler_get_filedata(compiler->srcname);
    compiler->srcsize   = compiler_get_filesize(compiler->srcname);
    compiler->srclength = strlen(compiler->srcdata);

    HorizonLexer *lexer = horizon_alloc_lexer(compiler->srcname, compiler->srcdata,
                                      compiler->srcsize, compiler->srclength,
                                      0, 0);

    compiler->lexer = lexer;

    compiler->tokens    = calloc(1, sizeof(HorizonToken *));
    compiler->tokens[0] = horizon_lexer_next_token(compiler->lexer);

    compiler->parser = horizon_alloc_parser();

    compiler->ast = horizon_alloc_ast_root_node();

    compiler->codegen = horizon_alloc_codegen();

    return compiler;
}

void horizon_compiler_write_asm(const char *srcname, HorizonCodeGen *codegen)
{
    char *asmfile = calloc(1, sizeof(char));

    int i = 0;

    while (srcname[i] != '.')
    {
        asmfile = realloc(asmfile, strlen(asmfile) + 2);

        strcat(asmfile, (char[]) {srcname[i], 0});

        i++;
    }

    asmfile = realloc(asmfile, strlen(asmfile) + strlen(".s") + 2);

    strcat(asmfile, ".s");

    FILE *file = fopen(asmfile, "w");

    if (!file)
    {
        return;
    }

    fputs(codegen->code, file);

    fclose(file);
}

int compiler_compile(HorizonCompiler *compiler)
{
    if (!compiler)
    {
        compiler_error("Compiler is NULL");
    }

    /* First convert srcdata into token array */

    HorizonToken *iterator = compiler->tokens[0];

    int i = 0;

    while (iterator->type != TOKEN_EOF)
    {
        //compiler_debug("Token received from lexer. Type: %s\t\tValue: %s", horizon_token_id_to_string(compiler->tokens[i]->type), compiler->tokens[i]->value);

        compiler->tokens = realloc(compiler->tokens, (i + 2) * sizeof(HorizonToken *));

        compiler->tokens[i + 1] = horizon_lexer_next_token(compiler->lexer);

        iterator = compiler->tokens[i + 1];

        i++;
    }

    horizon_parser_set_tokens(compiler->parser, compiler->tokens, i);
    horizon_parser_run(compiler->parser, compiler->ast);

    for (int i = 0; i < compiler->ast->node_count; i++)
    {
        //compiler_debug("Node Index: %d, Type: %d, Values -> %s", i, compiler->ast->nodes[i]->type, horizon_ast_node_to_template(compiler->ast->nodes[i]));
    }

    horizon_codegen_run(compiler->codegen, compiler->ast);

    horizon_compiler_write_asm(compiler->srcname, compiler->codegen);

    return 0;
}
