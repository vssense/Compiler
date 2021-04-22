#include "compiling.h"

const char* ASM_FILE_NAME = "asm_tmp.nasm";

static const char* SCAN  = "nepravdoi";
static const char* PRINT = "govoru";

/////////////////////////////////////////////////////////////////
//// NameTable
/////////////////////////////////////////////////////////////////
NameTable* MakeTableOfNames (Tree* tree);
size_t     GetNumFunc       (Tree* tree);
NameTable* NewNameTable     ();
void       FillNameTable    (NameTable* table, Tree* tree);
void       GetFunction      (Node* subtree, Function* function);
size_t     GetNumArgs       (Node* node);
size_t     GetNumVars       (Node* node);
void       GetNames         (char** vars, Node* subtree);
void       GetNewVars       (Node* node, char** vars, size_t* ofs);
void       DumpNameTable    (NameTable* table);
Function*  FindFunc         (const char* name, NameTable* table);
void       DestructNameTable(NameTable* table);
void       DeleteNameTable  (NameTable* table);

/////////////////////////////////////////////////////////////////
///// Assemble
/////////////////////////////////////////////////////////////////
void WriteAsmCode             (Tree* tree, NameTable* table, FILE* file);
void WriteAsmStdFunctions     (Compiler* compiler);
void WriteAsmStdData          (Compiler* compiler);
void WriteAsmFunc             (Node* node, Compiler* compiler);
void WriteAsmCompound         (Node* node, Compiler* compiler);
void WriteAsmStatement        (Node* node, Compiler* compiler);
void WriteAsmLoop             (Node* node, Compiler* compiler);
void WriteAsmCondition        (Node* node, Compiler* compiler);
void WriteAsmCall             (Node* node, Compiler* compiler);
bool WriteAsmStdCall          (Node* node, Compiler* compiler);
void WriteAsmAssignment       (Node* node, Compiler* compiler);
void WriteAsmExpression       (Node* node, Compiler* compiler);
void WriteAsmSimpleExpression (Node* node, Compiler* compiler);
void WriteAsmPrimaryExpression(Node* node, Compiler* compiler);
void WriteAsmCompare          (Node* node, Compiler* compiler);
int  GetVarOfs                (Function* function, char* name);


#define ASM_ASSERT assert(node);               \
                   assert(compiler->table);   \
                   assert(compiler->file);    \
                   assert(compiler->function)

#define FUNC      compiler->function
#define ASM_FILE  compiler->file
#define LABEL     compiler->label

void Compile(const char* input)
{
    assert(input);

    Parser* parser = Parse(input);
    Tree* tree = GetTree(parser);
    TreeDump(tree);
    Assemble(tree);

    DestructTree(tree);
    DeleteTree(tree);

    DestructParser(parser);    
    DeleteParser(parser);    
}

void Assemble(Tree* tree)
{
    assert(tree);

    NameTable* table = MakeTableOfNames(tree);

    FILE* asm_file = fopen(ASM_FILE_NAME, "w");
    assert(asm_file);

    WriteAsmCode(tree, table, asm_file);
 
    fclose(asm_file);
}

void WriteAsmCode(Tree* tree, NameTable* table, FILE* file)
{
    assert(tree);
    assert(file);

    Node* node = tree->root;

    Compiler compiler = {};
    compiler.table     = table;
    compiler.function  = table->functions;
    compiler.file      = file;
    compiler.label     = 0;

    fprintf(file, "section .text\n"
                  "global _start\n\n");

    fprintf(file, "_start:       \n\t"
                  "call main     \n\t"
                  "mov  rax, 0x3C\n\t"
                  "xor  rdi, rdi \n\t"
                  "syscall       \n");

    WriteAsmStdFunctions(&compiler);

    for (size_t i = 0; i < table->num_func; ++i)
    {
        WriteAsmFunc(node->right, &compiler);

        compiler.function++;
        node = node->left;
    }

    WriteAsmStdData(&compiler);

    DestructNameTable(table);
    DeleteNameTable(table);
}

void WriteAsmStdFunctions(Compiler* compiler)
{
    fprintf(ASM_FILE, "\nprint:                  \n\t"
                      "mov rax, [rsp + 8]      \n\t"
                      "mov rsi, IO_BUFFER + 15 \n\t"
                      "mov byte [rsi], '0'     \n\t"
                      "mov rbx, 10             \n\t"

                    "print_while:              \n\t"
                      "test rax, rax           \n\t"
                      "jz print_while_end      \n\t"

                      "cqo                     \n\t"
                      "div rbx                 \n\t"
                      "add rdx, '0'            \n\t"
                      "mov byte [rsi], dl      \n\t"
                      "dec rsi                 \n\t"
                      "jmp print_while         \n\t"
                    "print_while_end:          \n\t"

                      "mov rdx, 16             \n\t"
                      "mov rdi, 1              \n\t"
                      "mov rax, 1              \n\t"
                      "syscall                 \n\t"
                      "ret                     \n\t");
}

