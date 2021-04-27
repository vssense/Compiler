#include "byte_code.h"

const char* REGS[] = { "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi"
                       "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15"};
const char* JUMPS[] = { "je", "jne", "jl", "jg", "jle", "jge" };

void WriteMovR64R64(Compiler* compiler, R64 reg1, R64 reg2)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "mov %s, %s\n\t", REGS[reg1], REGS[reg2]);
    }
}

void WriteMovR64Mem(Compiler* compiler, R64 reg1, R64 reg2, int offset)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "mov %s, [%s + %d]\n\t", REGS[reg1], REGS[reg2], offset);
    }
}

void WriteMovMemR64(Compiler* compiler, R64 reg1, R64 reg2, int offset)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "mov [%s + %d], %s\n\t", REGS[reg1], offset, REGS[reg2]);
    }
}

void WriteMovR64Num(Compiler* compiler, R64 reg, int64_t value)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "mov %s, %ld\n\t", REGS[reg], value);
    }
}

void WriteAddR64R64(Compiler* compiler, R64 reg1, R64 reg2)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "add %s, %s\n\t", REGS[reg1], REGS[reg2]);
    }
}

void WriteAddR64Num(Compiler* compiler, R64 reg, int64_t value)
{
    assert(compiler);

    fprintf(compiler->file, "add %s, %ld\n\t", REGS[reg], value);
}

void WriteSubR64R64(Compiler* compiler, R64 reg1, R64 reg2)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "sub %s, %s\n\t", REGS[reg1], REGS[reg2]);
    }
}

void WriteSubR64Num(Compiler* compiler, R64 reg, int64_t value)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "sub %s, %ld\n\t", REGS[reg], value);
    }
}

void WriteMulR64(Compiler* compiler, R64 reg)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "imul %s\n\t", REGS[reg]);
    }
}

void WriteDivR64(Compiler* compiler, R64 reg)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {    
        fprintf(compiler->file, "cqo    \n\t"
                                "idiv %s\n\t", REGS[reg]);
    }
}

void WritePushR64(Compiler* compiler, R64 reg)
{
    assert(compiler);
    
    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "push %s\n\t", REGS[reg]);
    }
}

void WritePushNum(Compiler* compiler, int64_t value)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "push %ld\n\t", value);
    }
}

void WritePopR64(Compiler* compiler, R64 reg)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "pop %s\n\t", REGS[reg]);
    }
}

void WriteSalR64(Compiler* compiler, R64 reg, int offset)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "sal %s, %d\n\t", REGS[reg], offset);
    }
}

void WriteCmpR64R64(Compiler* compiler, R64 reg1, R64 reg2)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "cmp %s, %s\n\t", REGS[reg1], REGS[reg2]);
    }
}

void WriteJumpOp(Compiler* compiler, int op, size_t label)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "%s L%lu\n\t", JUMPS[op - EQUAL_OP], label);
    }
}

void WriteJump(Compiler* compiler, size_t label)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "jmp L%lu\n\t", label);
    }
}

void WriteLabel(Compiler* compiler, size_t label)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fseek(compiler->file, -1, SEEK_CUR);
        fprintf(compiler->file, "L%lu :\n\t", label);
    }
}

void WriteCall(Compiler* compiler, Function* func)
{
    assert(compiler);
    assert(func);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "call %s\n\t", func->name);
    }
}

void WriteCall(Compiler* compiler, const char* name)
{
    assert(compiler);
    assert(name);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "call %s\n\t", name);
    }
}

void WriteRet(Compiler* compiler)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "ret\n\t");
    }
}

void WriteTest(Compiler* compiler, R64 reg1, R64 reg2)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "test %s, %s\n\t", REGS[reg1], REGS[reg2]);
    }
}

void WriteFuncDecl(Compiler* compiler, const char* name)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "\n%s :\n\t", name);
    }
}
