	.file	"array_ref.c"
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
	subl	$16, %esp
	movl	$1, -12(%ebp)
	movl	$2, -8(%ebp)
	movl	$0, -4(%ebp)
	movl	$3, -8(%ebp)
	movl	$0, -4(%ebp)
	jmp	.L2
.L3:
	movl	-4(%ebp), %eax
	movl	$999, -12(%ebp,%eax,4)
	addl	$1, -4(%ebp)
.L2:
	cmpl	$1, -4(%ebp)
	jle	.L3
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
