	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.align	4, 0x90
_main:
Leh_func_begin1:
	pushq	%rbp
Ltmp0:
	movq	%rsp, %rbp
Ltmp1:
	subq	$80, %rsp
Ltmp2:
	movl	_C.0.1898(%rip), %eax
	movl	%eax, -32(%rbp)
	movl	_C.0.1898+4(%rip), %eax
	movl	%eax, -28(%rbp)
	movl	_C.0.1898+8(%rip), %eax
	movl	%eax, -24(%rbp)
	movl	_C.0.1898+12(%rip), %eax
	movl	%eax, -20(%rbp)
	movl	_C.0.1898+16(%rip), %eax
	movl	%eax, -16(%rbp)
	movl	_C.0.1898+20(%rip), %eax
	movl	%eax, -12(%rbp)
	movl	_C.1.1899(%rip), %eax
	movl	%eax, -56(%rbp)
	movl	_C.1.1899+4(%rip), %eax
	movl	%eax, -52(%rbp)
	movl	_C.1.1899+8(%rip), %eax
	movl	%eax, -48(%rbp)
	movl	_C.1.1899+12(%rip), %eax
	movl	%eax, -44(%rbp)
	movl	_C.1.1899+16(%rip), %eax
	movl	%eax, -40(%rbp)
	movl	_C.1.1899+20(%rip), %eax
	movl	%eax, -36(%rbp)
	xorb	%al, %al
	leaq	L_.str(%rip), %rcx
	movq	%rcx, %rdi
	callq	_printf
	leaq	-60(%rbp), %rcx
	xorb	%dl, %dl
	leaq	L_.str1(%rip), %rsi
	movq	%rsi, %rdi
	movq	%rcx, %rsi
	movb	%dl, %al
	callq	_scanf
	movabsq	$0, %rax
	cvtsi2ssq	%rax, %xmm0
	movss	%xmm0, -68(%rbp)
	movl	$0, -72(%rbp)
	movl	$0, -76(%rbp)
	jmp	LBB1_2
LBB1_1:
	movl	-76(%rbp), %eax
	movslq	%eax, %rax
	movl	-32(%rbp,%rax,4), %eax
	movl	-76(%rbp), %ecx
	movslq	%ecx, %rcx
	movl	-56(%rbp,%rcx,4), %ecx
	imull	%ecx, %eax
	cvtsi2ss	%eax, %xmm0
	movss	-68(%rbp), %xmm1
	addss	%xmm1, %xmm0
	movss	%xmm0, -68(%rbp)
	movl	-76(%rbp), %eax
	movslq	%eax, %rax
	movl	-56(%rbp,%rax,4), %eax
	movl	-72(%rbp), %ecx
	addl	%ecx, %eax
	movl	%eax, -72(%rbp)
	movl	-76(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -76(%rbp)
LBB1_2:
	movl	-76(%rbp), %eax
	cmpl	$5, %eax
	jle	LBB1_1
	movl	-72(%rbp), %eax
	cvtsi2ss	%eax, %xmm0
	movss	-68(%rbp), %xmm1
	divss	%xmm0, %xmm1
	movss	%xmm1, -64(%rbp)
	movss	-64(%rbp), %xmm0
	movabsq	$60, %rax
	cvtsi2ssq	%rax, %xmm1
	ucomiss	%xmm1, %xmm0
	jb	LBB1_5
	movss	-64(%rbp), %xmm0
	movabsq	$60, %rax
	cvtsi2ssq	%rax, %xmm1
	subss	%xmm1, %xmm0
	movss	%xmm0, -64(%rbp)
	movss	-64(%rbp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	movl	-60(%rbp), %eax
	movb	$1, %cl
	leaq	L_.str2(%rip), %rdx
	movq	%rdx, %rdi
	movl	%eax, %esi
	movb	%cl, %al
	callq	_printf
	jmp	LBB1_6
LBB1_5:
	movss	-64(%rbp), %xmm0
	movabsq	$60, %rax
	cvtsi2ssq	%rax, %xmm1
	subss	%xmm0, %xmm1
	movss	%xmm1, -64(%rbp)
	movss	-64(%rbp), %xmm0
	cvtss2sd	%xmm0, %xmm0
	movl	-60(%rbp), %eax
	movb	$1, %cl
	leaq	L_.str3(%rip), %rdx
	movq	%rdx, %rdi
	movl	%eax, %esi
	movb	%cl, %al
	callq	_printf
LBB1_6:
	movl	$0, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	addq	$80, %rsp
	popq	%rbp
	ret
Leh_func_end1:

	.section	__TEXT,__const
	.align	4
_C.0.1898:
	.long	76
	.long	82
	.long	90
	.long	86
	.long	79
	.long	62

	.align	4
_C.1.1899:
	.long	2
	.long	2
	.long	1
	.long	2
	.long	2
	.long	3

	.section	__TEXT,__cstring,cstring_literals
	.align	3
L_.str:
	.asciz	 "please input your student number:"

L_.str1:
	.asciz	 "%d"

	.align	3
L_.str2:
	.asciz	 "the score of student number %d is %f higher than 60.\n"

	.align	3
L_.str3:
	.asciz	 "the score of student number %d is %f lower than 60.\n"

	.section	__TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame0:
Lsection_eh_frame:
Leh_frame_common:
Lset0 = Leh_frame_common_end-Leh_frame_common_begin
	.long	Lset0
Leh_frame_common_begin:
	.long	0
	.byte	1
	.asciz	 "zR"
	.byte	1
	.byte	120
	.byte	16
	.byte	1
	.byte	16
	.byte	12
	.byte	7
	.byte	8
	.byte	144
	.byte	1
	.align	3
Leh_frame_common_end:
	.globl	_main.eh
_main.eh:
Lset1 = Leh_frame_end1-Leh_frame_begin1
	.long	Lset1
Leh_frame_begin1:
Lset2 = Leh_frame_begin1-Leh_frame_common
	.long	Lset2
Ltmp3:
	.quad	Leh_func_begin1-Ltmp3
Lset3 = Leh_func_end1-Leh_func_begin1
	.quad	Lset3
	.byte	0
	.byte	4
Lset4 = Ltmp0-Leh_func_begin1
	.long	Lset4
	.byte	14
	.byte	16
	.byte	134
	.byte	2
	.byte	4
Lset5 = Ltmp1-Ltmp0
	.long	Lset5
	.byte	13
	.byte	6
	.align	3
Leh_frame_end1:


.subsections_via_symbols
