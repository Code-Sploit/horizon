#include "compiler.h"
#include "ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

HorizonASTRootNode *horizon_alloc_ast_root_node()
{
    HorizonASTRootNode *root = calloc(1, sizeof(HorizonASTRootNode));

    if (!root)
    {
        compiler_error("Failed to allocate root AST node");
    }

    root->node_count = 0;

    return root;
}

HorizonASTNode *horizon_alloc_ast_function(char *fname)
{
    HorizonASTNode *node = calloc(1, sizeof(HorizonASTNode));

    if (!node)
    {
        compiler_error("Failed to allocate AST node");
    }

    node->type = NODE_TYPE_FUNCTION;

    node->function.fname = fname;

    return node;
}

HorizonASTNode *horizon_alloc_ast_ret(char *from, char *to)
{
    HorizonASTNode *node = calloc(1, sizeof(HorizonASTNode));

    if (!node)
    {
        compiler_error("Failed to allocate AST node");
    }

    node->type = NODE_TYPE_RETURN;

    node->ret.from = from;
    node->ret.to   = to;

    return node;
}

HorizonASTNode *horizon_alloc_ast_declaration(char *name, char *value, char *type)
{
    HorizonASTNode *node = calloc(1, sizeof(HorizonASTNode));

    if (!node)
    {
        compiler_error("Failed to allocate AST node");
    }

    node->type = NODE_TYPE_DECLARATION;

    node->declaration.name  = name;
    node->declaration.value = value;
    node->declaration.type  = type;

    return node;
}

HorizonASTNode *horizon_alloc_ast_call(char *target)
{
    HorizonASTNode *node = calloc(1, sizeof(HorizonASTNode));

    if (!node)
    {
        compiler_error("Failed to allocate AST node");
    }

    node->type = NODE_TYPE_CALL;

    node->call.target = target;

    return node;
}

HorizonASTNode *horizon_alloc_ast_require(char *library)
{
    HorizonASTNode *node = calloc(1, sizeof(HorizonASTNode));

    if (!node)
    {
        compiler_error("Failed to allocate AST node");
    }

    node->type = NODE_TYPE_REQUIRE;

    node->require.library = library;

    return node;
}

HorizonASTNode *horizon_alloc_ast_exit(char *value)
{
    HorizonASTNode *node = calloc(1, sizeof(HorizonASTNode));

    if (!node)
    {
        compiler_error("Failed to allocate AST node");
    }

    node->type = NODE_TYPE_EXIT;

    node->exit.value = value;

    return node;
}

void horizon_dealloc_ast_node(HorizonASTNode *node)
{
    if (!node)
    {
        return;
    }

    free(node);
}

char *horizon_ast_node_to_template(HorizonASTNode *node)
{
    if (!node)
    {
        return "";
    }

    switch (node->type)
    {
        case NODE_TYPE_FUNCTION:
        {
            const char *template = "fname: `%s`";
            
            char *buffer;

            sprintf(buffer, template, node->function.fname);

            return buffer;
        }

        case NODE_TYPE_RETURN:
        {
            const char *template = "from: `%s` to: `%s`";

            char *buffer;

            sprintf(buffer, template, node->ret.from, node->ret.to);

            return buffer;
        }

        case NODE_TYPE_DECLARATION:
        {
            const char *template = "name: `%s` value: `%s` type: `%s`";

            char *buffer;

            sprintf(buffer, template, node->declaration.name, node->declaration.value, node->declaration.type);

            return buffer;
        }

        case NODE_TYPE_CALL:
        {
            const char *template = "target: `%s`";

            char *buffer;

            sprintf(buffer, template, node->call.target);

            return buffer;
        }

        case NODE_TYPE_REQUIRE:
        {
            const char *template = "library: `%s`";

            char *buffer;
            
            sprintf(buffer, template, node->require.library);

            return buffer;
        }

        case NODE_TYPE_EXIT:
        {
            const char *template = "value: `%s`";

            char *buffer;

            sprintf(buffer, template, node->exit.value);

            return buffer;
        }

        case NODE_TYPE_OPERATION:
        {

        }

        case NODE_TYPE_NOOP:
        {

        }
    }

    return "";
}