#ifndef HORIZON_H
#define HORIZON_H

#include "codegen.h"
#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "ast.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct HorizonCompiler
{
    /* Container Structure */
    /*
    Lexer
    Parser
    AST
    CodeGen
    */

    const char *srcname;
    
    char *srcdata;

    int srcsize;
    int srclength;

    HorizonLexer *lexer;
    HorizonParser *parser;
    HorizonASTRootNode *ast;
    HorizonCodeGen *codegen;
    HorizonToken **tokens;
} HorizonCompiler;

HorizonCompiler *compiler_bootup(const char *filename);

int compiler_compile(HorizonCompiler *compiler);

#endif