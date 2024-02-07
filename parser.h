#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"

typedef struct HorizonParser
{
    HorizonToken **tokens;

    int token_count;
} HorizonParser;

HorizonParser *horizon_alloc_parser();

void horizon_dealloc_parser(HorizonParser *parser);
void horizon_parser_set_tokens(HorizonParser *parser, HorizonToken **tokens, int token_count);
void horizon_parser_run(HorizonParser *parser, HorizonASTRootNode *ast);

#endif