void WriteAsmStdData(Compiler* compiler)
{
    fprintf(ASM_FILE, "section .data  \n\t"
                      "IO_BUFFER:     \n\t"
                      "times 16 db 0  \n\t"
                      "         db ' '\n\t");
}

void WriteAsmFunc(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    fprintf(ASM_FILE, "\n%s :       \n\t", node->value.name);
    fprintf(ASM_FILE, "push rbp     \n\t"
                      "mov  rbp, rsp\n\t"
                      "sub  rsp, %lu\n\t", FUNC->num_vars * 8);

    WriteAsmCompound(node->left->right, compiler);
}

void WriteAsmCompound(Node* node, Compiler* compiler)
{
    if (node == nullptr)
    {
        return;
    }

    ASM_ASSERT;

    while (node != nullptr)
    {
        WriteAsmStatement(node->left, compiler);
        node = node->right;
    }
}

void WriteAsmStatement(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    switch (node->type)
    {
        case DECL_TYPE :
        {
            WriteAsmAssignment(node, compiler);
            break;
        }
        case ASSG_TYPE :
        {
            WriteAsmAssignment(node, compiler);
            break;
        }
        case JUMP_TYPE :
        {
            if (!FUNC->is_void)
            {
                WriteAsmExpression(node->right, compiler);
                fprintf(ASM_FILE, "pop  rax\n\t");
            }

            fprintf(ASM_FILE, "mov  rsp, rbp\n\t"
                              "pop  rbp     \n\t"
                              "ret          \n\t");
            break;
        }
        case COND_TYPE :
        {
            WriteAsmCondition(node, compiler);
            break;
        }
        case LOOP_TYPE :
        {
            WriteAsmLoop(node, compiler);
            break;
        }
        case CALL_TYPE :
        {
            WriteAsmCall(node, compiler);
            break;
        }
        default :
        {
            printf("I am here, but I shouldn't, line = %d\n", __LINE__);
            break;
        }
    }
}

void WriteAsmLoop(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    size_t label = LABEL++;

    fprintf(ASM_FILE, "LOOP_%lu :\n\t", label);
    WriteAsmExpression(node->left, compiler);

    fprintf(ASM_FILE, "push 0         \n\t"
                      "je CONTINUE_%lu\n\t", label);

    WriteAsmCompound(node->right->right, compiler);

    fprintf(ASM_FILE, "jmp LOOP_%lu  \n"
                      "CONTINUE_%lu :\n\t", label, label);
}

void WriteAsmCondition(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    WriteAsmExpression(node->left, compiler);

    size_t label = LABEL++;

    fprintf(ASM_FILE, "pop  rax      \n\t"
                      "test rax, rax \n\t"
                      "jnz  TRUE_%lu \n\t"
                      "jmp  FALSE_%lu\n"
                      "TRUE_%lu :   \n\t", label, label, label);

    WriteAsmCompound(node->right->left->right, compiler);

    fprintf(ASM_FILE, "jmp CONTINUE_%lu\n"
                      "FALSE_%lu :     \n\t", label, label);

    if (node->right->right != nullptr)
    {
        WriteAsmCompound(node->right->right->right, compiler);
    }

    fprintf(ASM_FILE, "CONTINUE_%lu :\n\t", label);
}

void WriteAsmAssignment(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    WriteAsmExpression(node->right, compiler);

    fprintf(ASM_FILE, "pop  rax\n\t"
                      "mov [rbp + %d], rax\n\t", GetVarOfs(FUNC, node->left->value.name));
}

void WriteAsmExpression(Node* node, Compiler* compiler)
{
    if (node == nullptr)
    {
        return;    
    }

    ASM_ASSERT;

    if (node->type == MATH_TYPE && node->value.op >= EQUAL_OP) // is comparison
    {
        WriteAsmExpression(node->left,  compiler);
        WriteAsmExpression(node->right, compiler);
        WriteAsmCompare(node, compiler);
    }
    else
    {
        WriteAsmSimpleExpression(node, compiler);
    }
}

