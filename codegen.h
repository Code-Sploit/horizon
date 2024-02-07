#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

typedef struct HorizonCodeGenDataSeg
{
    enum
    {
        DATA_RO,
        DATA_RW
    } type;

    char *name;
    char *data;

    int size;
} HorizonCodeGenDataSeg;

typedef struct HorizonCodeGenMainSeg
{
    char *code;

    int size;
} HorizonCodeGenMainSeg;

typedef struct HorizonCodeGen
{
    const char *code;

    HorizonCodeGenDataSeg **data_segments;
    HorizonCodeGenMainSeg *main_segment;

    int count;
} HorizonCodeGen;

HorizonCodeGen *horizon_alloc_codegen();

HorizonCodeGenDataSeg *horizon_alloc_codegen_data_seg(int type, char *name, char *data, int size);
HorizonCodeGenMainSeg *horizon_alloc_codegen_main_seg(char *code);

void horizon_codegen_run(HorizonCodeGen *codegen, HorizonASTRootNode *root);

#endif