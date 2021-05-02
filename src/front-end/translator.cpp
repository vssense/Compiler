#include "translator.h"

const char* STRING_MATH_OP[] =
{
    "+", "-", "*", "/", "==", "!=", "<", ">", "<=", ">="
};

const int rank = 10;

void TranslateTree      (Tree* tree, FILE* file);
void TranslateFunc      (Node* node, FILE* file);
void TranslateArgs      (Node* node, FILE* file);
void TranslateCompound  (Node* node, FILE* file, size_t num_tabs);
void TranslateStatement (Node* node, FILE* file, size_t num_tabs);
void TranslateLoop      (Node* node, FILE* file, size_t num_tabs);
void TranslateCondition (Node* node, FILE* file, size_t num_tabs);
bool TranslateStdCall   (Node* node, FILE* file);
void TranslateExpression(Node* node, FILE* file);
void TranslateParams    (Node* node, FILE* file);
void IntToDrevniyRus    (FILE* file, int number);

void Translate(const char* input, const char* output)
{
    assert(input);

    Tree* tree = GetTreeFromFile(input);
    TreeDump(tree);
    FILE* translate = fopen(output, "w");
    assert(translate);

    fprintf(translate, "~ THIS FILE WAS GENERATED AUTOMATICLY\n"
                       "~ TREE TAKED FROM FILE '%s'          \n"
                       "~ CHECK FILE BEFORE COMPILING      \n\n", input);

    TranslateTree(tree, translate);

    DestructTree(tree);
    DeleteTree(tree);
}

void TranslateTree(Tree* tree, FILE* file)
{
    assert(tree);
    assert(file);

    Node* node = tree->root;

    while (node != nullptr)
    {
        TranslateFunc(node->right, file);
        node = node->left;
        fprintf(file, "\n");
    }
}

void TranslateFunc(Node* node, FILE* file)
{
    assert(node);
    assert(file);

    fprintf(file, "%s %s(", OPERATORS[DECLARATOR].rus_name, node->value.name);
    TranslateArgs(node->right, file);
    fprintf(file, ")\n");
    TranslateCompound(node->left, file, 0);
}

void TranslateArgs(Node* node, FILE* file)
{
    assert(file);

    if (node != nullptr)
    {
        fprintf(file, "%s", node->value.name);
        node = node->right;

        while (node != nullptr)
        {
            fprintf(file, ", %s", node->value.name);
            node = node->right;
        }
    }
}

void SetTabs(FILE* file, size_t number)
{
    assert(file);

    for (size_t i = 0; i < number; ++i)
    {
        fprintf(file, "\t");
    }
}

void TranslateCompound(Node* node, FILE* file, size_t num_tabs)
{
    assert(node);
    assert(file);

    if (node == nullptr)
    {
        return;
    }

    node = node->right;
    SetTabs(file, num_tabs);
    fprintf(file, "%s\n", OPERATORS[OPENING_BRACE].rus_name);

    while (node != nullptr)
    {
        TranslateStatement(node->left, file, num_tabs + 1);
        node = node->right;
    }

    SetTabs(file, num_tabs);
    fprintf(file, "%s\n", OPERATORS[CLOSING_BRACE].rus_name);
}

void TranslateStatement(Node* node, FILE* file, size_t num_tabs)
{
    assert(node);
    assert(file);

    SetTabs(file, num_tabs);

    switch (node->type)
    {
        case ASSG_TYPE :
        {
            if (node->left->type == ID_TYPE)
            {
                fprintf(file, "%s %s ", node->left->value.name, OPERATORS[ASSIGNMENT].rus_name);
            }
            else
            {
                fprintf(file, "can't interpret this construction");
            }
            
            TranslateExpression(node->right, file);
            fprintf(file, ";\n");
            break;
        }
        case DECL_TYPE :
        {
            fprintf(file, "%s %s %s ", OPERATORS[DECLARATOR].rus_name, node->left->value.name, OPERATORS[ASSIGNMENT].rus_name);
            TranslateExpression(node->right, file);
            fprintf(file, ";\n");
            break;
        }
        case COND_TYPE :
        {
            TranslateCondition(node, file, num_tabs);
            break;
        }
        case LOOP_TYPE :
        {
            TranslateLoop(node, file, num_tabs);
            break;
        }
        case CALL_TYPE :
        {
            if (!TranslateStdCall(node, file))
            {
                fprintf(file, "%s(", node->left->value.name);
                TranslateParams(node->right, file);
                fprintf(file, ");\n");
            }
            break;
        }
        case JUMP_TYPE :
        {
            fprintf(file, "%s ", OPERATORS[RET].rus_name);
            if (node->right != nullptr)
            {
                TranslateExpression(node->right, file);
            }
            fprintf(file, ";\n");
            break;
        }
        default :
        {
            fprintf(file, "can't interpret this construction ");
            break;
        }
    }
}

