#include "../tree/tree.h"

struct Function
{
    const char* name;

    char** vars;

    bool   is_void;
    size_t num_args;
    size_t num_vars;
};

struct NameTable
{
    Function* functions;

    size_t num_func;
};

struct Compiler
{
    NameTable* table;
    Function*  function;

    FILE* file;

    size_t label;
};

NameTable* MakeTableOfNames(Tree* tree);
void       DumpNameTable   (NameTable* table);
void       Compile         (const int argc, const char** argv);
