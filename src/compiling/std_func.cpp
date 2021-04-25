
const char* print_func = "\nprint:                \n\t"
                         "mov rsi, IO_BUFFER + 15 \n\t"
                
                         "xor rdi, rdi            \n\t"
                         "mov rax, [rsp + 8]      \n\t"
                         "cmp rax, 0              \n\t"
                         "jge print_continue      \n\t"

                         "neg rax                 \n\t"
                         "mov rdi, 1              \n"

                    "print_continue:              \n\t"
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

const char* scan_func = "\nscan:                  \n\t"
                        "xor rax, rax             \n\t"
                        "xor rdi, rdi             \n\t"
                        "mov rsi, IO_BUFFER       \n\t"
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
                        "cmp cl, 0xa              \n\t"
                        "je scan_while_end        \n\t"

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
