#include "compiler.h"
#include "token.h"
#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

HorizonLexer *horizon_alloc_lexer(const char *srcfile, char *srcdata,
                                  int srcsize, int srclength, char index,
                                  int position)
{
    HorizonLexer *lexer = calloc(1, sizeof(HorizonLexer));

    if (!lexer)
    {
        compiler_error("Lexer failed to load");
    }

    lexer->srcfile   = srcfile;
    lexer->srcdata   = srcdata;
    lexer->srcsize   = srcsize;
    lexer->srclength = srclength;
    lexer->position  = lexer->position;
    lexer->index     = lexer->srcdata[lexer->position];

    return lexer;
}

void horizon_dealloc_lexer(HorizonLexer *lexer)
{
    if (!lexer)
    {
        return;
    }

    free(lexer);
}

void horizon_lexer_advance(HorizonLexer *lexer, int z)
{
    if (!lexer)
    {
        return;
    }

    if ((lexer->position + z) > lexer->srclength)
    {
        return;
    }

    lexer->position = lexer->position + z;
    lexer->index    = lexer->srcdata[lexer->position];
}

void horizon_lexer_devance(HorizonLexer *lexer, int z)
{
    if (!lexer)
    {
        return;
    }

    if ((lexer->position - z) < 0)
    {
        return;
    }

    lexer->position = lexer->position - z;
    lexer->index    = lexer->srcdata[lexer->position];
}

char horizon_lexer_peek(HorizonLexer *lexer, int z)
{
    if (!lexer)
    {
        return '\0';
    }

    if (z > lexer->srclength || z < 0)
    {
        return '\0';
    }

    return lexer->srcdata[lexer->position + z];
}

void horizon_lexer_skip(HorizonLexer *lexer)
{
    if (!lexer)
    {
        return;
    }

    while (horizon_lexer_peek(lexer, 0) == '\n' ||
           horizon_lexer_peek(lexer, 0) == '\t' ||
           horizon_lexer_peek(lexer, 0) == ' ')
    {
        horizon_lexer_advance(lexer, 1);
    }
}

HorizonToken *horizon_lexer_next_token(HorizonLexer *lexer)
{
    if (!lexer)
    {
        compiler_error("Failed to get next token in lexer");
    }

    horizon_lexer_skip(lexer);

    if (lexer->index == '\0')
    {
        return horizon_alloc_token(TOKEN_EOF, '\0');
    }

    char *identifier = calloc(1, sizeof(char));
    char *digit      = calloc(1, sizeof(char));

    /* Alpha character */

    if (isalpha(lexer->index))
    {
        while (isalpha(horizon_lexer_peek(lexer, 0)))
        {
            identifier = realloc(identifier, strlen(identifier) + 2);

            strcat(identifier, (char[]) {lexer->index, 0});

            horizon_lexer_advance(lexer, 1);
        }

        return horizon_alloc_token(horizon_token_get_type(identifier), identifier);
    }

    /* Digit character */

    else if (isdigit(lexer->index))
    {
        while (isdigit(horizon_lexer_peek(lexer, 0)))
        {
            digit = realloc(digit, strlen(digit) + 2);

            strcat(digit, (char[]) {lexer->index, 0});

            horizon_lexer_advance(lexer, 1);
        }

        return horizon_alloc_token(horizon_token_get_type(digit), identifier);
    }

    /* Operator */

    else
    {
        char *buffer = calloc(1, sizeof(char));

        if (lexer->index == '"')
        {
            /* Value coming up */

            horizon_lexer_advance(lexer, 1);

            while (lexer->index != '"')
            {
                buffer = realloc(buffer, strlen(buffer) + 2);

                strcat(buffer, (char[]) {lexer->index, 0});

                horizon_lexer_advance(lexer, 1);
            }

            horizon_lexer_advance(lexer, 1);

            return horizon_alloc_token(TOKEN_VALUE, buffer);
        }
        else if (lexer->index == '=')
        {
            horizon_lexer_advance(lexer, 2);

            while (lexer->index != ';')
            {
                if (lexer->index != '"')
                {
                    buffer = realloc(buffer, strlen(buffer) + 2);

                    strcat(buffer, (char[]) {lexer->index, 0});
                }

                horizon_lexer_advance(lexer, 1);
            }
            
            horizon_lexer_advance(lexer, 1);

            return horizon_alloc_token(TOKEN_VALUE, buffer);
        }

        buffer = realloc(buffer, strlen(buffer) + 2);

        strcat(buffer, (char[]) {lexer->index, 0});

        horizon_lexer_advance(lexer, 1);

        return horizon_alloc_token(TOKEN_OPERATOR, buffer);
    }

    return horizon_alloc_token(TOKEN_EOF, "");
}