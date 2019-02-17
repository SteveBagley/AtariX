	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 14
	.globl	_m68k_op_call_emu_proc  ## -- Begin function m68k_op_call_emu_proc
	.p2align	4, 0x90
_m68k_op_call_emu_proc:                 ## @m68k_op_call_emu_proc
	.cfi_startproc
## %bb.0:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset %ebp, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register %ebp
	pushl	%esi
	subl	$36, %esp
	.cfi_offset %esi, -12
	calll	L0$pb
L0$pb:
	popl	%eax
	movl	L_sBaseAddr$non_lazy_ptr-L0$pb(%eax), %ecx
	movl	(_m68ki_cpu-L0$pb)+36(%eax), %edx
	movl	%edx, -8(%ebp)
	movl	(_m68ki_cpu-L0$pb)+40(%eax), %edx
	movl	%edx, -12(%ebp)
	movl	(%ecx), %edx
	addl	-8(%ebp), %edx
	movl	%edx, -16(%ebp)
	movl	-16(%ebp), %edx
	movl	%edx, -20(%ebp)
	movl	-16(%ebp), %edx
	movl	(%edx), %edx
	movl	%edx, -8(%ebp)
	movl	-8(%ebp), %edx
	movl	%edx, -20(%ebp)
	movl	-20(%ebp), %edx
	movl	-12(%ebp), %esi
	movl	(%ecx), %ecx
	movl	%esi, (%esp)
	movl	%ecx, 4(%esp)
	movl	%eax, -24(%ebp)         ## 4-byte Spill
	calll	*%edx
	movl	-24(%ebp), %ecx         ## 4-byte Reload
	movl	%eax, (_m68ki_cpu-L0$pb)+4(%ecx)
	addl	$36, %esp
	popl	%esi
	popl	%ebp
	retl
	.cfi_endproc
                                        ## -- End function
	.globl	_m68ki_cpu              ## @m68ki_cpu
.zerofill __DATA,__common,_m68ki_cpu,72,2
	.comm	_sBaseAddr,4,2          ## @sBaseAddr

	.section	__IMPORT,__pointers,non_lazy_symbol_pointers
L_sBaseAddr$non_lazy_ptr:
	.indirect_symbol	_sBaseAddr
	.long	0

.subsections_via_symbols
