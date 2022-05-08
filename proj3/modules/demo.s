	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0_m2p0_a2p0_f2p0_d2p0"
	.file	"demo.c"
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
	.globl	do_add                          # -- Begin function do_add
	.p2align	2
	.type	do_add,@function
do_add:                                 # @do_add
	.cfi_startproc
# %bb.0:                                # %entry
	addw	a0, a1, a0
	mv	x26, ra
	and	x26, x26, x30
	or	x26, x26, x28
	jr	x26
.Lfunc_end0:
	.size	do_add, .Lfunc_end0-do_add
	.cfi_endproc
                                        # -- End function
	.globl	local_state                     # -- Begin function local_state
	.p2align	2
	.type	local_state,@function
local_state:                            # @local_state
	.cfi_startproc
# %bb.0:                                # %entry
	lui	a1, %hi(local_state.counter)
	addi	x27, a1, %lo(local_state.counter)
	and	x27, x27, x30
	or	x27, x27, x29
	lw	a0, 0(x27)
	addi	a2, a0, 1
	addi	x27, a1, %lo(local_state.counter)
	and	x27, x27, x30
	or	x27, x27, x29
	sw	a2, 0(x27)
	mv	x26, ra
	and	x26, x26, x30
	or	x26, x26, x28
	jr	x26
.Lfunc_end1:
	.size	local_state, .Lfunc_end1-local_state
	.cfi_endproc
                                        # -- End function
	.globl	find_in_string                  # -- Begin function find_in_string
	.p2align	2
	.type	find_in_string,@function
find_in_string:                         # @find_in_string
	.cfi_startproc
# %bb.0:                                # %entry
	lui	a2, %hi(searchstr)
	addi	x27, a2, %lo(searchstr)
	and	x27, x27, x30
	or	x27, x27, x29
	lbu	a3, 0(x27)
	addi	a1, zero, -1
	beqz	a3, .LBB2_4
# %bb.1:                                # %for.body.preheader
	addi	a2, a2, %lo(searchstr)
	addi	a4, a2, 1
.LBB2_2:                                # %for.body
                                        # =>This Inner Loop Header: Depth=1
	andi	a3, a3, 255
	beq	a3, a0, .LBB2_5
# %bb.3:                                # %for.inc
                                        #   in Loop: Header=BB2_2 Depth=1
	addi	x27, a4, 0
	and	x27, x27, x30
	or	x27, x27, x29
	lbu	a3, 0(x27)
	addi	a2, a2, 1
	addi	a4, a4, 1
	bnez	a3, .LBB2_2
.LBB2_4:                                # %cleanup
	sext.w	a0, a1
	mv	x26, ra
	and	x26, x26, x30
	or	x26, x26, x28
	jr	x26
.LBB2_5:                                # %if.then
	lui	a0, %hi(searchstr)
	addi	a0, a0, %lo(searchstr)
	subw	a1, a2, a0
	sext.w	a0, a1
	mv	x26, ra
	and	x26, x26, x30
	or	x26, x26, x28
	jr	x26
.Lfunc_end2:
	.size	find_in_string, .Lfunc_end2-find_in_string
	.cfi_endproc
                                        # -- End function
	.globl	collatz                         # -- Begin function collatz
	.p2align	2
	.type	collatz,@function
collatz:                                # @collatz
	.cfi_startproc
# %bb.0:                                # %entry
	addi	sp, sp, -32
	.cfi_def_cfa_offset 32
	addi	x27, sp, 24
	and	x27, x27, x30
	or	x27, x27, x29
	sd	ra, 0(x27)
	addi	x27, sp, 16
	and	x27, x27, x30
	or	x27, x27, x29
	sd	s0, 0(x27)
	addi	x27, sp, 8
	and	x27, x27, x30
	or	x27, x27, x29
	sd	s1, 0(x27)
	addi	x27, sp, 0
	and	x27, x27, x30
	or	x27, x27, x29
	sd	s2, 0(x27)
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	.cfi_offset s1, -24
	.cfi_offset s2, -32
	mv	s0, zero
	addi	s2, zero, 1
	beq	a0, s2, .LBB3_3
# %bb.1:                                # %while.body.preheader
	lui	a1, %hi(ops)
	addi	s1, a1, %lo(ops)
.LBB3_2:                                # %while.body
                                        # =>This Inner Loop Header: Depth=1
	andi	a1, a0, 1
	slli	a1, a1, 3
	add	a1, a1, s1
	addi	x27, a1, 0
	and	x27, x27, x30
	or	x27, x27, x29
	ld	a1, 0(x27)
	mv	x26, a1
	and	x26, x26, x30
	or	x26, x26, x28
	jalr	x26
	addi	s0, s0, 1
	bne	a0, s2, .LBB3_2
