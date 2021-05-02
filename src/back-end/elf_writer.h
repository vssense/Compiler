#ifndef ELF_WRITER_H
#define ELF_WRITER_H

#include <assert.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Function;

const int buffer_default_size = 8192;
const int calls_default_size  = 16;

struct CallInfo
{
    size_t offset;
    Function* function;
};

// #define WRITER_DEBUG

#ifdef  WRITER_DEBUG
const size_t canary1_check = 0xDED32DED32DED999;
const size_t canary2_check = 0xDEADDEADDEADDEAD;

#define ASSERT_WRITER if (!WriterOk(writer, __func__)) \
                      {                      \
                         DumpWriter(writer); \
                         assert(0);          \
                      }
#else
#define ASSERT_WRITER
#endif

struct ElfFileWriter
{
    #ifdef WRITER_DEBUG
    size_t canary1 = canary1_check;
    #endif

    char*  buffer;
    size_t offset;
    size_t buffer_capacity;

    CallInfo* calls;
    size_t    calls_size;
    size_t    calls_capacity;

    Elf64_Ehdr header;
    Elf64_Phdr text_header;
    Elf64_Phdr data_header;

    #ifdef WRITER_DEBUG
    size_t canary2 = canary2_check;
    #endif
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