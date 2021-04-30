#include "byte_code.h"

typedef unsigned char byte;

const char* REGS[] = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi",
                      "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15"};
const char* JUMPS[]        = {"je", "jne", "jl", "jg", "jle", "jge"};
const byte JUMP_OP_CODES[] = {0x84,  0x85, 0x8c, 0x8f,  0x8e,  0x8d};

const int max_int  = 0xFFFFFFFF;
const int max_char = 0xFF;

const byte default_r64_rex = 0b01001000;

const byte reg1_additional_bit = 0b001;
const byte reg2_additional_bit = 0b100;

const byte mode_mask      = 0b11;
const byte reg_mask       = 0b111;
const byte long_mode_mask = 0b11111;

const byte additional_rsp_r12_byte  = 0x24;
const byte additional_push_pop_byte = 0x41;

const byte mov_r64_r64  = 0x89;
const byte mov_r64_mem  = 0x8b;
const byte mov_mem_r64  = 0x89;
const byte add_r64_r64  = 0x01;
const byte add_r64_num4 = 0x81;
const byte add_r64_num1 = 0x83;
const byte sub_r64_r64  = 0x29;
const byte sub_r64_num4 = 0x81;
const byte sub_r64_num1 = 0x83;
const byte mul_div_r64  = 0xf7;
const byte push_num4    = 0x68;
const byte push_num1    = 0x6a;
const byte sal_r64      = 0xc1;
const byte cmp_r64_r64  = 0x29;
const byte call_func    = 0xe8;
const byte ret_func     = 0xc3;
const byte test_r64_r64 = 0x85;
const byte jmp_4_bytes  = 0x0f;
const byte nop          = 0x90;
const byte jmp          = 0xe9;

const byte cqo_first_byte      = 0x48;
const byte cqo_second_byte     = 0x99;
const byte syscall_first_byte  = 0x0f;
const byte syscall_second_byte = 0x05;

const byte mode_mov_r64_num = 0b10111;
const byte mode_add_r64_num = 0b11000;
const byte mode_sub_r64_num = 0b11101;
const byte mode_mul_r64_num = 0b11100;
const byte mode_div_r64_num = 0b11110;
const byte mode_push_r64    = 0b01010;
const byte mode_pop_r64     = 0b01011;
const byte mode_sal_r64     = 0b11100;

const byte mode_r64_r64         = 0b11;
const byte mode_r64_mem_4_bytes = 0b10;
const byte mode_r64_mem_1_byte  = 0b01;

const int reg_code_len      = 3;
const int pop_reg_str_size  = 9;
const int push_reg_str_size = 10;

byte CodeRexByteR64R64(R64 reg1, R64 reg2)
{
    byte first_byte = default_r64_rex;

    if (reg1 > RDI)
    {
        first_byte |= reg1_additional_bit;
    }

    if (reg2 > RDI)
    {
        first_byte |= reg2_additional_bit;
    }

    return first_byte;
}

byte CodeRexByteR64(R64 reg)
{
    byte first_byte = default_r64_rex;

    if (reg > RDI)
    {
        first_byte |= reg1_additional_bit;
    }

    return first_byte;
}

byte CodeR64R64(byte mode, R64 reg1, R64 reg2)
{
    return (((mode & mode_mask) << 2 * reg_code_len) |
            ((reg1 & reg_mask) << reg_code_len) | (reg2 & reg_mask));
}

byte CodeR64(byte mode, R64 reg)
{
    return (((mode & long_mode_mask) << reg_code_len) | (reg & reg_mask));
}

void WriteMovR64R64(Compiler* compiler, R64 reg1, R64 reg2)
{
    assert(compiler);

    SetByte(&compiler->writer, CodeRexByteR64R64(reg1, reg2));

    SetByte(&compiler->writer, mov_r64_r64);

    SetByte(&compiler->writer, CodeR64R64(mode_r64_r64, reg2, reg1));

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "mov %s, %s\n\t", REGS[reg1], REGS[reg2]);
    }
}

