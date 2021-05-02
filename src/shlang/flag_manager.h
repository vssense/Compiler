#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct FlagInfo
{
    const char* input_file  = nullptr;
    const char* output_file = nullptr;

    bool emit_tree_requiered     = false;
    bool emit_code_requiered     = false;

    bool parser_dump_required    = false;
    bool tree_dump_required      = false;
    bool nametable_dump_required = false;

    bool asm_listing_required    = false;

    bool speed_optimization_required  = false;
    bool memory_optimization_required = false;
};

void GetFlagInfo(const int argc, const char** argv, FlagInfo* info);
