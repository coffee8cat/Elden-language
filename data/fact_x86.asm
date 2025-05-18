section .text

global main
extern print_double
extern elem_in
extern elem_out
extern scanf
main:
	; prepare for printf

	; push call params
	mov  rax,  __float64__(5.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	call feed
	add rsp, 8                ; clear stack
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; CALL END

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

feed:
	push rbp
	mov  rbp, rsp
	sub  rsp, 16
		; get params from stack====================================
	movsd xmm0, [rsp + 32]        ; pop 1 param
	movsd [rbp - 8 * 1], xmm0

	; params loaded============================================

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
	jne .endif0
	; if body:
; prepare return value
	mov  rax,  __float64__(1.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	leave                      ; restore rbp and rsp
	ret

	.endif0:

; IF
	; condition
	mov  rax,  __float64__(1.000000)
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
	jne .endif2
	; if body:
; prepare return value
	mov  rax,  __float64__(1.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	leave                      ; restore rbp and rsp
	ret

	.endif2:
; prepare return value
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp - 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	; push call params
	; prepare result of left subtree in stack:
	movsd xmm0, [rbp - 8 * 1]
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; prepare result of right subtree in stack:
	mov  rax,  __float64__(1.000000)
	movq xmm0, rax
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	subsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	call feed
	add rsp, 8                ; clear stack
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	; CALL END

	movsd xmm1, [rsp]          ; get result of right subtree from stack
	add   rsp, 8
	movsd xmm0, [rsp]          ; get result of left  subtree from stack
	add   rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0          ; save result in stack

	movsd xmm0, [rsp]          ; get function result before ret
	add   rsp, 8
	leave                      ; restore rbp and rsp
	ret


section .data


	; global variables


	; data for libs

	double_format db "%lf", 0
	double_var    dq 0.0
section .note.GNU-stack noalloc noexec nowrite progbits
