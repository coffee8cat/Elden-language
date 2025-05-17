section .text

global main
extern elem_in
extern elem_out
extern scanf
main:
	; prepare for printf

	
;before CALL shift base and stack pointers (rsp, rbp)
	sub rbp, 8 * 0
	mov rsp, rbp
	; push call params
	mov  rax,  __float64__(4.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; get params from stack====================================
	movsd xmm0, [rsp]          ; pop 1 param
	add   rsp, 8
	movsd [rbp + 8 * 0], xmm0

	; params loaded============================================
	call feed
	add rbp, 8 * 0
	mov rsp, rbp
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; CALL END

	movsd xmm0, [rsp]
	add   rsp, 8
	call elem_out

	; Exit with code 0 (success)
	mov rax, 60                 ; sys_exit
	xor rdi, rdi                ; code 0
	syscall

feed:
	
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
	jne .endif0
	; if body:
; prepare return value
	mov  rax,  __float64__(1.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	ret

	.endif0:
	
; IF
	; condition
	mov  rax,  __float64__(1.000000)
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
	jne .endif2
	; if body:
; prepare return value
	mov  rax,  __float64__(1.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	ret

	.endif2:
; prepare return value
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 8 * 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	
;before CALL shift base and stack pointers (rsp, rbp)
	sub rbp, 8 * 1
	mov rsp, rbp
	; push call params
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp + 8 * 0]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	mov  rax,  __float64__(1.000000)
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

	; get params from stack====================================
	movsd xmm0, [rsp]          ; pop 1 param
	add   rsp, 8
	movsd [rbp + 8 * 0], xmm0

	; params loaded============================================
	call feed
	add rbp, 8 * 1
	mov rsp, rbp
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; CALL END

	movsd xmm0, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm1, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	ret


section .data


	; global variables


	; data for libs

	double_format db "%lf", 0
	double_var    dq 0.0
section .note.GNU-stack noalloc noexec nowrite progbits
