#ifndef COMPILING_H
#define COMPILING_H

#include "../front-end/tree.h"
#include "../front-end/iotree.h"

struct BackEndInfo
{
    const char* output_file = nullptr;

    bool nametable_dump_required = false;
    bool asm_listing_required    = false;

    bool speed_optimization_required  = false;
    bool memory_optimization_required = false;
};

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

// #define COMPILER_DEBUG

#ifdef COMPILER_DEBUG 

#define ASSERT_COMPILER if (!CompilerOk(compiler))          \
                        {                                    \
                           printf("function = %s", __func__);\
                           DumpCompiler(compiler);           \
                           assert(0);                        \
                        }
#else
#define ASSERT_COMPILER
#endif            

struct Compiler
{
    #ifdef COMPILER_DEBUG
    size_t canary1 = canary1_check;
    #endif

    NameTable* table;
    Function*  function;

    FILE* file;

    size_t label;

    bool asm_listing_required;
    bool speed_optimization_required;
    bool memory_optimization_required;

    ElfFileWriter writer;

    size_t print_offset;
    size_t scan_offset;
        
    #ifdef COMPILER_DEBUG
    size_t canary2 = canary2_check;
    #endif
};

NameTable* MakeTableOfNames(Tree* tree);
void       DumpNameTable   (NameTable* table);
void       Compile         (Tree* tree, BackEndInfo* info);

#endif