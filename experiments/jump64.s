	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14
	.globl	_m68k_op_call_emu_proc  ## -- Begin function m68k_op_call_emu_proc
	.p2align	4, 0x90
_m68k_op_call_emu_proc:                 ## @m68k_op_call_emu_proc
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	_sBaseAddr@GOTPCREL(%rip), %rax
	movl	_m68ki_cpu+36(%rip), %ecx
	movl	%ecx, -4(%rbp)
	movl	_m68ki_cpu+40(%rip), %ecx
	movl	%ecx, -8(%rbp)
	movq	(%rax), %rdx
	movl	-4(%rbp), %ecx
	movl	%ecx, %esi
	addq	%rsi, %rdx
	movq	%rdx, -16(%rbp)
	movq	-16(%rbp), %rdx
	movq	%rdx, -24(%rbp)
	movq	-16(%rbp), %rdx
	movl	(%rdx), %ecx
	movl	%ecx, -4(%rbp)
	movl	-4(%rbp), %ecx
	movl	%ecx, %edx
	movq	%rdx, -24(%rbp)
	movq	-24(%rbp), %rdx
	movl	-8(%rbp), %edi
	movq	(%rax), %rsi
	callq	*%rdx
	movl	%eax, _m68ki_cpu+4(%rip)
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_m68ki_cpu              ## @m68ki_cpu
.zerofill __DATA,__common,_m68ki_cpu,72,2
	.comm	_sBaseAddr,8,3          ## @sBaseAddr

.subsections_via_symbols
