

section .data
	double_var    dq 0.0

section .rodata
	double_format db "%lf", 0

section .text

global main
extern printf
extern scanf
main:
	; prepare for scanf
	lea rdi, [rel double_format]  ; db "%lf", 0
	lea rsi, [rel double_var]     ; dq 0.0
	xor eax, eax
	call scanf
	; store result in stack
	movsd xmm0, [double_var]
	sub rsp, 8
	movsd [rsp], xmm0

	movsd xmm0, [rsp]
	add   rsp, 8
	movsd [rel aCoeff], xmm0
	; scanf end

	; prepare for scanf
	mov rdi, double_format  ; db "%lf", 0
	mov rsi, double_var     ; dq 0.0
	xor eax, eax
	call scanf
	; store result in stack
	movsd xmm0, [double_var]
	sub rsp, 8
	movsd [rsp], xmm0

	movsd xmm0, [rsp]
	add   rsp, 8
	movsd [rel bCoeff], xmm0
	; scanf end

	; prepare for scanf
	mov rdi, double_format  ; db "%lf", 0
	mov rsi, double_var     ; dq 0.0
	xor eax, eax
	call scanf
	; store result in stack
	movsd xmm0, [double_var]
	sub rsp, 8
	movsd [rsp], xmm0

	movsd xmm0, [rsp]
	add   rsp, 8
	movsd [rel cCoeff], xmm0
	; scanf end

	;calculating expression for assignment

;before CALL shift base and stack pointers (rsp, rbp)
	sub rbp, 8 * 6
	mov rsp, rbp
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
	add rbp, 8 * 6
	mov rsp, rbp
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; CALL END

	movsd xmm0, [rsp]          ; assignment to bCoeff
	add   rsp, 8
	movsd  [rbp + 8 * 1], xmm0
	; prepare for printf

	movsd xmm0, [rel numofanswers]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]
	add   rsp, 8
	mov rdi, double_format  ; pointer to format string
	mov eax, 1              ; number of arguments in xmm
	call printf

	; prepare for printf

	movsd xmm0, [rel first]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]
	add   rsp, 8
	mov rdi, double_format  ; pointer to format string
	mov eax, 1              ; number of arguments in xmm
	call printf

	; prepare for printf

	movsd xmm0, [rel second]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]
	add   rsp, 8
	mov rdi, double_format  ; pointer to format string
	mov eax, 1              ; number of arguments in xmm
	call printf

	; Exit with code 0 (success)
	mov rax, 60                 ; sys_exit
	xor rdi, rdi                ; code 0
	syscall

LINEAR:

; IF
	; condition
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp + 8 * 1]
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

	movsd xmm0, [rbp + 8 * 0]
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
	ret

	.endif1:
; prepare return value
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
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
	movsd xmm0, [rbp + 8 * 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	divsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to bCoeff
	add   rsp, 8
	movsd  [rbp + 8 * 1], xmm0
; prepare return value
	mov  rax,  __float64__(1.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	ret

	; Exit with code 0 (success)
	mov rax, 60                 ; sys_exit
	xor rdi, rdi                ; code 0
	syscall

SQSOLVER:

; IF
	; condition
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp + 8 * 2]
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

;before CALL shift base and stack pointers (rsp, rbp)
	sub rbp, 8 * 4
	mov rsp, rbp
	; push call params
	movsd xmm0, [rbp + 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp + 8 * 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	call LINEAR
	add rbp, 8 * 4
	mov rsp, rbp
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; CALL END

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	ret

	.endif4:
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 8 * 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 8 * 2]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	mov  rax,  __float64__(4.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to bCoeff
	add   rsp, 8
	movsd  [rbp + 8 * 1], xmm0

; IF
	; condition
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp + 8 * 3]
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
	movsd xmm0, [rbp + 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	mov  rax,  __float64__(2.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	divsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to bCoeff
	add   rsp, 8
	movsd  [rbp + 8 * 1], xmm0
; prepare return value
	mov  rax,  __float64__(1.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	ret

	.endif6:

; IF
	; condition
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp + 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	comisd xmm0, xmm1
	ja .endif8
	; if body:
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 8 * 3]
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

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	divsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	sqrtsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to bCoeff
	add   rsp, 8
	movsd  [rbp + 8 * 1], xmm0
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
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
	movsd xmm0, [rbp + 8 * 2]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	divsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to bCoeff
	add   rsp, 8
	movsd  [rbp + 8 * 1], xmm0
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(0.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
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
	movsd xmm0, [rbp + 8 * 2]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	divsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to bCoeff
	add   rsp, 8
	movsd  [rbp + 8 * 1], xmm0
; prepare return value
	mov  rax,  __float64__(2.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	ret

	.endif8:
; prepare return value
	movsd xmm0, [rbp + 8 * 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
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


section .note.GNU-stack noalloc noexec nowrite progbits
