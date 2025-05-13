
Start:
;CALL Save current Base pointer (rbp)
	push rbp

	;push call params
	mov  rcx, 4
	push rcx
	sub  rbp,    0             ; allocate stack frame for function variables
	call feed
	pop  rbp                   ; restore base pointer
	push rax                   ; save return value in stack
	; CALL END

	call printf

	ret

feed:

; IF
	; condition
	mov  rcx, 0
	push rcx
	mov  rcx, [bp + 0]
	push rcx

	pop  rcx
	pop  rdx
	test rcx, rdx

	jne .endif0:
	; if body:
; prepare return value
	mov  rcx, 1
	push rcx
	ret

	.endif0:

; IF
	; condition
	mov  rcx, 1
	push rcx
	mov  rcx, [bp + 0]
	push rcx

	pop  rcx
	pop  rdx
	test rcx, rdx

	jne .endif2:
	; if body:
; prepare return value
	mov  rcx, 1
	push rcx
	ret

	.endif2:
; prepare return value
	; prepare result of left subtree in stack:
	mov  rcx, [bp + 0]
	push rcx
	; prepare result of right subtree in stack:

;CALL Save current Base pointer (rbp)
	push rbp

	;push call params
	; prepare result of left subtree in stack:
	mov  rcx, [bp + 0]
	push rcx
	; prepare result of right subtree in stack:
	mov  rcx, 1
	push rcx
	pop  rax                   ; get result of right subtree from stack
	pop  rdx                   ; get result of left  subtree from stack
	sub  rax, rdx
	push rax                   ; save result in stack

	sub  rbp,    1             ; allocate stack frame for function variables
	call feed
	pop  rbx                   ; restore base pointer
	push rax                   ; save return value in stack
	; CALL END

	pop  rax                   ; get result of right subtree from stack
	pop  rdx                   ; get result of left  subtree from stack
	mul  rax, rdx
	push rax                   ; save result in stack

	ret

