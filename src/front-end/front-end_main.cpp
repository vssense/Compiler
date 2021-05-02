#include "tree.h"
#include "iotree.h"
#include "translator.h"

const char* tree_tmp_file = "build/tree_tmp.txt";

struct FrontEndInfo
{
    const char* input_file  = nullptr;
    const char* output_file = nullptr;

    bool parser_dump_required    = false;
    bool tree_dump_required      = false;

    bool emit_tree_requiered     = false;
    bool emit_code_requiered     = false;
};

const int parser_dump_flag_offset = 2;
const int tree_dump_flag_offset   = 3;
const int emit_tree_flag_offset   = 4;
const int emit_code_flag_offset   = 5;

int main(const int argc, const char* argv[])
{
    FrontEndInfo info = {};
    info.input_file = argv[1];

    info.parser_dump_required = (strcmp(argv[parser_dump_flag_offset], "1") == 0);
    info.tree_dump_required   = (strcmp(argv[tree_dump_flag_offset],   "1") == 0);
    info.emit_tree_requiered  = (strcmp(argv[emit_tree_flag_offset],   "1") == 0);
    info.emit_code_requiered  = (strcmp(argv[emit_code_flag_offset],   "1") == 0);

    if (argc - 1 > emit_code_flag_offset)
    {
        info.output_file = argv[emit_code_flag_offset + 1];
    }


    if (info.emit_code_requiered)
    {
        const char* output = "translated_program.shl";
        if (info.output_file != nullptr)
        {
            output = info.output_file;
        }

        Translate(info.input_file, output);
        return 0;
    }

    Parser* parser = Parse(info.input_file);
    if (info.parser_dump_required)
    {
        ParserDump(parser);
    }

    Tree* tree = GetTree(parser);
    if (info.tree_dump_required)
    {
        TreeDump(tree);
    }

    if (info.emit_tree_requiered)
    {
        const char* output = "tree_in_written_form.txt";
        WriteToFile(tree, info.output_file);
    }
    else
    {
        WriteToFile(tree, tree_tmp_file);
    }

    DestructTree(tree);
    DeleteTree(tree);

    DestructParser(parser);
    DeleteParser(parser);
    return 0;
}