bool TranslateStdCall(Node* node, FILE* file)
{
    assert(node);
    assert(file);

    if (strcmp(node->left->value.name, "scan") == 0)
    {
        fprintf(file, "nepravdoi()");
        return true;
    }

    if (strcmp(node->left->value.name, "print") == 0)
    {
        fprintf(file, "govoru(");
        TranslateParams(node->right, file);
        fprintf(file, ");\n");
        return true;
    }

    return false;
}

void TranslateLoop(Node* node, FILE* file, size_t num_tabs)
{
    assert(node);
    assert(file);

    fprintf(file, "%s (", OPERATORS[LOOP].rus_name);
    TranslateExpression(node->left, file);
    fprintf(file, ")\n");

    TranslateCompound(node->right, file, num_tabs);
}

void TranslateCondition(Node* node, FILE* file, size_t num_tabs)
{
    assert(node);
    assert(file);

    fprintf(file, "%s (", OPERATORS[IF].rus_name);
    TranslateExpression(node->left, file);
    fprintf(file, ")\n");

    TranslateCompound(node->right->left, file, num_tabs);

    if (node->right->right != nullptr)
    {
        SetTabs(file, num_tabs);
        fprintf(file, "%s\n", OPERATORS[ELSE].rus_name);
        TranslateCompound(node->right->right, file, num_tabs);
    }
}

bool NeededBracket(Node* node)
{
    assert(node);

    return (node->value.op == SUB_OP || node->value.op == ADD_OP) && node->parent->type == MATH_TYPE;
}

void TranslateExpression(Node* node, FILE* file)
{
    assert(node);
    assert(file);

    switch (node->type)
    {
        case MATH_TYPE :
        {
            if (NeededBracket(node))
            {
                fprintf(file, "(");
            }
            
            TranslateExpression(node->left, file);
            fprintf(file, " %s ", STRING_MATH_OP[node->value.op]);
            TranslateExpression(node->right, file);

            if (NeededBracket(node)) 
            {
                fprintf(file, ")");
            }
            break;
        }
        case NUMB_TYPE :
        {   
            IntToDrevniyRus(file, node->value.number);   
            break;
        }
        case CALL_TYPE :
        {
            if (!TranslateStdCall(node, file))
            {
                fprintf(file, "%s(", node->left->value.name);
                TranslateParams(node->right, file);
                fprintf(file, ")");
            }
            break;
        }
        case ID_TYPE :
        {
            fprintf(file, "%s", node->value.name);
            break;
        }
        default :
        {
            fprintf(file, "can't interpret this construction");
            // printf("I'm here, but I shouldn't, line = %d\n", __LINE__);
            break;
        }
    }
}

void TranslateParams(Node* node, FILE* file)
{
    assert(file);

    if (node != nullptr)
    {
        while (node->right != nullptr)
        {
            node = node->right;
        }

        TranslateExpression(node->left, file);
        node = node->parent;

        while (node->type == ARG_TYPE)
        {
            fprintf(file, ", ");
            TranslateExpression(node->left, file);
            node = node->parent;
        }
    }
}

void IntToDrevniyRus(FILE* file, int number)
{
    assert(file);

    if (number == 0)
    {
        fprintf(file, "ноль");
        return;
    }

    bool NeededBracket = false;
    if (number < 0)
    {
        fprintf(file, "(полушка - целковый) * ");
        number *= -1;
        if (number - 1 > rank)
        {
            NeededBracket = true;
        }
    }

    if (NeededBracket)
    {
        fprintf(file, "(");
    }

    for (size_t power = 0; number > 0; ++power)
    {
        int digit = number % rank;
        fprintf(file, "%s", DIGITS[digit].rus_digit);

        for (size_t i = 0; i < power; ++i)
        {
            fprintf(file, " * десятичок");
        }

        if (number > rank)
        {
            fprintf(file, " + ");
        }

        number /= rank;
    }

    if (NeededBracket)
    {
        fprintf(file, ")");
    }
}
