#include "compiling.h"
#include "flag_manager.h"
#include "byte_code.h"

const size_t offset_to_set_data   = 0x02;
const size_t start_virtual_offset = 0x400000;

const char* ASM_FILE_NAME = "asm_listing.nasm";
const char* DEFAULT_OUTPUT_FILE_NAME = "b.out";

#include "std_func.cpp"

const char* SCAN  = "nepravdoi";
const char* PRINT = "govoru";

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
void       GetVars          (Node* node, char** vars, size_t* ofs);
void       NameTableDump    (NameTable* table);
Function*  FindFunc         (const char* name, NameTable* table);
void       DestructNameTable(NameTable* table);
void       DeleteNameTable  (NameTable* table);

/////////////////////////////////////////////////////////////////
///// Assemble
/////////////////////////////////////////////////////////////////
void Assemble                 (Tree* tree, FlagInfo* info);
void WriteAsmCode             (Tree* tree, NameTable* table, FILE* file, FlagInfo* info);
void InitCompiler             (Compiler* compiler, NameTable* table, FILE* file, FlagInfo* info);
void WriteAsm_start           (Compiler* compiler);
void WriteByteCodeToFile      (Compiler* compiler, FlagInfo* info);
void WriteAsmStdFunctions     (Compiler* compiler);
void WriteAsmStdData          (Compiler* compiler);
void WriteAsmFunc             (Node* node, Compiler* compiler);
void WriteAsmCompound         (Node* node, Compiler* compiler);
void WriteAsmStatement        (Node* node, Compiler* compiler);
void WriteAsmArrayDeclaration (Node* node, Compiler* compiler);
void WriteAsmLoop             (Node* node, Compiler* compiler);
void WriteAsmCondition        (Node* node, Compiler* compiler);
void WriteAsmCall             (Node* node, Compiler* compiler);
bool WriteAsmStdCall          (Node* node, Compiler* compiler);
void WriteAsmAssignment       (Node* node, Compiler* compiler);
void WriteAsmExpression       (Node* node, Compiler* compiler);
void WriteAsmSimpleExpression (Node* node, Compiler* compiler);
void WriteAsmPrimaryExpression(Node* node, Compiler* compiler);
void WriteAsmMemoryAccess     (Node* node, Compiler* compiler);
void WriteAsmCompare          (Node* node, Compiler* compiler);
int  GetVarOfs                (Function* function, char* name);


#define ASM_ASSERT assert(node);               \
                   assert(compiler->table);    \
                   assert(compiler->function)

#define FUNC       compiler->function

void Compile(const int argc, const char** argv)
{
    assert(argv);

    FlagInfo info = {};
    GetFlagInfo(argc, argv, &info);

    if (info.input_file == nullptr)
    {
        printf("error : no input file\n");
        return;
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

    Assemble(tree, &info);

    DestructTree(tree);
    DeleteTree(tree);

    DestructParser(parser);
    DeleteParser(parser);
}

void Assemble(Tree* tree, FlagInfo* info)
{
    assert(tree);

    NameTable* table = MakeTableOfNames(tree);

    if (info->nametable_dump_required)
    {
        NameTableDump(table);
    }

    FILE* asm_file = nullptr;

    if (info->asm_listing_required)
    {
        asm_file = fopen(ASM_FILE_NAME, "w");
        assert(asm_file);
    }

    WriteAsmCode(tree, table, asm_file, info);

    if (info->asm_listing_required)
    {
        fclose(asm_file);
    }
}

void InitCompiler(Compiler* compiler, NameTable* table, FILE* file, FlagInfo* info)
{
    assert(compiler);
    assert(table);
    assert(info);

    compiler->table    = table;
    compiler->function = table->functions;
    compiler->file     = file;
    compiler->label    = 0;

    compiler->asm_listing_required         = info->asm_listing_required;
    compiler->speed_optimization_required  = info->speed_optimization_required;
    compiler->memory_optimization_required = info->memory_optimization_required;

    Construct(&compiler->writer);
    InitElf64(&compiler->writer);
}

void WriteAsmCode(Tree* tree, NameTable* table, FILE* file, FlagInfo* info)
{
    assert(tree);
    assert(table);
    assert(info);

    Node* node = tree->root;

    Compiler compiler = {};

    InitCompiler(&compiler, table, file, info);
    WriteAsm_start(&compiler);
    WriteAsmStdFunctions(&compiler);

    for (size_t i = 0; i < table->num_func; ++i)
    {
        WriteAsmFunc(node->right, &compiler);

        compiler.function++;
        node = node->left;
    }

    WriteAsmStdData(&compiler);
    SetAllCalls(&compiler);
    SetElfHeaders(&compiler.writer);

    WriteByteCodeToFile(&compiler, info);

    DestructNameTable(table);
    DeleteNameTable(table);
    Destruct(&compiler.writer);
}

void WriteAsm_start(Compiler* compiler)
{
    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "section .text\n"
                                "global _start\n\n"
                                "_start:      \n\t");
    }

    WriteCall     (compiler, FindFunc("main", compiler->table));
    WriteMovR64Num(compiler, RAX, 0x3c);
    WriteMovR64Num(compiler, RDI, 0x00);
    WriteSyscall  (compiler);
}

