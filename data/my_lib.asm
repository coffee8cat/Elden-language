section .data
    minus     db '-'
    plus      db '+'
    dot       db '.'
    buffer    times 32 db 0  ; Буфер для строки
    precision equ 4          ; Точность (4 знака после запятой)
    ten       dq 10.0        ; Константа для умножения

section .text
    global print_double

print_double:
    push rbp
    mov  rbp, rsp
    push rbx
    push r12
    sub  rsp, 16

    ; --- Шаг 1: Проверка знака ---
    movq   rax, xmm0
    bt     rax, 63
    jnc    .positive
    mov    byte [rel buffer], '-'
    jmp    .sign_done
.positive:
    mov    byte [rel buffer], '+'
.sign_done:

    ; --- Шаг 2: Извлечение целой части ---
    cvttsd2si rdi, xmm0
    lea  rsi, [rel buffer + 1]
    call int_to_ascii

    ; --- Шаг 3: Извлечение дробной части ---
    movsd xmm1, xmm0
    cvtsi2sd xmm2, rdi
    subsd xmm1, xmm2
    mov   byte [rsi], '.'
    inc   rsi

    ; Умножаем на 10^precision один раз
    mov   ecx, precision
    movsd xmm3, [rel ten]
.fraction_prepare:
    mulsd xmm1, xmm3
    loop  .fraction_prepare

    ; Конвертируем в целое
    cvttsd2si rax, xmm1

    ; Преобразуем в строку с ведущими нулями
    mov   ecx, precision
    mov   r12, rsi           ; Сохраняем позицию начала дробной части
.fraction_loop:
    xor   edx, edx
    div   qword [rel ten]    ; RDX = последняя цифра
    add   dl, '0'
    mov   [rsi], dl
    inc   rsi
    loop  .fraction_loop

    ; Добавляем символ новой строки
    mov   byte [rsi], 10
    inc   rsi

    ; --- Шаг 4: Печать через syscall ---
    mov   rax, 1             ; sys_write
    mov   rdi, 1             ; stdout
    mov   rdx, rsi
    lea   rsi, [rel buffer]
    sub   rdx, rsi
    syscall

    add  rsp, 16
    pop  r12
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