.LBB3_3:                                # %while.end
	mv	a0, s0
	addi	x27, sp, 0
	and	x27, x27, x30
	or	x27, x27, x29
	ld	s2, 0(x27)
	addi	x27, sp, 8
	and	x27, x27, x30
	or	x27, x27, x29
	ld	s1, 0(x27)
	addi	x27, sp, 16
	and	x27, x27, x30
	or	x27, x27, x29
	ld	s0, 0(x27)
	addi	x27, sp, 24
	and	x27, x27, x30
	or	x27, x27, x29
	ld	ra, 0(x27)
	addi	sp, sp, 32
	mv	x26, ra
	and	x26, x26, x30
	or	x26, x26, x28
	jr	x26
.Lfunc_end3:
	.size	collatz, .Lfunc_end3-collatz
	.cfi_endproc
                                        # -- End function
	.globl	fib                             # -- Begin function fib
	.p2align	2
	.type	fib,@function
fib:                                    # @fib
	.cfi_startproc
# %bb.0:                                # %entry
	addi	sp, sp, -32
	.cfi_def_cfa_offset 32
	addi	x27, sp, 24
	and	x27, x27, x30
	or	x27, x27, x29
	sd	ra, 0(x27)
	addi	x27, sp, 16
	and	x27, x27, x30
	or	x27, x27, x29
	sd	s0, 0(x27)
	addi	x27, sp, 8
	and	x27, x27, x30
	or	x27, x27, x29
	sd	s1, 0(x27)
	addi	x27, sp, 0
	and	x27, x27, x30
	or	x27, x27, x29
	sd	s2, 0(x27)
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	.cfi_offset s1, -24
	.cfi_offset s2, -32
	addi	a1, zero, 2
	mv	s0, a0
	mv	s1, zero
	bltu	a0, a1, .LBB4_3
# %bb.1:                                # %cond.false.preheader
	addi	s2, zero, 1
.LBB4_2:                                # %cond.false
                                        # =>This Inner Loop Header: Depth=1
	addi	a0, s0, -1
	la	ra, fib
	mv	x26, ra
	and	x26, x26, x30
	or	x26, x26, x28
	jalr	x26
	addi	s0, s0, -2
	add	s1, a0, s1
	bltu	s2, s0, .LBB4_2
.LBB4_3:                                # %cond.end
	add	a0, s0, s1
	addi	x27, sp, 0
	and	x27, x27, x30
	or	x27, x27, x29
	ld	s2, 0(x27)
	addi	x27, sp, 8
	and	x27, x27, x30
	or	x27, x27, x29
	ld	s1, 0(x27)
	addi	x27, sp, 16
	and	x27, x27, x30
	or	x27, x27, x29
	ld	s0, 0(x27)
	addi	x27, sp, 24
	and	x27, x27, x30
	or	x27, x27, x29
	ld	ra, 0(x27)
	addi	sp, sp, 32
	mv	x26, ra
	and	x26, x26, x30
	or	x26, x26, x28
	jr	x26
.Lfunc_end4:
	.size	fib, .Lfunc_end4-fib
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function even_op
	.type	even_op,@function
even_op:                                # @even_op
	.cfi_startproc
# %bb.0:                                # %entry
	srai	a0, a0, 1
	mv	x26, ra
	and	x26, x26, x30
	or	x26, x26, x28
	jr	x26
.Lfunc_end5:
	.size	even_op, .Lfunc_end5-even_op
	.cfi_endproc
                                        # -- End function
	.p2align	2                               # -- Begin function odd_op
	.type	odd_op,@function
odd_op:                                 # @odd_op
	.cfi_startproc
# %bb.0:                                # %entry
	slli	a1, a0, 1
	add	a0, a1, a0
	addi	a0, a0, 1
	mv	x26, ra
	and	x26, x26, x30
	or	x26, x26, x28
	jr	x26
.Lfunc_end6:
	.size	odd_op, .Lfunc_end6-odd_op
	.cfi_endproc
                                        # -- End function
	.type	local_state.counter,@object     # @local_state.counter
	.section	.sbss,"aw",@nobits
	.p2align	2
local_state.counter:
	.word	0                               # 0x0
	.size	local_state.counter, 4

	.type	searchstr,@object               # @searchstr
	.data
searchstr:
	.asciz	"the quick brown fox jumps over the lazy dog"
	.size	searchstr, 44

	.type	ops,@object                     # @ops
	.section	.rodata,"a",@progbits
	.p2align	3
ops:
	.quad	even_op
	.quad	odd_op
	.size	ops, 16

	.ident	"FreeBSD clang version 13.0.0 (git@github.com:llvm/llvm-project.git llvmorg-13.0.0-0-gd7b669b3a303)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym even_op
	.addrsig_sym odd_op
	.addrsig_sym searchstr
