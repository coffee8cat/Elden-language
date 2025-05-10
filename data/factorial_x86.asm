
;CALL Save current Base pointer (rbp)
	push rbp

;push call params
	mov  rcx, 4
	push rcx

; BX_shift
	sub rbp, 0

call feed
	push rax
	; CALL END

call printf
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


; test condition
	jne .endif0:

; RETURN
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


; test condition
	jne .endif2:

; RETURN
	mov  rcx, 1
	push rcx
	ret


.endif2:


; RETURN
	mov  rcx, [bp + 0]
	push rcx

;CALL Save current Base pointer (rbp)
	push rbp


;push call params
	mov  rcx, [bp + 0]
	push rcx
	mov  rcx, 1
	push rcx
	pop rax
	pop rdx

SUB
	push rax


; BX_shift
	sub rbp, 1

call feed
	push rax
	; CALL END

	pop rax
	pop rdx

MUL
	push rax

	ret