void WriteMovR64Mem(Compiler* compiler, R64 reg1, R64 reg2, int offset)
{
    assert(compiler);

    SetByte(&compiler->writer, CodeRexByteR64R64(reg1, reg2));
    SetByte(&compiler->writer, mov_r64_mem);
    SetByte(&compiler->writer, CodeR64R64(mode_r64_mem_4_bytes, reg1, reg2));

    if (reg2 == RSP || reg2 == R12)
    {
        SetByte(&compiler->writer, additional_rsp_r12_byte);
    }

    SetInt(&compiler->writer, offset);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "mov %s, [%s + %d]\n\t", REGS[reg1], REGS[reg2], offset);
    }
}

void WriteMovMemR64(Compiler* compiler, R64 reg1, R64 reg2, int offset)
{
    assert(compiler);

    SetByte(&compiler->writer, CodeRexByteR64R64(reg2, reg1));
    SetByte(&compiler->writer, mov_mem_r64);
    SetByte(&compiler->writer, CodeR64R64(mode_r64_mem_4_bytes, reg2, reg1));

    if (reg1 == RSP || reg1 == R12)
    {
        SetByte(&compiler->writer, additional_rsp_r12_byte);
    }

    SetInt(&compiler->writer, offset);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "mov [%s + %d], %s\n\t", REGS[reg1], offset, REGS[reg2]);
    }
}

void WriteMovR64Num(Compiler* compiler, R64 reg, int64_t value)
{
    assert(compiler);

    if (compiler->memory_optimization_required && abs(value) < max_int)
    {
        if (reg > RDI)
        {
            SetByte(&compiler->writer, additional_push_pop_byte);
        }

        SetByte(&compiler->writer, CodeR64(mode_mov_r64_num, reg));
        SetInt(&compiler->writer, value);
    }
    else
    {
        SetByte(&compiler->writer, CodeRexByteR64(reg));
        SetByte(&compiler->writer, CodeR64(mode_mov_r64_num, reg));
        SetLong(&compiler->writer, value);
    }

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "mov %s, %ld\n\t", REGS[reg], value);
    }
}

void WriteAddR64R64(Compiler* compiler, R64 reg1, R64 reg2)
{
    assert(compiler);

    SetByte(&compiler->writer, CodeRexByteR64R64(reg1, reg2));
    SetByte(&compiler->writer, add_r64_r64);
    SetByte(&compiler->writer, CodeR64R64(mode_r64_r64, reg2, reg1));

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "add %s, %s\n\t", REGS[reg1], REGS[reg2]);
    }
}

void WriteAddR64Num(Compiler* compiler, R64 reg, int value)
{
    assert(compiler);

    SetByte(&compiler->writer, CodeRexByteR64(reg));

    if (compiler->memory_optimization_required && abs(value) < max_char)
    {
        SetByte(&compiler->writer, add_r64_num1);
        SetByte(&compiler->writer, CodeR64(mode_add_r64_num, reg));

        SetByte(&compiler->writer, value);
    }
    else
    {
        SetByte(&compiler->writer, add_r64_num4);
        SetByte(&compiler->writer, CodeR64(mode_add_r64_num, reg));

        SetInt(&compiler->writer, value);
    }

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "add %s, %d\n\t", REGS[reg], value);
    }
}

void WriteSubR64R64(Compiler* compiler, R64 reg1, R64 reg2)
{
    assert(compiler);

    SetByte(&compiler->writer, CodeRexByteR64R64(reg1, reg2));
    SetByte(&compiler->writer, sub_r64_r64);
    SetByte(&compiler->writer, CodeR64R64(mode_r64_r64, reg2, reg1));

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "sub %s, %s\n\t", REGS[reg1], REGS[reg2]);
    }
}

