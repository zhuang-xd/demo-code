/*
 * start.S
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*
 * Exception vector table
 */
.text

	.global	_start
_start:
@ 异常向量表
	b reset
	ldr pc, _undefined_instruction
	ldr pc, _software_interrupt
	ldr pc, _prefetch_abort
	ldr pc, _data_abort
	ldr pc, _not_used
	ldr pc, _irq
	ldr pc, _fiq

_undefined_instruction:
	.word undefined_instruction
_software_interrupt:
	.word software_interrupt
_prefetch_abort:
	.word prefetch_abort
_data_abort:
	.word data_abort
_not_used:
	.word not_used
_irq:
	.word irq
_fiq:
	.word fiq

 /* The actual reset code */
reset:
	@ 重新映射异常向量表的入口地址
	/* Set Vector Base Address Register */
	mrc p15, 0, r0, c1, c0, 0
	bic r0, #(1<<13)
	mcr p15, 0, r0, c1, c0, 0
	ldr	r0,=0xc0008000
	mcr	p15,0,r0,c12,c0,0		@ Vector Base Address Register

	/* Set the cpu to svc32 mode */
	mrs r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0xd3
	msr cpsr, r0

	/* Enable NEON/VFP unit */
	mrc p15, #0, r1, c1, c0, #2
	orr r1, r1, #(0xf << 20)
	mcr p15, #0, r1, c1, c0, #2
	mov r1, #0
	mcr p15, #0, r1, c7, c5, #4
	mov r0, #0x40000000
	fmxr fpexc, r0

	/* Cache init */
	mrc	p15, 0, r0, c0, c0, 0
	and	r1, r0, #0x00f00000
	and	r2, r0, #0x0000000f
	orr r2, r2, r1, lsr #20-4
	cmp r2, #0x30
	mrceq p15, 0, r0, c1, c0, 1
	orreq r0, r0, #0x6
	mcreq p15, 0, r0, c1, c0, 1

	/* Invalidate L1 I/D */
	mov r0, #0
	mcr	p15, 0, r0, c8, c7, 0
	mcr	p15, 0, r0, c7, c5, 0

	/* Disable mmu stuff and caches */
	mrc p15, 0, r0, c1, c0, 0
	bic r0, r0, #0x00002000
	bic r0, r0, #0x00000007
	orr r0, r0, #0x00001000
	orr r0, r0, #0x00000002
	orr r0, r0, #0x00000800
	mcr p15, 0, r0, c1, c0, 0

	/* Initialize stacks */
	@ 初始化各种模式下的栈空间
init_stack:
	ldr	r0, stacktop        /*get stack top pointer*/

	/********svc mode stack********/
	mov	sp, r0
	sub	r0, #128*4          /*512 byte  for irq mode of stack*/
	/********irq mode stack********/
	msr	cpsr, #0xd2
	mov	sp, r0
	sub	r0, #128*4          /*512 byte  for fiq mode of stack*/
	/********fiq mode stack********/
	msr	cpsr, #0xd1
	mov	sp, r0
	sub	r0, #0
	/********abort mode stack******/
	msr	cpsr, #0xd7
	mov	sp, r0
	sub	r0, #0
	/********undefine mode stack**/
	msr	cpsr, #0xdb
	mov	sp, r0
	sub	r0, #0
    /***sys mode and usr mode stack***/
	msr	cpsr, #0x10
	mov	sp, r0             /*1024 byte  for user mode of stack*/

    /******clear bss section********/
	@ 清除BSS段
	ldr	r0, =__bss_start	/* this is auto-relocated! */
	ldr	r1, =__bss_end__	/* this is auto-relocated! */
	mov	r2, #0x00000000		/* prepare zero to clear BSS */

clbss_l: cmp r0, r1			/* while not at end of BSS */
	strlo r2, [r0]			/* clear 32-bit BSS word */
	addlo r0, r0, #4		/* move to next */
	blo	clbss_l

	/* Call _main */
	ldr pc, =main     @ 汇编调用C  跳转到main.c文件的main函数中


/*
 * Exception handlers
 */
	.align 5  // 2的5次方，=32bit 也就是4字节对其
undefined_instruction:
	b	.

	.align 5
software_interrupt:
	b	.

	.align 5
prefetch_abort:
	b	.

	.align 5
data_abort:
	b	.

	.align 5
not_used:
	b	.

	.align 5
	.global irq
irq:
	sub  lr, lr, #4
	stmfd sp!, {r0-r12, lr}
	bl do_irq   @中断处理函数
	ldmfd sp!, {r0-r12, pc}^

	.align 5
	.global fiq
fiq:
	b .
stacktop:    .word 		stack + 4 * 512
.data

stack:	 .space  4 * 512


