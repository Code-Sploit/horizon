#include "compiler.h"
#include "parser.h"
#include "ast.h"

#include <stdlib.h>
#include <string.h>

HorizonParser *horizon_alloc_parser()
{
    HorizonParser *parser = calloc(1, sizeof(HorizonParser));

    if (!parser)
    {
        compiler_error("Failed to allocate parser");
    }

    return parser;
}

void horizon_dealloc_parser(HorizonParser *parser)
{
    if (!parser)
    {
        return;
    }

    free(parser);
}

void horizon_parser_set_tokens(HorizonParser *parser, HorizonToken **tokens, int token_count)
{
    if (!parser || !tokens || token_count == 0)
    {
        return;
    }

    parser->token_count = token_count;
    parser->tokens = calloc(parser->token_count, sizeof(HorizonToken *));

    for (int i = 0; i < parser->token_count; i++)
    {
        parser->tokens[i] = tokens[i];
    }
}

void horizon_parser_run(HorizonParser *parser, HorizonASTRootNode *ast)
{
    if (!parser)
    {
        return;
    }

    const char *current_function = "";

    int a = 1;

    for (int i = 0; i < parser->token_count; i += a)
    {
        HorizonToken *token = parser->tokens[i];

        if (!token)
        {
            return;
        }

        //printf("Value: %s\n", token->value);

        a = 1;

        switch (token->type)
        {
            case TOKEN_KEYWORD:
            {
                /* int/str/require/return */

                if (strcmp(token->value, "int") == 0)
                {
                    /* Can either be function or variable definition */

                    if (strcmp(parser->tokens[i + 2]->value, "(") == 0)
                    {
                        /* Function */

                        HorizonToken *name_token  = parser->tokens[i + 1];

                        a += 4;

                        if (!name_token)
                        {
                            return;
                        }

                        /* Add to the AST tree */

                        ast->node_count++;

                        ast->nodes = realloc(ast->nodes, ast->node_count * sizeof(HorizonASTNode *));

                        ast->nodes[ast->node_count - 1] = horizon_alloc_ast_function(name_token->value);
                    }
                    else
                    {
                        /* Variable */

                        HorizonToken *name_token  = parser->tokens[i + 1];
                        HorizonToken *value_token = parser->tokens[i + 2];

                        a += 2;

                        if (!name_token || !value_token)
                        {
                            return;
                        }

                        /* Add to the AST tree */

                        ast->node_count++;

                        ast->nodes = realloc(ast->nodes, ast->node_count * sizeof(HorizonASTNode *));

                        ast->nodes[ast->node_count - 1] = horizon_alloc_ast_declaration(name_token->value, value_token->value, "int");
                    }
                }
                else if (strcmp(token->value, "str") == 0)
                {
                    /* Can either be function or variable definition */

                    if (strcmp(parser->tokens[i + 2]->value, "(") == 0)
                    {
                        /* Function */

                        HorizonToken *name_token  = parser->tokens[i + 1];

                        a += 3;

                        if (!name_token)
                        {
                            return;
                        }

                        /* Add to the AST tree */

                        ast->node_count++;

                        ast->nodes = realloc(ast->nodes, ast->node_count * sizeof(HorizonASTNode *));

                        ast->nodes[ast->node_count - 1] = horizon_alloc_ast_function(name_token->value);
                    }
                    else
                    {
                        /* Variable */

                        HorizonToken *name_token  = parser->tokens[i + 1];
                        HorizonToken *value_token = parser->tokens[i + 2];

                        a += 3;

                        if (!name_token || !value_token)
                        {
                            return;
                        }

                        /* Add to the AST tree */

                        ast->node_count++;

                        ast->nodes = realloc(ast->nodes, ast->node_count * sizeof(HorizonASTNode *));

                        ast->nodes[ast->node_count - 1] = horizon_alloc_ast_declaration(name_token->value, value_token->value, "str");
                    }
                }
                else if (strcmp(token->value, "require") == 0)
                {
                    /* Require library */

                    HorizonToken *library_token = parser->tokens[i + 1];

                    a += 1;

                    if (!library_token)
                    {
                        return;
                    }

                    /* Add to the AST tree */

                    ast->node_count++;

                    ast->nodes = realloc(ast->nodes, ast->node_count * sizeof(HorizonASTNode *));

                    ast->nodes[ast->node_count - 1] = horizon_alloc_ast_require(library_token->value);
                }
                else if (strcmp(token->value, "return") == 0)
                {
                    /* Return from, to */
                    /* TODO: Implement multiple functions */

                    HorizonToken *value_token = parser->tokens[i + 1];

                    a += 1;

                    if (!value_token)
                    {
                        return;
                    }

                    /* Add to the AST tree that we want to exit */

                    ast->node_count++;

                    ast->nodes = realloc(ast->nodes, ast->node_count * sizeof(HorizonASTNode *));

                    ast->nodes[ast->node_count - 1] = horizon_alloc_ast_exit(value_token->value);
                }
                else
                {
                    compiler_error("Invalid keyword");
                }
            }

            case TOKEN_VARIABLE:
            {
                /* Nothing to do here */
            }

            case TOKEN_OPERATOR:
            {
                /* Nothing to do here */
            }

            case TOKEN_IDENTIFIER:
            {
                /* That's the name of something so nothing to do */
            }

            case TOKEN_VALUE:
            {
                /* Value of an identifier */
            }
            
            case TOKEN_NOOP: {}
            case TOKEN_EOF:  {}
        }
    }
}