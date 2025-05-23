section .text

global main
extern print_double
extern elem_in
extern elem_out
extern scanf
main:
	; scanf start ;===================================================
	sub rsp, 8                         ; align stack by 16 before call
	lea rdi, [rel double_format]       ; format string
	lea rsi, [rel double_var]          ; pointer to var
	xor eax, eax                       ; num of XMM regs (by ABI)
	call scanf  wrt ..plt
	add rsp, 8                         ; restore stack

	; store result in stack
	sub rsp, 8
	movsd [rsp], xmm0

	movsd xmm0, [rsp]
	add   rsp, 8
	movsd [rel aCoeff], xmm0
	; scanf end ;=====================================================

	; scanf start ;===================================================
	sub rsp, 8                         ; align stack by 16 before call
	lea rdi, [rel double_format]       ; format string
	lea rsi, [rel double_var]          ; pointer to var
	xor eax, eax                       ; num of XMM regs (by ABI)
	call scanf  wrt ..plt
	add rsp, 8                         ; restore stack

	; store result in stack
	sub rsp, 8
	movsd [rsp], xmm0

	movsd xmm0, [rsp]
	add   rsp, 8
	movsd [rel bCoeff], xmm0
	; scanf end ;=====================================================

	; scanf start ;===================================================
	sub rsp, 8                         ; align stack by 16 before call
	lea rdi, [rel double_format]       ; format string
	lea rsi, [rel double_var]          ; pointer to var
	xor eax, eax                       ; num of XMM regs (by ABI)
	call scanf  wrt ..plt
	add rsp, 8                         ; restore stack

	; store result in stack
	sub rsp, 8
	movsd [rsp], xmm0

	movsd xmm0, [rsp]
	add   rsp, 8
	movsd [rel cCoeff], xmm0
	; scanf end ;=====================================================

	;calculating expression for assignment
	; push call params
	movsd xmm0, [rel aCoeff]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rel bCoeff]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rel cCoeff]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	call SQSOLVER
	add rsp, 24                ; clear stack
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; CALL END

	movsd xmm0, [rsp]          ; assignment to numofanswers
	add   rsp, 8
	movsd  [rel numofanswers], xmm0
	; prepare for printf

	movsd xmm0, [rel numofanswers]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]
	add   rsp, 8
	mov rax, rsp               ; use caller-saved rcx
	and rax, 0xF               ; rcx = rsp % 16
	sub rsp, rax               ; align stack by 16
	push rax                   ; save aligning, now after call stack will be aligned by 16

	call print_double

	pop rax                    ; restore aligning
	add rsp, rax               ; restore stack

	; prepare for printf

	movsd xmm0, [rel first]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]
	add   rsp, 8
	mov rax, rsp               ; use caller-saved rcx
	and rax, 0xF               ; rcx = rsp % 16
	sub rsp, rax               ; align stack by 16
	push rax                   ; save aligning, now after call stack will be aligned by 16

	call print_double

	pop rax                    ; restore aligning
	add rsp, rax               ; restore stack

	; prepare for printf

	movsd xmm0, [rel second]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]
	add   rsp, 8
	mov rax, rsp               ; use caller-saved rcx
	and rax, 0xF               ; rcx = rsp % 16
	sub rsp, rax               ; align stack by 16
	push rax                   ; save aligning, now after call stack will be aligned by 16

	call print_double

	pop rax                    ; restore aligning
	add rsp, rax               ; restore stack

	; Exit with code 0 (success)
	mov rax, 60                 ; sys_exit
	xor rdi, rdi                ; code 0
	syscall

LINEAR:
	push rbp
	mov  rbp, rsp
	sub  rsp, 16
		; get params from stack====================================
	movsd xmm0, [rsp + 32]        ; pop 1 param
	movsd [rbp - 8 * 1], xmm0

	movsd xmm0, [rsp + 40]        ; pop 2 param
	movsd [rbp - 8 * 2], xmm0

	; params loaded============================================
	
; IF
	; condition
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp - 8 * 2]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	comisd xmm0, xmm1
	jne .endif0
	; if body:
	
; IF
	; condition
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp - 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	comisd xmm0, xmm1
	jne .endif1
	; if body:
; prepare return value
	mov  rax,  __float64__(5.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	leave                      ; restore rbp and rsp
	ret

	.endif1:
; prepare return value
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	leave                      ; restore rbp and rsp
	ret

	.endif0:
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp - 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp - 8 * 2]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	divsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to first
	add   rsp, 8
	movsd  [rel first], xmm0
; prepare return value
	mov  rax,  __float64__(1.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	leave                      ; restore rbp and rsp
	ret

	; Exit with code 0 (success)
	mov rax, 60                 ; sys_exit
	xor rdi, rdi                ; code 0
	syscall

SQSOLVER:
	push rbp
	mov  rbp, rsp
	sub  rsp, 32
		; get params from stack====================================
	movsd xmm0, [rsp + 48]        ; pop 1 param
	movsd [rbp - 8 * 1], xmm0

	movsd xmm0, [rsp + 56]        ; pop 2 param
	movsd [rbp - 8 * 2], xmm0

	movsd xmm0, [rsp + 64]        ; pop 3 param
	movsd [rbp - 8 * 3], xmm0

	; params loaded============================================
	
; IF
	; condition
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp - 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	comisd xmm0, xmm1
	jne .endif4
	; if body:
; prepare return value
	; push call params
	movsd xmm0, [rbp - 8 * 2]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp - 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	call LINEAR
	add rsp, 16                ; clear stack
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; CALL END

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	leave                      ; restore rbp and rsp
	ret

	.endif4:
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp - 8 * 2]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp - 8 * 2]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp - 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp - 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	mov  rax,  __float64__(4.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to D
	add   rsp, 8
	movsd  [rbp - 8 * 3], xmm0
	
; IF
	; condition
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp - 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	comisd xmm0, xmm1
	jne .endif6
	; if body:
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp - 8 * 2]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	mov  rax,  __float64__(2.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	divsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to first
	add   rsp, 8
	movsd  [rel first], xmm0
; prepare return value
	mov  rax,  __float64__(1.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	leave                      ; restore rbp and rsp
	ret

	.endif6:
	
; IF
	; condition
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp - 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	comisd xmm0, xmm1
	jbe .endif8
	; if body:
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp - 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(1.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	mov  rax,  __float64__(2.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	divsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	sqrtsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to D
	add   rsp, 8
	movsd  [rbp - 8 * 3], xmm0
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp - 8 * 2]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp - 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	addsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(2.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp - 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	divsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to first
	add   rsp, 8
	movsd  [rel first], xmm0
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp - 8 * 2]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp - 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(2.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp - 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	divsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to second
	add   rsp, 8
	movsd  [rel second], xmm0
; prepare return value
	mov  rax,  __float64__(2.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	leave                      ; restore rbp and rsp
	ret

	.endif8:
; prepare return value
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	leave                      ; restore rbp and rsp
	ret


section .data


	; global variables

	aCoeff dq 0
	bCoeff dq 0
	cCoeff dq 0
	first dq 0
	second dq 0
	numofanswers dq 0

	; data for libs

	double_format db "%lf", 0
	double_var    dq 0.0
section .note.GNU-stack noalloc noexec nowrite progbits
