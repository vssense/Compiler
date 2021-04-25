#include "flag_manager.h"

const char* flags[] =
{
    "-h", "-o", "-dot-dump", "-nametable-dump", "-parser-dump"
};

const char* help_message = "use flags:\n"
                 "      -h               display help\n"
                 "      -o               next arg is a name of output file\n"
                 "      -parser-dump     show all tokens and their types\n"
                 "      -nametable-dump  show info about functions and variables\n"
                 "      -dot-dump        show tree of program using dot\n";

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
        info->dot_dump_required = true;
    }
    else if (strcmp(argv[*offset], flags[3]) == 0)
    {
        info->nametable_dump_required = true;
    }
    else if (strcmp(argv[*offset], flags[4]) == 0)
    {
        info->parser_dump_required = true;
    }
    else
    {
        printf("Unknown flag. Use -h for help\n");
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
