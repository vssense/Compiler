#include "compiling.h"

const char* tree_tmp_file = "build/tree_tmp.txt";

const int nametable_dump_flag_offset      = 1;
const int asm_listing_flag_offset         = 2;
const int speed_optimization_flag_offset  = 3;
const int memory_optimization_flag_offset = 4;

int main(int argc, char const* argv[])
{
    BackEndInfo info = {};
    info.nametable_dump_required      = (strcmp(argv[nametable_dump_flag_offset],      "1") == 0);
    info.asm_listing_required         = (strcmp(argv[asm_listing_flag_offset],         "1") == 0);
    info.speed_optimization_required  = (strcmp(argv[speed_optimization_flag_offset],  "1") == 0);
    info.memory_optimization_required = (strcmp(argv[memory_optimization_flag_offset], "1") == 0);

    if (argc - 1 > memory_optimization_flag_offset)
    {
        info.output_file = argv[memory_optimization_flag_offset + 1];
    }

    Tree* tree = GetTreeFromFile(tree_tmp_file);

    Compile(tree, &info);

    DestructTree(tree);
    DeleteTree(tree);
    return 0;
}