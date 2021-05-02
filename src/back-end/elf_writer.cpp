#include "elf_writer.h"

const int elf_magic                   = 0x464c457f;
const int elf_mode_offset             = 4;
const unsigned char elf_64bit_mode    = 0x02;
const int elf_encoding_offset         = 5;
const unsigned char elf_little_endian = 0x01;
const int elf_version_offset          = 6;
const unsigned char elf_version       = 0x01;
const uint16_t elf_executable         = 0x02;
const uint16_t elf_machine_amd64      = 0x3e;
const uint32_t elf_ibj_version        = 0x01;
const uint32_t virtual_start          = 0x400000;
const uint64_t elf_entry              = 0x401000;
const uint64_t elf_text_offset        = 0x1000;
const uint32_t elf_data_size          = 0x100;
const uint64_t elf_segment_alignment  = 0xFFF;

const int num_segments = 2;

bool WriterOk(ElfFileWriter* writer, const char* function)
{
    bool is_ok = true;
    if (writer == nullptr)
    {
        printf("writer is nullptr\n");
        return false;
    }

    if (writer->calls == nullptr)
    {
        printf("calls is nullptr\n");
        is_ok = false;
    }

    if (writer->canary1 != canary1_check)
    {
        printf("first canary dead(%lu)(\n", writer->canary1);
        is_ok = false;
    }

    if (writer->canary2 != canary2_check)
    {
        printf("second canary dead(%lu)\n", writer->canary2);
        is_ok = false;
    }

    if (writer->offset > writer->buffer_capacity)
    {
        printf("offset > buffer_capacity\n");
        is_ok = false;
    }

    if (writer->calls_size > writer->calls_capacity)
    {
        printf("calls_size  > calls_capacity\n");
        is_ok = false;
    }

    for (size_t i = 0; i < writer->calls_size; ++i)
    {
        if (writer->calls[i].function == nullptr)
        {
            printf("function[%lu] pointer is nullptr\n", i);
            break;
        }

        if (writer->calls[i].offset > writer->offset)
        {
            printf("incorrect offset[%lu] value = %lu\n", i, writer->calls[i].offset);
            break;
        }
    }

    if (!is_ok)
    {
        printf("function from calld = %s\n", function);
    }

    return is_ok;
}

void DumpWriter(ElfFileWriter* writer)
{
    printf(
    "canary1         = %lu (%lu)\n"
    "buffer          = %p \n"
    "offset          = %lu[0x%lx]\n"
    "buffer_capacity = %lu[0x%lx]\n"
    "calls           = %p \n"
    "calls_size      = %lu\n"
    "calls_capacity  = %lu\n"
    "canary2         = %lu (%lu)\n",
    writer->canary1, canary1_check,
    writer->buffer,
    writer->offset, writer->offset,
    writer->buffer_capacity, writer->buffer_capacity,
    writer->calls,
    writer->calls_size,
    writer->calls_capacity,
    writer->canary2, canary2_check);

    for (size_t i = 0; i < writer->calls_capacity; ++i)
    {
        if (i < writer->calls_size)
        {
            printf(" [%lu] offset = %lu, pointer = %p\n", i,
                   writer->calls[i].offset, writer->calls[i].function);
        }
        else
        {
            printf("*[%lu] offset = %lu, pointer = %p\n", i,
                   writer->calls[i].offset, writer->calls[i].function);
        }
    }


}

void InitElf64(ElfFileWriter* writer)
{
    ASSERT_WRITER;

    int* tmp_ptr = (int*)writer->header.e_ident;
    *tmp_ptr = elf_magic;

    writer->header.e_ident[elf_mode_offset] = elf_64bit_mode;
    writer->header.e_ident[elf_encoding_offset] = elf_little_endian;
    writer->header.e_ident[elf_version_offset] = elf_version;

    writer->header.e_type    = elf_executable;
    writer->header.e_machine = elf_machine_amd64;
    writer->header.e_version = elf_ibj_version;

    writer->header.e_entry     = elf_entry; 
    writer->header.e_phoff     = sizeof(Elf64_Ehdr); 
    writer->header.e_flags     = 0;
    writer->header.e_ehsize    = sizeof(Elf64_Ehdr);
    writer->header.e_phentsize = sizeof(Elf64_Phdr);
    writer->header.e_phnum     = num_segments;
    writer->header.e_shnum     = 0;

    writer->text_header.p_type   = PT_LOAD;
    writer->text_header.p_flags  = PF_X | PF_R;
    writer->text_header.p_offset = elf_text_offset;
    writer->text_header.p_vaddr  = elf_entry;

    writer->text_header.p_paddr  = 0;
    writer->text_header.p_align  = 0;

    writer->data_header.p_type   = PT_LOAD;
    writer->data_header.p_flags  = PF_W | PF_R;

    writer->data_header.p_paddr  = 0;
    writer->data_header.p_filesz = elf_data_size;
    writer->data_header.p_memsz  = elf_data_size;
    writer->data_header.p_align  = 0;

    writer->offset = elf_text_offset;
}

