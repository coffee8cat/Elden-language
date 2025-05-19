section .data
    minus     db '-'
    plus      db '+'
    dot       db '.'
    buffer    times 32 db 0  ; buffer for string
    precision equ 4          ; precision - 4 digits after '.'

section .text
    global print_double

print_double:
    push rbp
    mov  rbp, rsp

    push rbx
    push r11
    sub  rsp, 16

    mov  r11, __float64__(10.0)

    ; CHECK SIGN ====================================================
    movq   rax, xmm0
    bt     rax, 63
    jnc    .positive
    mov    byte [rel buffer], '-'
    jmp    .sign_done
.positive:
    mov    byte [rel buffer], '+'
.sign_done:

    ; EXTRACT INTEGER PART ==========================================
    cvttsd2si rdi, xmm0
    lea  rsi, [rel buffer + 1]
    call int_to_ascii

    ; EXTRACT FRACTIONAL PART =======================================
    movsd xmm1, xmm0
    cvtsi2sd xmm2, rdi
    subsd xmm1, xmm2
    mov   byte [rsi], '.'
    inc   rsi

    ; PREPARE FRACTIONAL PART - mul on 10^precision =================
    mov   ecx, precision
    movq  xmm3, r11
.fraction_prepare:
    mulsd xmm1, xmm3
    loop  .fraction_prepare

    ; convert to int
    cvttsd2si rax, xmm1

    ; CONVERT FRACTIONAL PART WITH PRECISION ========================
    mov   ecx, precision
.fraction_loop:
    xor   edx, edx
    div   r11                   ; rdx = last digit
    add   dl, '0'
    mov   [rsi], dl
    inc   rsi
    loop  .fraction_loop

    mov   byte [rsi], 10        ; add '\n'
    inc   rsi

    ; TYPE WITH SYSCALL =============================================
    mov   rax, 1                ; sys_write
    mov   rdi, 1                ; stdout
    mov   rdx, rsi
    lea   rsi, [rel buffer]     ; ptr to string
    sub   rdx, rsi              ; str length
    syscall

    add  rsp, 16                ; restore registers
    pop  r11
    pop  rbx
    leave
    ret

int_to_ascii:
    mov  rax, rdi
    mov  rcx, 10
    xor  rbx, rbx
    test rax, rax
    jns  .next_digit
    neg  rax
.next_digit:
    xor  rdx, rdx
    div  rcx
    add  dl, '0'
    push rdx
    inc  rbx
    test rax, rax
    jnz  .next_digit
.store_digits:
    pop  rax
    mov  [rsi], al
    inc  rsi
    dec  rbx
    jnz  .store_digits
    ret

section .note.GNU-stack noalloc noexec nowrite progbits
