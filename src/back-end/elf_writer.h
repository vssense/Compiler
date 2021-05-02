#ifndef ELF_WRITER_H
#define ELF_WRITER_H

#include <assert.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Function;

const int buffer_default_size = 2048;
const int calls_default_size  = 16;

struct CallInfo
{
    size_t offset;
    Function* function;
};

struct ElfFileWriter
{
    char*  buffer;
    size_t offset;
    size_t buffer_capacity;

    CallInfo* calls;
    size_t    calls_size;
    size_t    calls_capacity;

    Elf64_Ehdr header;
    Elf64_Phdr text_header;
    Elf64_Phdr data_header;
};

void   Construct      (ElfFileWriter* writer);
void   SetByte        (ElfFileWriter* writer, char byte);
char   LastByte       (ElfFileWriter* writer);
void   SetInt         (ElfFileWriter* writer, int value);
void   SetInt         (ElfFileWriter* writer, int value, size_t offset);
void   SetLong        (ElfFileWriter* writer, int64_t value);
void   AddCall        (ElfFileWriter* writer, size_t offset, Function* function);
void   InitElf64      (ElfFileWriter* writer);
size_t InitDataSegment(ElfFileWriter* writer);
void   SetElfHeaders  (ElfFileWriter* writer);
void   Destruct       (ElfFileWriter* writer);

#endif