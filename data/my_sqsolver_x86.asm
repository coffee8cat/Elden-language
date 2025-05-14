section .text

global main
extern printf

main:

	call scanf
	pop rcx
	movsd [rel aCoeff], xmm0

	call scanf
	pop rcx
	movsd [rel bCoeff], xmm0

	call scanf
	pop rcx
	movsd [rel cCoeff], xmm0
	;calculating expression for assignment
	
;CALL Save current Base pointer (rbp)
	push rbp

	;push call params
	movsd xmm0, [rel aCoeff]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rel bCoeff]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rel cCoeff]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	sub  rbp,    6             ; allocate stack frame for function variables
	call SQSOLVER
	pop  rbp                   ; restore base pointer
	; CALL END

	movsd xmm0, [rsp]          ; assignment to bCoeff
	add   rsp, 8
	movsd  [rbp + 1], xmm0
	movsd xmm0, [rel numofanswers]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	call printf

	movsd xmm0, [rel first]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	call printf

	movsd xmm0, [rel second]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	call printf

	ret

LINEAR:
	
; IF
	; condition
	mov  rax,  __float64__(0)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp + 1]
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
	mov  rax,  __float64__(0)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp + 0]
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
	mov  rax,  __float64__(5)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	ret

	.endif1:
; prepare return value
	mov  rax,  __float64__(0)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	ret

	.endif0:
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(0)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 1]
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
	movsd  [rbp + 1], xmm0
; prepare return value
	mov  rax,  __float64__(1)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	ret

	ret

SQSOLVER:
	
; IF
	; condition
	mov  rax,  __float64__(0)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp + 2]
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
	
;CALL Save current Base pointer (rbp)
	push rbp

	;push call params
	movsd xmm0, [rbp + 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp + 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	sub  rbp,    4             ; allocate stack frame for function variables
	call LINEAR
	pop  rbp                   ; restore base pointer
	; CALL END

	ret

	.endif4:
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 1]
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
	movsd xmm0, [rbp + 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 2]
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
	mov  rax,  __float64__(4)
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
	movsd  [rbp + 1], xmm0
	
; IF
	; condition
	mov  rax,  __float64__(0)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp + 3]
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
	mov  rax,  __float64__(0)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	mov  rax,  __float64__(2)
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
	movsd  [rbp + 1], xmm0
; prepare return value
	mov  rax,  __float64__(1)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	ret

	.endif6:
	
; IF
	; condition
	mov  rax,  __float64__(0)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rbp + 3]
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
	movsd xmm0, [rbp + 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(1)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	mov  rax,  __float64__(2)
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
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; assignment to bCoeff
	add   rsp, 8
	movsd  [rbp + 1], xmm0
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(0)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 3]
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
	mov  rax,  __float64__(2)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 2]
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
	movsd  [rbp + 1], xmm0
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	mov  rax,  __float64__(0)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; prepare result of left subtree in stack:
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 3]
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
	mov  rax,  __float64__(2)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 2]
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
	movsd  [rbp + 1], xmm0
; prepare return value
	mov  rax,  __float64__(2)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	ret

	.endif8:
; prepare return value
	movsd xmm0, [rbp + 3]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	ret

section .note.GNU-stack noalloc noexec nowrite progbits
