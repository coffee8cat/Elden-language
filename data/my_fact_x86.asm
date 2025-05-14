section .text

__start:
	;calculating expression for assignment
	mov  rax,  __float64__(4)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	pop  xmm2                  ; assignment to Maliketh
	movsd  [rbp + 1], xmm2
	;calculating expression for assignment

;CALL Save current Base pointer (rbp)
	push rbp

	;push call params
	movsd xmm0, [rel Michella]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	sub  rbp,    2             ; allocate stack frame for function variables
	call FACTORIAL
	pop  rbp                   ; restore base pointer
	; CALL END

	pop  xmm2                   ; assignment to Maliketh
	movsd  [rbp + 1], xmm2
	movsd xmm0, [rel Maliketh]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	call printf

	ret

FACTORIAL:

; IF
	; condition
	mov  rax,  __float64__(1)
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
	jne .endif0:
	; if body:
; prepare return value
	mov  rax,  __float64__(1)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	ret

	.endif0:
	;calculating expression for assignment
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	mov  rax,  __float64__(1)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	pop  xmm2                   ; assignment to Maliketh
	movsd  [rbp + 1], xmm2
	;calculating expression for assignment
	; prepare result of left subtree in stack:

;CALL Save current Base pointer (rbp)
	push rbp

	;push call params
	movsd xmm0, [rbp + 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	sub  rbp,    2             ; allocate stack frame for function variables
	call FACTORIAL
	pop  rbp                   ; restore base pointer
	; CALL END

	; prepare result of right subtree in stack:
	movsd xmm0, [rbp + 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	pop  xmm2                   ; assignment to Maliketh
	movsd  [rbp + 1], xmm2
; prepare return value
	movsd xmm0, [rbp + 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	ret

