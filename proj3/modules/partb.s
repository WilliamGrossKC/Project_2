	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0_m2p0_a2p0_f2p0_d2p0"
	.file	"partb.c"
                                        # Start of file scope inline assembly
	.globl	__trampoline
	.p2align	2
	.type	__trampoline,@function
__trampoline:
	.cfi_startproc
	jr	t6
.L__trampoline_end1:
	.size	__trampoline, .L__trampoline_end1-__trampoline
	.cfi_endproc

                                        # End of file scope inline assembly
	.globl	doit                            # -- Begin function doit
	.p2align	2
	.type	doit,@function
doit:                                   # @doit
	.cfi_startproc
# %bb.0:                                # %entry
	#APP
	mv	a5, zero
	add	a5, a5, t6
	lui	t2, 6
	addiw	t2, t2, 616
	add	a5, a5, t2
	j	6
	addi	s1, s1, 1592
	addi	s1, s1, -90
	j	6
	addi	s1, s1, -456

	#NO_APP
	mv	a0, zero
	mv	x26, ra
	and	x26, x26, x30
	or	x26, x26, x28
	jr	x26
.Lfunc_end0:
	.size	doit, .Lfunc_end0-doit
	.cfi_endproc
                                        # -- End function
	.ident	"FreeBSD clang version 13.0.0 (git@github.com:llvm/llvm-project.git llvmorg-13.0.0-0-gd7b669b3a303)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
