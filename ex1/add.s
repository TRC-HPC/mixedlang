    .globl add          # export symbol to linker
    .text               # start code section

add:                    # int add(int i, int j);
    movl %edi, %eax     # return value in eax = 1st arg in edi
    addl %esi, %eax     # eax += 2nd arg in esi
    ret
    .section    .note.GNU-stack,"",@progbits
