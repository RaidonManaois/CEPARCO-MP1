section .text
global kernel_ymm

kernel_ymm:

    mov r10, rcx
    mov r11, rdx
    mov r12, r8
    mov r13, r9

    xor r14, r14

loop_ymm:

    cmp r14, r10
    jge done

    vmovups ymm0, [r11 + r14*4]
    vmovups ymm1, [r12 + r14*4]

    vmaxps ymm2, ymm0, ymm1

    vmovups [r13 + r14*4], ymm2

    add r14, 8
    jmp loop_ymm

done:
    vzeroupper
    ret