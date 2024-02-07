#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef struct HorizonLexer
{
    /* srcfile   */
    const char *srcfile;

    /* srcdata   */
    char *srcdata;

    /* srcsize   */
    int srcsize;

    /* srclength */
    int srclength;

    /* index     */
    char index;

    /* position  */
    int position;
} HorizonLexer;

HorizonLexer *horizon_alloc_lexer(const char *srcfile, char *srcdata,
                                  int srcsize, int srclength, char index,
                                  int position);

void horizon_dealloc_lexer(HorizonLexer *lexer);

void horizon_lexer_advance(HorizonLexer *lexer, int z);
void horizon_lexer_devance(HorizonLexer *lexer, int z);

char horizon_lexer_peek(HorizonLexer *lexer, int z);

HorizonToken *horizon_lexer_next_token(HorizonLexer *lexer);

#endif