section .text

__start:

;CALL Save current Base pointer (rbp)
	push rbp

	;push call params
	mov  rax,  __?float64?__(4)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	sub  rbp,    0             ; allocate stack frame for function variables
	call feed
	pop  rbp                   ; restore base pointer
	; CALL END

	call printf

	ret

feed:

; IF
	; condition
	mov  rax,  __?float64?__(0)
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
	mov  rax,  __?float64?__(1)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	ret

	.endif0:

; IF
	; condition
	mov  rax,  __?float64?__(1)
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
	jne .endif2:
	; if body:
; prepare return value
	mov  rax,  __?float64?__(1)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	ret

	.endif2:
; prepare return value
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:

;CALL Save current Base pointer (rbp)
	push rbp

	;push call params
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	mov  rax,  __?float64?__(1)
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

	sub  rbp,    1             ; allocate stack frame for function variables
	call feed
	pop  rbp                   ; restore base pointer
	; CALL END

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	ret

section .data

	feed dq 0
