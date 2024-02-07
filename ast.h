#ifndef AST_H
#define AST_H

typedef struct HorizonASTNode
{
    enum
    {
        NODE_TYPE_FUNCTION,
        NODE_TYPE_RETURN,
        NODE_TYPE_DECLARATION,
        NODE_TYPE_CALL,
        NODE_TYPE_REQUIRE,
        NODE_TYPE_EXIT,
        NODE_TYPE_OPERATION,
        NODE_TYPE_NOOP
    } type;

    union
    {
        struct
        {
            char *fname;
        } function;

        struct
        {
            char *from;
            char *to;
        } ret;

        struct
        {
            char *name;
            char *value;
            char *type;
        } declaration;

        struct
        {
            char *target;
        } call;

        struct
        {
            char *library;
        } require;

        struct
        {
            char *value;
        } exit;
    };

    struct HorizonASTNode *left;
    struct HorizonASTNode *right;
} HorizonASTNode;

typedef struct HorizonASTRootNode
{
    HorizonASTNode **nodes;

    int node_count;
} HorizonASTRootNode;

HorizonASTRootNode *horizon_alloc_ast_root_node();

HorizonASTNode *horizon_alloc_ast_function(char *fname);
HorizonASTNode *horizon_alloc_ast_ret(char *from, char *to);
HorizonASTNode *horizon_alloc_ast_declaration(char *name, char *value, char *type);
HorizonASTNode *horizon_alloc_ast_call(char *target);
HorizonASTNode *horizon_alloc_ast_require(char *library);
HorizonASTNode *horizon_alloc_ast_exit(char *value);

void horizon_dealloc_ast_node(HorizonASTNode *node);

char *horizon_ast_node_to_template(HorizonASTNode *node);

#endif