void WriteAsmSimpleExpression(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    if (node->type == MATH_TYPE)
    {
        WriteAsmSimpleExpression(node->left,  compiler);
        WriteAsmSimpleExpression(node->right, compiler);
        
        switch (node->value.op)
        {
            case MUL_OP :
            {
                fprintf(ASM_FILE, "pop  rbx\n\t"
                                  "pop  rax\n\t"
                                  "imul rbx\n\t"
                                  "push rax\n\t");
                break;
            }
            case DIV_OP :
            {   
                fprintf(ASM_FILE, "pop  rbx\n\t"
                                  "pop  rax\n\t"
                                  "cqo     \n\t"
                                  "idiv rbx\n\t"
                                  "push rax\n\t");
                break;
            }
            case ADD_OP :
            {   
                fprintf(ASM_FILE, "pop  rbx\n\t"
                                  "pop  rax\n\t"
                                  "add  rax, rbx\n\t"
                                  "push rax\n\t");
                break;
            }
            case SUB_OP :
            {
                fprintf(ASM_FILE, "pop  rbx\n\t"
                                  "pop  rax\n\t"
                                  "sub  rax, rbx\n\t"
                                  "push rax\n\t");
                break;
            }
            default:
            {
                printf("I am here, but I shouldn't, line = %d\n", __LINE__);
            }
        }
    }
    else
    {
        WriteAsmPrimaryExpression(node, compiler);
    }
}

void WriteAsmPrimaryExpression(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    switch (node->type)
    {
        case ID_TYPE :
        {
            fprintf(ASM_FILE, "mov  rax, [rbp + %d]\n\t"
                              "push rax            \n\t",
                               GetVarOfs(FUNC, node->value.name));
            break;
        }
        case NUMB_TYPE :
        {
            fprintf(ASM_FILE, "push %d\n\t", node->value.number);
            break;
        }
        case CALL_TYPE :
        {
            WriteAsmCall(node, compiler);
            break;
        }
        default :
        {
            printf("I am here, but I shouldn't, line = %d\n", __LINE__);
        }
    }
}

void WriteAsmCall(Node* node, Compiler* compiler)
{
    if (WriteAsmStdCall(node, compiler))
    {
        return;
    }

    ASM_ASSERT;

    Function* call_func = FindFunc(node->left->value.name, compiler->table);

    if (call_func != nullptr)
    {       
        for (size_t i = 0; i < call_func->num_args; ++i)
        {
            node = node->right;
            if (node == nullptr)
            {
                printf("error : too few arguments for '%s'\n", call_func->name);
                return;
            }

            WriteAsmExpression(node->left, compiler);
        }

        if (node->right != nullptr)
        {
            printf("error : too more arguments for '%s'\n", call_func->name);
        }

        fprintf(ASM_FILE, "call %s\n\t"
                          "add rsp, %lu\n\t", call_func->name, call_func->num_args * 8);
        if (!call_func->is_void)
        {
            fprintf(ASM_FILE, "push rax\n\t");
        }
    }
}

bool WriteAsmStdCall(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    if (strcmp(node->left->value.name, SCAN) == 0)
    {
        fprintf(ASM_FILE, "call scan\n\t"
                          "push rax \n\t");
        return true;
    }

    if (strcmp(node->left->value.name, PRINT) == 0)
    {
        WriteAsmExpression(node->right->left, compiler);
        fprintf(ASM_FILE, "call print\n\t"
                          "pop  rbx  \n\t"); //cleaning stack
        return true;
    }

    return false;
}

Function* FindFunc(const char* name, NameTable* table)
{
    assert(name);

    for (size_t i = 0; i < table->num_func; ++i)
    {
        if (strcmp(table->functions[i].name, name) == 0)
        {
            return table->functions + i;
        }
    }

    printf("error : unknown function : '%s'\n", name);

    return nullptr;
}

void WriteAsmCompare(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    fprintf(ASM_FILE, "pop  rax\n\t"
                      "pop  rbx\n\t"
                      "cmp  rbx, rax\n\t"); //!!! might be rbx, rax

    switch (node->value.op)
    {
        case EQUAL_OP :
        {
            fprintf(ASM_FILE, "je ");
            break;
        }
        case NOT_EQUAL_OP :
        {
            fprintf(ASM_FILE, "jne ");
            break;
        }
        case LESS_OP :
        {
            fprintf(ASM_FILE, "jl ");
            break;
        }
        case GREATER_OP :
        {
            fprintf(ASM_FILE, "jg ");
            break;
        }
        case LESS_EQUAL_OP :
        {
            fprintf(ASM_FILE, "jle ");
            break;
        }
        case GREATER_EQUAL_OP :
        {
            fprintf(ASM_FILE, "jge ");
            break;
        }
    }

    size_t label = LABEL++;

    fprintf(ASM_FILE, "TRUE_%lu      \n\t"
                      "push 0          \n\t"
                      "jmp CONTINUE_%lu\n"
                      "TRUE_%lu :      \n\t"
                      "push 1          \n"
                      "CONTINUE_%lu :  \n\t", label, label, label, label);
}