void WriteSubR64Num(Compiler* compiler, R64 reg, int value)
{
    assert(compiler);

    SetByte(&compiler->writer, CodeRexByteR64(reg));
    
    if (compiler->memory_optimization_required && abs(value) < max_char)
    {
        SetByte(&compiler->writer, sub_r64_num1);
        SetByte(&compiler->writer, CodeR64(mode_sub_r64_num, reg));

        SetByte(&compiler->writer, value);
    }
    else
    {
        SetByte(&compiler->writer, sub_r64_num4);
        SetByte(&compiler->writer, CodeR64(mode_sub_r64_num, reg));

        SetInt(&compiler->writer, value);
    }

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "sub %s, %d\n\t", REGS[reg], value);
    }
}

void WriteMulR64(Compiler* compiler, R64 reg)
{
    assert(compiler);

    SetByte(&compiler->writer, CodeRexByteR64(reg));
    SetByte(&compiler->writer, mul_div_r64);
    SetByte(&compiler->writer, CodeR64(mode_mul_r64_num, reg));

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "imul %s\n\t", REGS[reg]);
    }
}

void WriteDivR64(Compiler* compiler, R64 reg)
{
    assert(compiler);

    SetByte(&compiler->writer, cqo_first_byte);
    SetByte(&compiler->writer, cqo_second_byte);

    SetByte(&compiler->writer, CodeRexByteR64(reg));
    SetByte(&compiler->writer, mul_div_r64);
    SetByte(&compiler->writer, CodeR64(mode_div_r64_num, reg));

    if (compiler->asm_listing_required)
    {    
        fprintf(compiler->file, "cqo    \n\t"
                                "idiv %s\n\t", REGS[reg]);
    }
}

void WritePushR64(Compiler* compiler, R64 reg)
{
    assert(compiler);

    if (compiler->speed_optimization_required &&
        CodeR64(mode_pop_r64, reg) == LastByte(&compiler->writer))
    {
        if (compiler->asm_listing_required)
        {
            fseek(compiler->file, -pop_reg_str_size, SEEK_CUR);
        }

        compiler->writer.offset--;
        
        if (LastByte(&compiler->writer) == additional_push_pop_byte)
        {
            compiler->writer.offset--;
        }

        return;
    }

    if (reg > RDI)
    {
        SetByte(&compiler->writer, additional_push_pop_byte);
    }

    SetByte(&compiler->writer, CodeR64(mode_push_r64, reg));

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "push %s\n\t", REGS[reg]);
    }
}

void WritePushNum(Compiler* compiler, int value)
{
    assert(compiler);

    if (compiler->memory_optimization_required && abs(value) < max_char)
    {
        SetByte(&compiler->writer, push_num1);
        SetByte(&compiler->writer, value); 
    }
    else
    {
        SetByte(&compiler->writer, push_num4);
        SetInt(&compiler->writer, value);
    }

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "push %d\n\t", value);
    }
}

void WritePopR64(Compiler* compiler, R64 reg)
{
    assert(compiler);

    if (compiler->speed_optimization_required &&
        CodeR64(mode_push_r64, reg) == LastByte(&compiler->writer))
    {
        if (compiler->asm_listing_required)
        {
            fseek(compiler->file, -push_reg_str_size, SEEK_CUR);
        }

        compiler->writer.offset--;
        
        if (LastByte(&compiler->writer) == additional_push_pop_byte)
        {
            compiler->writer.offset--;
        }

        return;
    }


    if (reg > RDI)
    {
        SetByte(&compiler->writer, additional_push_pop_byte);
    }

    SetByte(&compiler->writer, CodeR64(mode_pop_r64, reg));

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "pop %s\n\t", REGS[reg]);
    }
}

void WriteSalR64(Compiler* compiler, R64 reg, int offset)
{
    assert(compiler);

    SetByte(&compiler->writer, CodeRexByteR64(reg));
    SetByte(&compiler->writer, sal_r64);
    SetByte(&compiler->writer, CodeR64(mode_sal_r64, reg));
    SetByte(&compiler->writer, (char)offset);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "sal %s, %d\n\t", REGS[reg], offset);
    }
}