size_t InitDataSegment(ElfFileWriter* writer)
{
    ASSERT_WRITER;
    
    writer->text_header.p_filesz = writer->offset - elf_text_offset;
    writer->text_header.p_memsz  = writer->offset - elf_text_offset;

    size_t data_segment_offset = writer->offset & ~elf_segment_alignment;
    data_segment_offset       += elf_text_offset;
    writer->offset             = data_segment_offset;

    if (writer->buffer_capacity < writer->offset + elf_data_size)
    {
        writer->buffer_capacity = writer->offset + elf_data_size + 1;
        writer->buffer          = (char*)realloc(writer->buffer,
                                                 writer->buffer_capacity * sizeof(char));
        assert(writer->buffer);
    }

    writer->data_header.p_offset = data_segment_offset;
    writer->data_header.p_vaddr  = virtual_start + data_segment_offset;

    return data_segment_offset;
}

void SetElfHeaders(ElfFileWriter* writer)
{
    ASSERT_WRITER;

    memcpy(writer->buffer, &writer->header, sizeof(Elf64_Ehdr) + num_segments * sizeof(Elf64_Phdr));
}

void Construct(ElfFileWriter* writer)
{
    writer->offset = 0;
    writer->buffer = (char*)calloc(buffer_default_size, sizeof(char));
    writer->buffer_capacity = buffer_default_size;

    writer->calls = (CallInfo*)calloc(calls_default_size, sizeof(CallInfo));
    writer->calls_size = 0;
    writer->calls_capacity = calls_default_size;
}

void CheckElfWriterBuffer(ElfFileWriter* writer)
{
    ASSERT_WRITER;

    if (writer->offset >= writer->buffer_capacity - sizeof(int64_t))
    {
        writer->buffer_capacity *= 2;
        writer->buffer = (char*)realloc(writer->buffer,
                                        writer->buffer_capacity * sizeof(char));
        assert(writer->buffer);
    }
}

void CheckElfWriterCallsBuffer(ElfFileWriter* writer)
{
    ASSERT_WRITER;

    if (writer->calls_size >= writer->calls_capacity - 1)
    {
        writer->calls_capacity *= 2;
        writer->calls = (CallInfo*)realloc(writer->calls,
                                           writer->calls_capacity * sizeof(CallInfo));
        assert(writer->calls);
    }
}

void SetByte(ElfFileWriter* writer, char byte)
{
    ASSERT_WRITER;

    CheckElfWriterBuffer(writer);

    writer->buffer[writer->offset++] = byte;
}

char LastByte(ElfFileWriter* writer)
{
    ASSERT_WRITER;

    if (writer->offset > 0)
    {
        return writer->buffer[writer->offset - 1];
    }

    return 0xFF;
}

void SetInt(ElfFileWriter* writer, int value)
{
    ASSERT_WRITER;

    CheckElfWriterBuffer(writer);

    int* tmp_ptr = (int*)(writer->buffer + writer->offset);
    *tmp_ptr = value;
    writer->offset += sizeof(int);
}

void SetInt(ElfFileWriter* writer, int value, size_t offset)
{
    ASSERT_WRITER;

    int* tmp_ptr = (int*)(writer->buffer + offset);
    *tmp_ptr = value;
}

void SetLong(ElfFileWriter* writer, int64_t value)
{
    ASSERT_WRITER;

    CheckElfWriterBuffer(writer);

    long long* tmp_ptr = (long long*)(writer->buffer + writer->offset);
    *tmp_ptr = value;
    writer->offset += sizeof(long long);
}

void AddCall(ElfFileWriter* writer, size_t offset, Function* function)
{
    ASSERT_WRITER;
    assert(function);

    CheckElfWriterCallsBuffer(writer);

    writer->calls[writer->calls_size++] = {offset, function};
}

void Destruct(ElfFileWriter* writer)
{
    ASSERT_WRITER;

    assert(writer->buffer);
    free(writer->buffer);
    writer->buffer          = nullptr;
    writer->offset          = 0;
    writer->buffer_capacity = 0;

    assert(writer->calls);
    free(writer->calls);
    writer->calls          = nullptr;
    writer->calls_size     = 0;
    writer->calls_capacity = 0;
}