int GetVarOfs(Function* function, char* name)//!!!
{
    assert(function);
    assert(name);

    for (size_t i = 0; i < function->num_args; ++i)
    {
        if (strcmp(function->vars[i], name) == 0)
        {
            return (i + 2) * 8;
        }
    }

    for (size_t i = function->num_args; i < function->num_vars + function->num_args; ++i)
    {
        if (strcmp(function->vars[i], name) == 0)
        {
            return - (i + 1) * 8;
        }
    }
    
    printf("error : '%s' wasn't declarated here\n", name);
    return -1;
}

NameTable* MakeTableOfNames(Tree* tree)
{
    assert(tree);

    size_t num_func = GetNumFunc(tree);

    NameTable* table = NewNameTable();
    table->functions = (Function*)calloc(num_func, sizeof(Function));
    table->num_func  = num_func;

    FillNameTable(table, tree);

    return table;
}

void DestructNameTable(NameTable* table)
{
    assert(table);

    for (size_t i = 0; i < table->num_func; ++i)
    {
        free(table->functions[i].vars);
    }
    
    free(table->functions);
}

void DeleteNameTable(NameTable* table)
{
    assert(table);

    free(table);
}

size_t GetNumFunc(Tree* tree)
{
    assert(tree);

    size_t result = 0;
    Node* node = tree->root;

    while (node != nullptr)
    {
        node = node->left;
        result++;
    }

    return result;
}

NameTable* NewNameTable()
{
    NameTable* table = (NameTable*)calloc(1, sizeof(NameTable));
    assert(table);

    table->num_func  = 0;
    table->functions = nullptr;

    return table;
}

void FillNameTable(NameTable* table, Tree* tree)
{
    assert(table);
    assert(tree);

    Node* node = tree->root;

    for (size_t i = 0; i < table->num_func; ++i)
    {
        GetFunction(node->right, table->functions + i);
        node = node->left;
    }
}

void GetFunction(Node* subtree, Function* function)
{
    assert(subtree);
    assert(function);

    function->name    = subtree->value.name;
    function->is_void = subtree->parent->value.is_void;

    function->num_args = GetNumArgs(subtree->right);
    function->num_vars = GetNumVars(subtree->left->right);

    function->vars = (char**)calloc(function->num_args + function->num_vars, sizeof(char*));

    GetNames(function->vars, subtree);
}

size_t GetNumArgs(Node* node)
{
    size_t result = 0;

    while (node != nullptr)
    {
        node = node->right;
        result++;
    }

    return result;
}

size_t GetNumVars(Node* node)
{
    if (node == nullptr) 
    {
        return 0;
    }

    if (node->type == DECL_TYPE)
    {
        return 1 + GetNumVars(node->left) + GetNumVars(node->right);
    }

    return GetNumVars(node->left) + GetNumVars(node->right);
}

void GetNames(char** vars, Node* subtree)
{
    assert(vars);
    assert(subtree);

    Node* arg = subtree->right;

    size_t ofs = 0;

    for (; arg != nullptr; ++ofs)
    {
        vars[ofs] = arg->value.name;

        arg = arg->right;
    }

    GetNewVars(subtree->left->right, vars, &ofs);
}

void GetNewVars(Node* node, char** vars, size_t* ofs)
{
    assert(vars);
    assert(ofs);

    if (node == nullptr)
    {
        return;
    }

    GetNewVars(node->left, vars, ofs);

    if (node->type == DECL_TYPE)
    {
        vars[*ofs] = node->left->value.name;
        (*ofs)++;
        return;
    }

    GetNewVars(node->right, vars, ofs);
}

void DumpNameTable(NameTable* table)
{
    assert(table);

    for (size_t i = 0; i < table->num_func; ++i)
    {
        printf("\n%s :\n", table->functions[i].name);
        printf("\tparams : ");

        for (size_t j = 0; j < table->functions[i].num_args; ++j)
        {
            printf("%s, ", table->functions[i].vars[j]);
        }
        printf("\n\tvars : ");

        for (size_t j = table->functions[i].num_args; j < table->functions[i].num_args + 
                                                          table->functions[i].num_vars; ++j)
        {
            printf("%s, ", table->functions[i].vars[j]);
        }
    }
}
