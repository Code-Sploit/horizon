#include "compiler.h"
#include "token.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

HorizonToken *horizon_alloc_token(HorizonTokenType type, const char *value)
{
    HorizonToken *token = calloc(1, sizeof(HorizonToken));

    if (!token)
    {
        compiler_error("Token: Failed to allocate token");
    }

    token->type  = type;
    token->value = value;

    return token;
}

void horizon_dealloc_token(HorizonToken *token)
{
    if (!token)
    {
        compiler_warning("Token: Failed to deallocate token because of NULL object");
        
        return;
    }

    free(token);
}

char *horizon_token_id_to_string(HorizonTokenType type)
{
    switch (type)
    {
        case TOKEN_KEYWORD:    {return "TOKEN_KEYWORD";}
        case TOKEN_VARIABLE:   {return "TOKEN_VARIABLE";}
        case TOKEN_OPERATOR:   {return "TOKEN_OPERATOR";}
        case TOKEN_IDENTIFIER: {return "TOKEN_IDENTIFIER";}
        case TOKEN_VALUE:      {return "TOKEN_VALUE";}
        case TOKEN_NOOP:       {return "TOKEN_NOOP";}
        case TOKEN_EOF:        {return "TOKEN_EOF";}
    }

    return "TOKEN_NOOP";
}

HorizonTokenType horizon_token_get_type(char *data)
{
    if (isalpha(data[0]) || data[0] == '_')
    {
        if (strcmp(data, "require") == 0)
        {
            return TOKEN_KEYWORD;
        }
        else if (strcmp(data, "return") == 0)
        {
            return TOKEN_KEYWORD;
        }
        else if (strcmp(data, "int") == 0)
        {
            return TOKEN_KEYWORD;
        }
        else if (strcmp(data, "Array") == 0)
        {
            return TOKEN_KEYWORD;
        }
        else if (strcmp(data, "str") == 0)
        {
            return TOKEN_KEYWORD;
        }
        else
        {
            return TOKEN_IDENTIFIER;
        }
    }
    else
    {
        return TOKEN_VARIABLE;
    }

    return TOKEN_NOOP;
}