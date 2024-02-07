#include "horizon.h"
#include "compiler.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        compiler_error("No target file specified!");
    }

    for (int i = 1; i < argc; i++)
    {
        compiler_debug("Compiling: %s", argv[i]);

        HorizonCompiler *compiler = compiler_bootup(argv[i]);

        int _ret = compiler_compile(compiler);
        
        compiler_debug("Target: %s Compiled: %i", argv[i], _ret);
    }

    return 0;
}