void WriteCmpR64R64(Compiler* compiler, R64 reg1, R64 reg2)
{
    assert(compiler);

    SetByte(&compiler->writer, CodeRexByteR64R64(reg1, reg2));
    SetByte(&compiler->writer, cmp_r64_r64);
    SetByte(&compiler->writer, CodeR64R64(mode_r64_r64, reg2, reg1));

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "cmp %s, %s\n\t", REGS[reg1], REGS[reg2]);
    }
}

void WriteTest(Compiler* compiler, R64 reg1, R64 reg2)
{
    assert(compiler);

    SetByte(&compiler->writer, CodeRexByteR64R64(reg1, reg2));
    SetByte(&compiler->writer, test_r64_r64);
    SetByte(&compiler->writer, CodeR64R64(mode_r64_r64, reg2, reg1));

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "test %s, %s\n\t", REGS[reg1], REGS[reg2]);
    }
}

size_t WriteJumpOp(Compiler* compiler, int op, size_t label, size_t offset_to_jump)
{
    assert(compiler);

    SetByte(&compiler->writer, jmp_4_bytes);
    SetByte(&compiler->writer, JUMP_OP_CODES[op - EQUAL_OP]);
    SetInt(&compiler->writer, offset_to_jump - compiler->writer.offset - sizeof(int));

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "%s L%lu\n\t", JUMPS[op - EQUAL_OP], label);
    }

    return compiler->writer.offset - sizeof(int);
}

size_t WriteJump(Compiler* compiler, size_t label, size_t offset_to_jump)
{
    assert(compiler);

    SetByte(&compiler->writer, jmp);
    SetInt(&compiler->writer, offset_to_jump - compiler->writer.offset - sizeof(int));

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "jmp L%lu\n\t", label);
    }
    
    return compiler->writer.offset - sizeof(int);
}

size_t WriteLabel(Compiler* compiler, size_t label)
{
    assert(compiler);

    if (compiler->asm_listing_required)
    {
        fseek(compiler->file, -1, SEEK_CUR);
        fprintf(compiler->file, "L%lu :\n\t", label);
    }

    return compiler->writer.offset;
}

void WriteCall(Compiler* compiler, Function* func)
{
    assert(compiler);
    assert(func);

    SetByte(&compiler->writer, call_func);
    AddCall(&compiler->writer, compiler->writer.offset, func);
    SetInt(&compiler->writer, 0);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "call %s\n\t", func->name);
    }
}

void WriteCall(Compiler* compiler, const char* name)
{
    assert(compiler);
    assert(name);

    SetByte(&compiler->writer, call_func);

    if (strcmp(name, "print") == 0)
    {
        SetInt(&compiler->writer, compiler->print_offset - compiler->writer.offset - sizeof(int));
    }
    else
    {
        SetInt(&compiler->writer, compiler->scan_offset - compiler->writer.offset - sizeof(int));
    }

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "call %s\n\t", name);
    }
}

void WriteRet(Compiler* compiler)
{
    assert(compiler);

    SetByte(&compiler->writer, ret_func);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "ret\n\t");
    }
}

void WriteFuncDecl(Compiler* compiler, Function* func)
{
    assert(compiler);

    SetByte(&compiler->writer, nop);

    func->offset = compiler->writer.offset;

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "\n%s :\n\t", func->name);
    }
}

void WriteSyscall(Compiler* compiler)
{
    assert(compiler);

    SetByte(&compiler->writer, syscall_first_byte);
    SetByte(&compiler->writer, syscall_second_byte);

    if (compiler->asm_listing_required)
    {
        fprintf(compiler->file, "syscall\n\t");
    }
}

void SetAllCalls(Compiler* compiler)
{
    assert(compiler);

    for (size_t i = 0; i < compiler->writer.calls_size; ++i)
    {
        int* tmp_ptr = (int*)(compiler->writer.buffer + compiler->writer.calls[i].offset);
        *tmp_ptr = compiler->writer.calls[i].function->offset - compiler->writer.calls[i].offset - sizeof(int);
    }
}
