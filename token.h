#ifndef TOKEN_H
#define TOKEN_H

typedef enum HorizonTokenType
{
    TOKEN_KEYWORD,
    TOKEN_VARIABLE,
    TOKEN_OPERATOR,
    TOKEN_IDENTIFIER,
    TOKEN_VALUE,
    TOKEN_NOOP,
    TOKEN_EOF
} HorizonTokenType;

typedef struct HorizonToken
{
    HorizonTokenType type;

    const char *value;
} HorizonToken;

HorizonToken *horizon_alloc_token(HorizonTokenType type, const char *value);

HorizonTokenType horizon_token_get_type(char *data);

char *horizon_token_id_to_string(HorizonTokenType type);

void horizon_dealloc_token(HorizonToken *token);

#endif