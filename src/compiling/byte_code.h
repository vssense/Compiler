#pragma once

#include "compiling.h"

enum R64
{
    RAX, RCX, RDX, RBX, RSP, RBP, RSI, RDI,
    R8,  R9,  R10, R11, R12, R13, R14, R15
};

void WriteMovR64R64(Compiler* compiler, R64 reg1, R64 reg2);
void WriteMovR64Mem(Compiler* compiler, R64 reg1, R64 reg2, int offset = 0);
void WriteMovMemR64(Compiler* compiler, R64 reg1, R64 reg2, int offset = 0);
void WriteMovR64Num(Compiler* compiler, R64 reg, int64_t value);

void WriteAddR64R64(Compiler* compiler, R64 reg1, R64 reg2);
void WriteAddR64Num(Compiler* compiler, R64 reg, int value);
void WriteSubR64R64(Compiler* compiler, R64 reg1, R64 reg2);
void WriteSubR64Num(Compiler* compiler, R64 reg, int value);

void WriteMulR64   (Compiler* compiler, R64 reg);
void WriteDivR64   (Compiler* compiler, R64 reg);

void WritePushR64  (Compiler* compiler, R64 reg);
void WritePushNum  (Compiler* compiler, int value);
void WritePopR64   (Compiler* compiler, R64 reg);

void WriteSalR64   (Compiler* compiler, R64 reg, int offset);
void WriteCmpR64R64(Compiler* compiler, R64 reg1, R64 reg2);
void WriteTest     (Compiler* compiler, R64 reg1, R64 reg2);

size_t WriteJumpOp   (Compiler* compiler, int op, size_t label, size_t offset_to_jump = 0);
size_t WriteJump     (Compiler* compiler, size_t label, size_t offset_to_jump = 0);
size_t WriteLabel    (Compiler* compiler, size_t label);
void   WriteCall     (Compiler* compiler, Function* func);
void   WriteCall     (Compiler* compiler, const char* name);
void   WriteRet      (Compiler* compiler);
void   WriteFuncDecl (Compiler* compiler, Function* func);
void   WriteSyscall  (Compiler* compiler);
void   SetAllCalls   (Compiler* compiler);
