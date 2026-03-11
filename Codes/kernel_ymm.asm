section .text
global kernel_ymm

kernel_ymm:

    push rbp
    mov rbp, rsp

    mov r10, rcx
    mov r11, rdx
    mov r12, r8
    mov r13, r9
    mov r14, [rbp+48]

    xor r15, r15

; ----------------------
; SIMD LOOP (8 floats)
; ----------------------
simd_loop:

    mov rax, r10
    sub rax, 8
    cmp r15, rax
    jg scalar_loop

    vmovups ymm0, [r11 + r15*4]
    vmovups ymm1, [r12 + r15*4]

    vmaxps ymm2, ymm0, ymm1

    vmovups [r13 + r15*4], ymm2

    add r15, 8
    jmp simd_loop

; REMAINDER LOOP
scalar_loop:

    cmp r15, r10
    jge done

    movss xmm0, [r11 + r15*4]
    movss xmm1, [r12 + r15*4]

    comiss xmm0, xmm1
    jb b_larger

a_larger:
    movss [r13 + r15*4], xmm0
    mov dword [r14 + r15*4], 0
    jmp next

b_larger:
    movss [r13 + r15*4], xmm1
    mov dword [r14 + r15*4], 1

next:
    inc r15
    jmp scalar_loop

done:
    vzeroupper
    pop rbp
    ret