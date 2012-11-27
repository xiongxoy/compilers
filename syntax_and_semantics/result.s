	.file	"simple1.c"
	.text
	.section	.rodata
	.align 4
.LC0:
	.string "c[1] + d[1] is %d\n"
	.align 4
.LC1:
	.string "a is %d\n"
	.align 4
.LC2:
	.string "%d"
	.align 4
.LC3:
	.string "%d\n"
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
	subl $96, %esp
	movl $10, %eax
	movl %eax, -4(%ebp)
	movl $20, %eax
	movl %eax, -8(%ebp)
	movl $14, -12(%ebp)
	movl $13, -16(%ebp)
	movl $12, -20(%ebp)
	movl $11, -24(%ebp)
	movl $10, -28(%ebp)
	movl $22, -36(%ebp)
	movl $21, -40(%ebp)
	movl $20, -44(%ebp)
	movl $1, %ebx
	movl $1, %eax
	movl %eax, -28(%ebp,%ebx,4)
	movl $2, %ebx
	movl $2, %eax
	movl %eax, -28(%ebp,%ebx,4)
	movl $1, %ebx
	movl $3, %eax
	movl %eax, -52(%ebp,%ebx,4)
	movl $2, %ebx
	movl $4, %eax
	movl %eax, -52(%ebp,%ebx,4)
	movl $1, %ebx
	movl -28(%ebp,%ebx,4), %eax
	movl $2, %ebx
	addl -52(%ebp,%ebx,4), %eax
	movl %eax, -60(%ebp)
	movl -60(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC0, %eax
	movl %eax, 0(%esp)
	call	printf
	movl $0, %eax
	movl %eax, -4(%ebp)
.L1:
	movl $1, -64(%ebp)
	movl $5, %eax
	cmpl %eax, -4(%ebp)
	jl .L0
	movl $0, -64(%ebp)
.L0:
	cmpl $1, -64(%ebp)
	jne  .L2
	movl -4(%ebp), %eax
	movl $2, %ebx
	imull -28(%ebp,%ebx,4), %eax
	movl %eax, -68(%ebp)
	movl -68(%ebp), %eax
	movl %eax, -4(%ebp)
	movl -4(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC1, %eax
	movl %eax, 0(%esp)
	call	printf
	addl $1, -4(%ebp)
	jmp .L1
.L2:
	movl -4(%ebp), %eax
	imull $13, %eax
	movl %eax, -72(%ebp)
	movl -72(%ebp), %eax
	movl %eax, -4(%ebp)
	movl -4(%ebp), %eax
	imull $1, %eax
	movl %eax, -76(%ebp)
	movl -76(%ebp), %eax
	movl %eax, -4(%ebp)
	movl -4(%ebp), %eax
	subl $1, %eax
	movl %eax, -80(%ebp)
	movl -80(%ebp), %eax
	movl %eax, -4(%ebp)
	movl -4(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC1, %eax
	movl %eax, 0(%esp)
	call	printf
	leal -4(%ebp), %eax
	movl %eax, -84(%ebp)
	movl -84(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC2, %eax
	movl %eax, 0(%esp)
	call	scanf
	movl -4(%ebp), %eax
	movl %eax, 4(%esp)
	movl $.LC3, %eax
	movl %eax, 0(%esp)
	call	printf
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LBE0:	.size main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
