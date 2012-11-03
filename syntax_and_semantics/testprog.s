	.file	"testprog.c"
	.globl	s
	.section	.rodata
.LC0:
	.string	"hard string\n"
	.data
	.align 4
	.type	s, @object
	.size	s, 4
s:
	.long	.LC0
	.text
	.globl	add
	.type	add, @function
add:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	movl	12(%ebp), %eax
	movl	8(%ebp), %edx
	addl	%edx, %eax
	popl	%ebp
	.cfi_def_cfa 4, 4
	.cfi_restore 5
	ret
	.cfi_endproc
.LFE0:
	.size	add, .-add
	.section	.rodata
	.align 4
.LC1:
	.string	"please input your student number:"
.LC2:
	.string	"%d"
	.align 4
.LC5:
	.string	"the score of student number %d is %f higher than 60.\n"
	.align 4
.LC6:
	.string	"the score of student number %d is %f lower than 60.\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$112, %esp
	movl	$76, 44(%esp)
	movl	$82, 48(%esp)
	movl	$90, 52(%esp)
	movl	$86, 56(%esp)
	movl	$79, 60(%esp)
	movl	$62, 64(%esp)
	movl	$2, 68(%esp)
	movl	$2, 72(%esp)
	movl	$1, 76(%esp)
	movl	$2, 80(%esp)
	movl	$2, 84(%esp)
	movl	$3, 88(%esp)
	movl	$.LC1, %eax
	movl	%eax, (%esp)
	call	printf
	movl	$.LC2, %eax
	leal	92(%esp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	__isoc99_scanf
	movl	$0x00000000, %eax
	movl	%eax, 96(%esp)
	movl	$0, 100(%esp)
	movl	$0, 104(%esp)
	jmp	.L3
.L4:
	movl	104(%esp), %eax
	movl	44(%esp,%eax,4), %edx
	movl	104(%esp), %eax
	movl	68(%esp,%eax,4), %eax
	imull	%edx, %eax
	movl	%eax, 28(%esp)
	fildl	28(%esp)
	flds	96(%esp)
	faddp	%st, %st(1)
	fstps	96(%esp)
	movl	104(%esp), %eax
	movl	68(%esp,%eax,4), %eax
	addl	%eax, 100(%esp)
	addl	$1, 104(%esp)
.L3:
	cmpl	$5, 104(%esp)
	jle	.L4
	fildl	100(%esp)
	flds	96(%esp)
	fdivp	%st, %st(1)
	fstps	108(%esp)
	flds	108(%esp)
	flds	.LC4
	fxch	%st(1)
	fucomip	%st(1), %st
	fstp	%st(0)
	setae	%al
	testb	%al, %al
	je	.L5
	flds	108(%esp)
	flds	.LC4
	fsubrp	%st, %st(1)
	fstps	108(%esp)
	flds	108(%esp)
	movl	92(%esp), %edx
	movl	$.LC5, %eax
	fstpl	8(%esp)
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	jmp	.L6
.L5:
	flds	.LC4
	fsubs	108(%esp)
	fstps	108(%esp)
	flds	108(%esp)
	movl	92(%esp), %edx
	movl	$.LC6, %eax
	fstpl	8(%esp)
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
.L6:
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.section	.rodata
	.align 4
.LC4:
	.long	1114636288
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
