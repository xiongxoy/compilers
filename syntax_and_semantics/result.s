	.file	"simple1.c"
	.text
	.section	.rodata
	.align 4
.LC0:
	.string "please input your student number:"
	.align 4
.LC1:
	.string "%d"
	.align 4
.LC2:
	.string "mul is %d\n"
	.align 4
.LC3:
	.string "socre[i] is %d\n"
	.align 4
.LC4:
	.string "credit[i] is %d\n"
	.align 4
.LC5:
	.string "sum is %d\n"
	.align 4
.LC6:
	.string "temp is %d\n"
	.align 4
.LC7:
	.string "mean is %d\n"
	.align 4
.LC8:
	.string "the score of student number %d is %d higher than 60.\n"
	.align 4
.LC9:
	.string "the score of student number %d is %f lower than 60.\n"
	.global main
	.type main, @function
	main:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl %esp, %ebp
	.cfi_def_cfa_register 5
	subl $156, %esp
	movl $62, -4(%ebp)
	movl $79, -8(%ebp)
	movl $86, -12(%ebp)
	movl $90, -16(%ebp)
	movl $82, -20(%ebp)
	movl $76, -24(%ebp)
	movl $3, -32(%ebp)
	movl $2, -36(%ebp)
	movl $2, -40(%ebp)
	movl $1, -44(%ebp)
	movl $2, -48(%ebp)
	movl $2, -52(%ebp)
	movl $.LC0, %eax
	movl %eax, 0(%esp)
	call	printf
	leal -60(%ebp), %eax
	movl %eax, -80(%ebp)
	movl -80(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC1, %eax
	movl %eax, 0(%esp)
	call	scanf
	movl $0, %eax
	movl %eax, -68(%ebp)
	movl $0, %eax
	movl %eax, -72(%ebp)
	movl -68(%ebp), %eax
	movl %eax, -92(%ebp)
	movl $0, %ebx
	movl -24(%ebp,%ebx,4), %eax
	movl %eax, -84(%ebp)
	movl $0, %ebx
	movl -52(%ebp,%ebx,4), %eax
	imull -84(%ebp), %eax
	movl %eax, -88(%ebp)
	movl -88(%ebp), %eax
	addl -92(%ebp), %eax
	movl %eax, -96(%ebp)
	movl -96(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC2, %eax
	movl %eax, 0(%esp)
	call	printf
	movl $0, %eax
	movl %eax, -76(%ebp)
.L1:
	movl $1, -100(%ebp)
	movl $6, %eax
	cmpl %eax, -76(%ebp)
	jl .L0
	movl $0, -100(%ebp)
.L0:
	cmpl $1, -100(%ebp)
	jne  .L2
	movl -76(%ebp), %ebx
	movl -24(%ebp,%ebx,4), %eax
	movl %eax, 4(%esp)
	movl $.LC3, %eax
	movl %eax, 0(%esp)
	call	printf
	movl -76(%ebp), %ebx
	movl -52(%ebp,%ebx,4), %eax
	movl %eax, 4(%esp)
	movl $.LC4, %eax
	movl %eax, 0(%esp)
	call	printf
	movl -68(%ebp), %eax
	movl %eax, -112(%ebp)
	movl -76(%ebp), %ebx
	movl -24(%ebp,%ebx,4), %eax
	movl %eax, -104(%ebp)
	movl -76(%ebp), %ebx
	movl -52(%ebp,%ebx,4), %eax
	imull -104(%ebp), %eax
	movl %eax, -108(%ebp)
	movl -108(%ebp), %eax
	addl -112(%ebp), %eax
	movl %eax, -116(%ebp)
	movl -116(%ebp), %eax
	movl %eax, -68(%ebp)
	movl -72(%ebp), %eax
	movl %eax, -120(%ebp)
	movl -76(%ebp), %ebx
	movl -52(%ebp,%ebx,4), %eax
	addl -120(%ebp), %eax
	movl %eax, -124(%ebp)
	movl -124(%ebp), %eax
	movl %eax, -72(%ebp)
	movl -68(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC5, %eax
	movl %eax, 0(%esp)
	call	printf
	movl -72(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC6, %eax
	movl %eax, 0(%esp)
	call	printf
	addl $1, -76(%ebp)
	jmp .L1
.L2:
	movl -68(%ebp), %eax
	movl $0, %edx
	movl -72(%ebp), %ecx
	idiv %ecx
	movl %eax, -128(%ebp)
	movl -128(%ebp), %eax
	movl %eax, -64(%ebp)
	movl -64(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC7, %eax
	movl %eax, 0(%esp)
	call	printf
	movl $1, -132(%ebp)
	movl $60, %eax
	cmpl -64(%ebp), %eax
	jl .L3
	movl $0, -132(%ebp)
.L3:
	cmpl $1, -132(%ebp)
	jne  .L4
	movl -64(%ebp), %eax
	subl $60, %eax
	movl %eax, -136(%ebp)
	movl -136(%ebp), %eax
	movl %eax, -64(%ebp)
	movl -64(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC7, %eax
	movl %eax, 0(%esp)
	call	printf
	movl -64(%ebp), %eax
	movl %eax, 8(%esp)
	movl -60(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC8, %eax
	movl %eax, 0(%esp)
	call	printf
	jmp .L5
.L4:
	movl $60, %eax
	subl -64(%ebp), %eax
	movl %eax, -140(%ebp)
	movl -140(%ebp), %eax
	movl %eax, -64(%ebp)
	movl -64(%ebp), %eax
	movl %eax, 8(%esp)
	movl -60(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC9, %eax
	movl %eax, 0(%esp)
	call	printf
.L5:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LBE0:	.size main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
