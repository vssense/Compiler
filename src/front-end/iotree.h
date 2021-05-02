#ifndef IOTREE_H
#define IOTREE_H
#include "tree.h"

Tree* GetTreeFromFile(const char* file);
void  WriteToFile    (Tree* tree, const char* output);
#endif