void WriteByteCodeToFile(Compiler* compiler, FlagInfo* info)
{
    assert(compiler);
    assert(info);

    const char* output_file_name = DEFAULT_OUTPUT_FILE_NAME;

    if (info->output_file != nullptr)
    {
        output_file_name = info->output_file;
    }

    FILE* output = fopen(output_file_name, "w");
    assert(output);
    fwrite(compiler->writer.buffer, compiler->writer.offset, sizeof(char), output);
    fclose(output);
}

void WriteAsmStdFunctions(Compiler* compiler)
{
    assert(compiler);

    ElfFileWriter* writer = &compiler->writer;

    memcpy(writer->buffer + writer->offset, byte_code_print, sizeof(byte_code_print));
    compiler->print_offset = writer->offset;
    writer->offset += sizeof(byte_code_print);

    memcpy(writer->buffer + writer->offset, byte_code_scan, sizeof(byte_code_scan));
    compiler->scan_offset = writer->offset;
    writer->offset += sizeof(byte_code_scan);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "%s", nasm_print_func);
        fprintf(compiler->file, "%s", nasm_scan_func);
    }
}

void WriteAsmStdData(Compiler* compiler)
{
    assert(compiler);

    int data_segment_offset = InitDataSegment(&compiler->writer);

    memcpy(compiler->writer.buffer + compiler->writer.offset, byte_code_std_data, sizeof(byte_code_std_data));
    compiler->writer.offset += sizeof(byte_code_std_data);

    SetInt(&compiler->writer, start_virtual_offset + data_segment_offset, offset_to_set_data + compiler->print_offset);
    SetInt(&compiler->writer, start_virtual_offset + data_segment_offset, offset_to_set_data + compiler->scan_offset);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "%s", asm_data);
    }
}

