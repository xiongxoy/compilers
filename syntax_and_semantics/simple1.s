	.file	"simple1.c"
	.section	.rodata
.LC0:
	.string	"%d"
.LC1:
	.string	"%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
	movl	$10, 24(%esp)
	movl	$20, 28(%esp)
	movl	$4, 24(%esp)
	movl	$5, 28(%esp)
	movl	$0, 24(%esp)
	jmp	.L2
.L3:
	movl	$1, 24(%esp)
	movl	$6, 24(%esp)
	movl	24(%esp), %eax
	addl	$1, %eax
	movl	%eax, 24(%esp)
.L2:
	movl	24(%esp), %eax
	cmpl	$4, %eax
	jle	.L3
	leal	24(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$.LC0, (%esp)
	call	scanf
	movl	24(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$.LC1, (%esp)
	call	printf
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
