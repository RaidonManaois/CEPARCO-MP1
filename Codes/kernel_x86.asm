section .text
global kernel_x86

; Windows x64 calling convention
; rcx = n
; rdx = A
; r8  = B
; r9  = C
; idx = [rsp+40]

kernel_x86:

    push rbp
    mov rbp, rsp

    mov r10, rcx        ; n
    mov r11, rdx        ; A
    mov r12, r8         ; B
    mov r13, r9         ; C
    mov r14, [rbp+48]   ; idx pointer

    xor r15, r15        ; i = 0

loop_start:

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
    jmp loop_start

done:
    pop rbp
    ret