void WriteAsmFunc(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    WriteFuncDecl(compiler, FUNC);

    WritePushR64  (compiler, RBP);
    WriteMovR64R64(compiler, RBP, RSP);
    WriteSubR64Num(compiler, RSP, FUNC->num_vars * sizeof(int64_t));

    WriteAsmCompound(node->left->right, compiler);

    WriteMovR64R64(compiler, RSP, RBP);
    WritePopR64   (compiler, RBP);
    WriteRet      (compiler);
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
        case ADECL_TYPE :
        {
            WriteAsmArrayDeclaration(node, compiler);
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
                WritePopR64(compiler, RAX);
            }

            WriteMovR64R64(compiler, RSP, RBP);
            WritePopR64   (compiler, RBP);
            WriteRet      (compiler);
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

void WriteAsmArrayDeclaration(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    WriteAsmExpression(node->right, compiler);

    WriteMovMemR64(compiler, RBP, RSP, GetVarOfs(FUNC, node->left->value.name));
    WritePopR64   (compiler, RAX);
    WriteSalR64   (compiler, RAX, 3); //sizeof(int64_t)
    WriteSubR64R64(compiler, RSP, RAX);
}

void WriteAsmLoop(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    size_t loop_label = compiler->label++;
    size_t continue_label = compiler->label++;

    size_t loop_label_offset = WriteLabel(compiler, loop_label);

    WriteAsmExpression(node->left, compiler);

    WritePopR64(compiler, RAX);
    WriteTest  (compiler, RAX, RAX);
    size_t continue_jump_offset = WriteJumpOp(compiler, EQUAL_OP, continue_label);

    WriteAsmCompound(node->right->right, compiler);

    size_t loop_jump_offset      = WriteJump (compiler, loop_label);
    size_t continue_label_offset = WriteLabel(compiler, continue_label);

    SetInt(&compiler->writer, loop_label_offset - loop_jump_offset - sizeof(int), loop_jump_offset);
    SetInt(&compiler->writer, continue_label_offset - continue_jump_offset - sizeof(int), continue_jump_offset);
}

void WriteAsmCondition(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    WriteAsmExpression(node->left, compiler);

    size_t true_label = compiler->label++;
    size_t false_label = compiler->label++;
    size_t continue_label = compiler->label++;

    WritePopR64(compiler, RAX);
    WriteTest  (compiler, RAX, RAX);

    size_t true_jump_offset  = WriteJumpOp(compiler, NOT_EQUAL_OP, true_label);
    size_t false_jump_offset = WriteJump  (compiler, false_label);
    size_t true_label_offset = WriteLabel (compiler, true_label);

    WriteAsmCompound(node->right->left->right, compiler);

    size_t continue_jump_offset = WriteJump(compiler, continue_label);
    size_t false_label_offset   = WriteLabel(compiler, false_label);

    if (node->right->right != nullptr)
    {
        WriteAsmCompound(node->right->right->right, compiler);
    }

    size_t continue_label_offset = WriteLabel(compiler, continue_label);

    SetInt(&compiler->writer, true_label_offset - true_jump_offset - sizeof(int), true_jump_offset);
    SetInt(&compiler->writer, false_label_offset - false_jump_offset - sizeof(int), false_jump_offset);
    SetInt(&compiler->writer, continue_label_offset - continue_jump_offset - sizeof(int), continue_jump_offset);
}

void WriteAsmAssignment(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    WriteAsmExpression(node->right, compiler);

    if (node->left->type == MEM_ACCESS_TYPE)
    {
        WriteAsmExpression(node->left->right, compiler);

        WriteMovR64Mem(compiler, RAX, RBP, GetVarOfs(FUNC, node->left->left->value.name));
        WritePopR64   (compiler, RBX);
        WriteSalR64   (compiler, RBX, 3);    //sizeof(int64_t)
        WriteSubR64R64(compiler, RAX, RBX);
        WritePopR64   (compiler, RBX);
        WriteMovMemR64(compiler, RAX, RBX);

        return;
    }

    WritePopR64   (compiler, RAX);
    WriteMovMemR64(compiler, RBP, RAX, GetVarOfs(FUNC, node->left->value.name));
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
        
        WritePopR64(compiler, RBX);
        WritePopR64(compiler, RAX);

        switch (node->value.op)
        {
            case ADD_OP :
            {   
                WriteAddR64R64(compiler, RAX, RBX);
                break;
            }
            case SUB_OP :
            {
                WriteSubR64R64(compiler, RAX, RBX);
                break;
            }
            case MUL_OP :
            {
                WriteMulR64(compiler, RBX);
                break;
            }
            case DIV_OP :
            {   
                WriteDivR64(compiler, RBX);
                break;
            }
            default:
            {
                printf("I am here, but I shouldn't, line = %d\n", __LINE__);
            }
        }

        WritePushR64(compiler, RAX);
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
            WriteMovR64Mem(compiler, RAX, RBP, GetVarOfs(FUNC, node->value.name));
            WritePushR64  (compiler, RAX);
            break;
        }
        case NUMB_TYPE :
        {
            WritePushNum(compiler, node->value.number);
            break;
        }
        case MEM_ACCESS_TYPE :
        {
            WriteAsmMemoryAccess(node, compiler);
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

void WriteAsmMemoryAccess(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    WriteAsmExpression(node->right, compiler);

    WriteMovR64Mem(compiler, RAX, RBP, GetVarOfs(FUNC, node->left->value.name));
    WritePopR64   (compiler, RBX);
    WriteSalR64   (compiler, RBX, 3);
    WriteSubR64R64(compiler, RAX, RBX);
    WriteMovR64Mem(compiler, RAX, RAX);
    WritePushR64  (compiler, RAX);
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

        WriteCall     (compiler, call_func);
        WriteAddR64Num(compiler, RSP, call_func->num_args * 8);

        if (!call_func->is_void)
        {
            WritePushR64(compiler, RAX);
        }
    }
}

bool WriteAsmStdCall(Node* node, Compiler* compiler)
{
    ASM_ASSERT;

    if (strcmp(node->left->value.name, SCAN) == 0)
    {
        WriteCall   (compiler, "scan");
        WritePushR64(compiler, RAX);
        return true;
    }

    if (strcmp(node->left->value.name, PRINT) == 0)
    {
        WriteAsmExpression(node->right->left, compiler);

        WriteCall  (compiler, "print");
        WritePopR64(compiler, RBX); //cleaning stack
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

    WritePopR64   (compiler, RAX);
    WritePopR64   (compiler, RBX);
    WriteCmpR64R64(compiler, RBX, RAX);

    size_t true_label     = compiler->label++;
    size_t continue_label = compiler->label++;

    size_t true_jump_offset = WriteJumpOp(compiler, node->value.op, true_label);

    WritePushNum(compiler, 0);
    size_t continue_jump_offset  = WriteJump (compiler, continue_label);
    size_t true_label_offset     = WriteLabel(compiler, true_label);

    WritePushNum(compiler, 1);
    size_t continue_label_offset = WriteLabel(compiler, continue_label);

    SetInt(&compiler->writer, true_label_offset     - true_jump_offset     - sizeof(int), true_jump_offset);
    SetInt(&compiler->writer, continue_label_offset - continue_jump_offset - sizeof(int), continue_jump_offset);
}

int GetVarOfs(Function* function, char* name)
{
    assert(function);
    assert(name);

    for (size_t i = 0; i < function->num_args; ++i)
    {
        if (strcmp(function->vars[i], name) == 0)
        {
            return (i + 2) * sizeof(int64_t);
        }
    }

    for (size_t i = function->num_args; i < function->num_vars + function->num_args; ++i)
    {
        if (strcmp(function->vars[i], name) == 0)
        {
            return - (i - function->num_args + 1) * sizeof(int64_t);
        }
    }
    
    printf("error : '%s' wasn't declarated here\n", name);
    return -0xFFFFFFFF;
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

    function->name     = subtree->value.name;
    function->is_void  = subtree->parent->value.is_void;

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

    int num = 0;

    if (node->type == DECL_TYPE || node->type == ADECL_TYPE)
    {
        num = 1;
    }

    return num + GetNumVars(node->left) + GetNumVars(node->right);
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

    GetVars(subtree->left->right, vars, &ofs);
}

void GetVars(Node* node, char** vars, size_t* ofs)
{
    assert(vars);
    assert(ofs);

    if (node == nullptr)
    {
        return;
    }

    if (node->type == DECL_TYPE || node->type == ADECL_TYPE)
    {
        vars[(*ofs)++] = node->left->value.name;
    }

    GetVars(node->right, vars, ofs);
    GetVars(node->left,  vars, ofs);
}

void NameTableDump(NameTable* table)
{
    assert(table);

    for (size_t i = 0; i < table->num_func; ++i)
    {
        printf("\n%s :\n", table->functions[i].name);
        printf("\tparams(%lu) : ", table->functions[i].num_args);

        for (size_t j = 0; j < table->functions[i].num_args; ++j)
        {
            printf("%s[%d], ", table->functions[i].vars[j], GetVarOfs(table->functions + i, table->functions[i].vars[j]));
        }
        printf("\n\tvars(%lu) : ", table->functions[i].num_vars);

        for (size_t j = table->functions[i].num_args; j < table->functions[i].num_args + 
                                                          table->functions[i].num_vars; ++j)
        {
            printf("%s[%d], ", table->functions[i].vars[j], GetVarOfs(table->functions + i, table->functions[i].vars[j]));
        }
    }
}
