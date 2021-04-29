#ifndef COMPILING_H
#define COMPILING_H

#include "../tree/tree.h"

struct Function
{
    const char* name;

    char** vars;

    bool   is_void;
    size_t num_args;
    size_t num_vars;

    size_t offset;
};

#include "elf_writer.h"

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

    bool asm_listing_required;

    ElfFileWriter writer;

    size_t print_offset;
    size_t scan_offset;
};

NameTable* MakeTableOfNames(Tree* tree);
void       DumpNameTable   (NameTable* table);
void       Compile         (const int argc, const char** argv);

#endif