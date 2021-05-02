#include "iotree.h"

void WriteToFileRecursively(Node* node, FILE* file);
void GetTreeFromBuffer     (Node* node, Buffer* buffer);
void DeleteFictiveNodes    (Node* node);

void WriteToFile(Tree* tree, const char* output)
{
    assert(tree);
    assert(output);

    FILE* file = fopen(output, "w");
    assert(file);

    WriteToFileRecursively(tree->root, file);

    fclose(file);
}

void WriteToFileRecursively(Node* node, FILE* file)
{
    assert(file);

    if (node == nullptr)
    {
        return;
    }

    fprintf(file, "%d | ", node->type);

    if (node->type == NUMB_TYPE)
    {
        fprintf(file, "%d ", node->value.number);
    }
    else if (node->type == ID_TYPE)
    {
        fprintf(file, "%s ", node->value.name);
    }
    else
    {
        fprintf(file, "%d ", node->value.op);
    }


    fprintf(file, "{ ");
    WriteToFileRecursively(node->left, file);
    fprintf(file, "} ");

    fprintf(file, "{ ");
    WriteToFileRecursively(node->right, file);
    fprintf(file, "} ");
}

Tree* GetTreeFromFile(const char* file)
{
    assert(file);

    Buffer* buffer = GetBuffer(file);

    Tree* tree = NewTree();

    tree->root = ConstructNode(D_TYPE, { .op = 0 });
    buffer->str += 5;

    GetTreeFromBuffer(tree->root, buffer);

    DeleteFictiveNodes(tree->root);

    return tree;
}

void GetTreeFromBuffer(Node* node, Buffer* buffer)
{
    assert(node);
    assert(buffer);

    while (size_t(buffer->str - buffer->original_str) < buffer->lenght)
    {
        IgnoreSpaces(buffer);

        if (*buffer->str == '{')
        {
            if (node->left == nullptr)
            {
                node->left = NewNode();
                node->left->parent = node;
                node = node->left;
            }
            else
            {
                node->right = NewNode();
                node->right->parent = node;
                node = node->right;
            }

            buffer->str++;
            continue;
        }

        if (*buffer->str == '}')
        {
            node = node->parent;
            buffer->str++;
            continue;
        }

        int len = 0;
        sscanf(buffer->str, "%d %*c %n", (int*)&node->type, &len);
        buffer->str += len;

        if (node->type == NUMB_TYPE)
        {
            sscanf(buffer->str, "%d %n", &node->value.number, &len);
        }
        else if (node->type == ID_TYPE || node->type == DECL_TYPE)
        {
            node->value.name = (char*)calloc(MAX_NAME_LEN, sizeof(char));
            assert(node->value.name);

            sscanf(buffer->str, "%s %n", node->value.name, &len);
        }
        else
        {
            sscanf(buffer->str, "%d %n", &node->value.op, &len);
        }

        buffer->str += len;
    }
}

void DeleteFictiveNodes(Node* node)
{
    assert(node);

    if (node->left == nullptr && node->right == nullptr)
    {
        if (node->parent->left == node)
        {
            node->parent->left = nullptr;
        }
        else
        {
            node->parent->right = nullptr;
        }

        free(node);
    }
    else
    {
        if (node->left != nullptr)
        {
            DeleteFictiveNodes(node->left);
        }

        if (node->right != nullptr)
        {
            DeleteFictiveNodes(node->right);
        }
    }
}
