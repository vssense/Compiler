#include "flag_manager.h"

const char* flags[] =
{
    "-h", "-o", "-S", "-Os", "-Om",
    "-femit-parser-dump", "-femit-tree-dump", "-femit-nametable-dump",
    "-femit-tree", "-translate"
};

const char* help_message =
"use flags:                                                                              \n\t"
"      -h                    display help                                                \n\t"
"      -o                    output file name (default b.out)                            \n\t"
"      -S                    create file with nasm code of your program                  \n\t"
"      -Os                   optimization for speed                                      \n\t"
"      -Om                   optimization for memory usage                               \n\t"
"      -femit-parser-dump    emit console dump of all tokens and their types             \n\t"
"      -femit-tree-dump      emit tree diagram in 'log' subdir and open it using dot tool\n\t"
"      -femit-nametable-dump emit console dump info about functions and variables        \n\t"
"      -femit-tree           emit tree in written form for translation                   \n\t"
"      -translate            emit drevniy-rus-language code received from tree           \n\t";

void GetFlag(const char** argv, FlagInfo* info, int* offset)
{
    assert(argv);
    assert(info);

    if (strcmp(argv[*offset], flags[0]) == 0)
    {
        printf("%s\n", help_message);
    }
    else if (strcmp(argv[*offset], flags[1]) == 0)
    {
        info->output_file = argv[++*offset];
    }
    else if (strcmp(argv[*offset], flags[2]) == 0)
    {
        info->asm_listing_required = true;
    }
    else if (strcmp(argv[*offset], flags[3]) == 0)
    {
        info->speed_optimization_required = true;
    }
    else if (strcmp(argv[*offset], flags[4]) == 0)
    {
        info->memory_optimization_required = true;
    }
    else if (strcmp(argv[*offset], flags[5]) == 0)
    {
        info->parser_dump_required = true;
    }
    else if (strcmp(argv[*offset], flags[6]) == 0)
    {
        info->tree_dump_required = true;
    }
    else if (strcmp(argv[*offset], flags[7]) == 0)
    {
        info->nametable_dump_required = true;
    }
    else if (strcmp(argv[*offset], flags[8]) == 0)
    {
        info->emit_tree_requiered = true;
    }
    else if (strcmp(argv[*offset], flags[9]) == 0)
    {
        info->emit_code_requiered = true;
    }
    else
    {
        printf("Unknown flag <%s>. Use -h for help.\n", argv[*offset]);
    }
}

void GetFlagInfo(const int argc, const char** argv, FlagInfo* info)
{
    assert(argv);
    assert(info);

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-')
        {
            GetFlag(argv, info, &i);
        }
        else
        {
            if (info->input_file != nullptr)
            {
                printf("error : more than one input files\n");
                return;
            }

            info->input_file = argv[i];
        }
    }
}

