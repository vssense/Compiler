#include "flag_manager.h"

const int syscall_size = 128;

void CallFrontEnd(FlagInfo* info)
{
    assert(info);

    char front_end_call[syscall_size] = "";
    int len = 0;

    sprintf(front_end_call, "build/front-end %s %d %d %d %d%n", info->input_file,
            info->parser_dump_required, info->tree_dump_required,
            info->emit_tree_requiered, info->emit_code_requiered, &len);

    if ((info->emit_tree_requiered || info->emit_code_requiered) &&
         info->output_file != nullptr)
    {
        sprintf(front_end_call + len, " %s", info->output_file);
    }

    system(front_end_call);
}

void CallBackEnd(FlagInfo* info)
{
    assert(info);

    char back_end_call[syscall_size] = "";

    int len = 0;

    sprintf(back_end_call, "build/back-end %d %d %d %d%n", info->nametable_dump_required,
            info->asm_listing_required, info->speed_optimization_required,
            info->memory_optimization_required, &len);

    if (info->output_file != nullptr)
    {
        sprintf(back_end_call + len, " %s", info->output_file);
    }

    system(back_end_call);
}

int main(int argc, char const* argv[])
{
    FlagInfo info = {};
    GetFlagInfo(argc, argv, &info);

    if (info.input_file == nullptr)
    {
        printf("error: no input file\n");
        return 0;
    }

    CallFrontEnd(&info);

    if (info.emit_tree_requiered || info.emit_code_requiered)
    {
        return 0;
    }

    if (info.speed_optimization_required)
    {
        system("build/middle-end");
    }

    CallBackEnd(&info);

    return 0;
}