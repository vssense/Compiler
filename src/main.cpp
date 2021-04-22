#include "compiling/compiling.h"

const char* FILE_TO_WRITE = "tree_in_written_form.txt";

int main(const int argc, const char* argv[])
{
    if (argc - 1 > 0)
    {
        Compile(argv[1]);
    }
    else
    {
        printf("error: no input file\n");
    }

    return 0;
}