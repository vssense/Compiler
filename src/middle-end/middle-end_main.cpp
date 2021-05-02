#include "tree_optimize.h"
#include "../front-end/iotree.h"

const char* tree_tmp_file = "build/tree_tmp.txt";

int main()
{
    Tree* tree = GetTreeFromFile(tree_tmp_file);
    Optimize(tree);
    WriteToFile(tree, tree_tmp_file);

    DestructTree(tree);
    DeleteTree(tree);
    return 0;
}