const char* nasm_print_func =
                         "\nprint:                \n\t"
                         "mov rsi, IO_BUFFER      \n\t"
                
                         "xor rdi, rdi            \n\t"
                         "mov [rsi], rdi          \n\t"
                         "mov [rsi + 8], rdi      \n\t"
                         "mov rax, [rsp + 8]      \n\t"
                         "cmp rax, 0              \n\t"
                         "jge print_continue      \n\t"

                         "neg rax                 \n\t"
                         "mov rdi, 1              \n"

                    "print_continue:              \n\t"
                        "add rsi, 15              \n\t"
                        "mov byte [rsi], '0'      \n\t"
                        "mov rbx, 10              \n"

                    "print_while:                 \n\t"
                         "test rax, rax           \n\t"
                         "jz print_while_end      \n\t"

                         "cqo                     \n\t"
                         "div rbx                 \n\t"
                         "add rdx, '0'            \n\t"
                         "mov byte [rsi], dl      \n\t"
                         "dec rsi                 \n\t"
                         "jmp print_while         \n"
                    "print_while_end:             \n\t"
                         "test rdi, rdi           \n\t"
                         "jz print_positive       \n\t"
                         "dec rsi                 \n\t"
                         "mov byte [rsi], '-'     \n"

                    "print_positive:              \n\t"

                         "mov rdx, 16             \n\t"
                         "mov rdi, 1              \n\t"
                         "mov rax, 1              \n\t"
                         "syscall                 \n\t"
                         "ret                     \n\t";

const char* nasm_scan_func =
                        "\nscan:                  \n\t"
                        "xor rax, rax             \n\t"
                        "xor rdi, rdi             \n\t"
                        "mov rsi, IO_BUFFER       \n\t"
                        "mov [rsi], rdi           \n\t"
                        "mov [rsi + 8], rdi       \n\t"
                        "mov rdx, 15              \n\t"
                        "syscall                  \n\t"

                        "mov rdi, 1               \n\t"
                        "cmp byte [rsi], '-'      \n\t"
                        "jne scan_continue        \n\t"
                        "inc rsi                  \n\t"
                        "mov rdi, -1              \n"
                    "scan_continue:               \n\t"
                        "xor rax, rax             \n\t"
                        "mov rbx, 10              \n"
                    "scan_while:                  \n\t"
                        "xor rcx, rcx             \n\t"
                        "mov cl, byte [rsi]       \n\t"
                        "cmp cl, '0'              \n\t"
                        "jl scan_while_end        \n\t"

                        "cmp cl, '9'              \n\t"
                        "jg scan_while_end        \n\t"

                        "mul rbx                  \n\t"
                        "sub cl, '0'              \n\t"
                        "add rax, rcx             \n\t"
                        "inc rsi                  \n\t"
                        "jmp scan_while           \n"
                    "scan_while_end:              \n\t"

                        "mul rdi                  \n\t"
                        "ret                      \n\t";

const char* asm_data = "\nsection .data           \n"
                       "IO_BUFFER:                \n"
                       "times 16 db 0             \n"
                       "         db ' '           \n";

const char byte_code_print[] = 
"\x48\xbe\x00\x20\x40\x00\x00\x00\x00\x00\x48\x31\xff\x48\x89\x3e"
"\x48\x89\x7e\x08\x48\x8b\x44\x24\x08\x48\x83\xf8\x00\x7d\x08\x48"
"\xf7\xd8\xbf\x01\x00\x00\x00\x48\x83\xc6\x0f\xc6\x06\x30\xbb\x0a"
"\x00\x00\x00\x48\x85\xc0\x74\x10\x48\x99\x48\xf7\xf3\x48\x83\xc2"
"\x30\x88\x16\x48\xff\xce\xeb\xeb\x48\x85\xff\x74\x06\x48\xff\xce"
"\xc6\x06\x2d\xba\x10\x00\x00\x00\xbf\x01\x00\x00\x00\xb8\x01\x00"
"\x00\x00\x0f\x05\xc3";

const char byte_code_scan[] =
"\x48\xbe\x00\x20\x40\x00\x00\x00\x00\x00\x48\x31\xc0\x48\x31\xff"
"\x48\x89\x3e\x48\x89\x7e\x08\xba\x0f\x00\x00\x00\x0f\x05\xbf\x01"
"\x00\x00\x00\x80\x3e\x2d\x75\x0a\x48\xff\xc6\x48\xc7\xc7\xff\xff"
"\xff\xff\x48\x31\xc0\xbb\x0a\x00\x00\x00\x48\x31\xc9\x8a\x0e\x80"
"\xf9\x30\x7c\x13\x80\xf9\x39\x7f\x0e\x48\xf7\xe3\x80\xe9\x30\x48"
"\x01\xc8\x48\xff\xc6\xeb\xe3\x48\xf7\xe7\xc3";

const char byte_code_std_data[] =
"\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00"
"\x20";
