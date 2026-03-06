section .text
global kernel_xmm

kernel_xmm:

    mov r10, rcx    ; n
    mov r11, rdx    ; A
    mov r12, r8     ; B
    mov r13, r9     ; C

    xor r14, r14

loop_simd:

    cmp r14, r10
    jge done

    vmovups xmm0, [r11 + r14*4]
    vmovups xmm1, [r12 + r14*4]

    vmaxps xmm2, xmm0, xmm1

    vmovups [r13 + r14*4], xmm2

    add r14, 4
    jmp loop_simd

done:
    ret