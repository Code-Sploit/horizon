global _start

section .text

_start:
	mov rax, 60
	mov rdi, 0
	syscall

section .data
	data: db "0", 1
	_data: equ $ - data
	datat: db "hi", 2
	_datat: equ $ - datat

