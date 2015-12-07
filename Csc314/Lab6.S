
@@--------------------DATA-----------------------
	.data
ptot:	.word	0
ntot:	.word	0
dtot:	.word	0
qtot:	.word	0
pcnt:	.word	0	@PENNIES: WORTH 1
ncnt:	.word	0	@NICKLES: WORTH 5
dcnt:	.word	0	@DIMES: WORTH 10
qcnt:	.word	0	@QUARTERS: WORTH 25
inp:	.asciz	"%d %d %d %d"	@INPUT
prmt:	.ascii	"Enter the number of pennies, nickles, dimes, and quarters"
	.asciz	" for week %d\n"
out1:	.ascii	"Over four weeks you have collected %d pennies, %d nickles"
	.asciz	", %d dimes, and %d quarters.\n"
out2:	.asciz	"This comes to $%d.%d\n"
out3:	.asciz	"Your weekly average is $%d.%d\n"
out4:	.asciz	"Your estimated yearly savings is $%d.%d\n"
out5:	.asciz	"%d\n"
@@--------------------TEXT-----------------------
	.text
	.globl main
@@--------------------MAIN-----------------------
main:	stmfd	sp!,{ r4-r5, lr }
	mov	r5, #1
loop:	ldr	r0, =prmt	@Load string for prompt
	mov	r1, r5		@r4 = i
	bl	printf		
	ldr	r0, =inp	@Load string for scan
	ldr	r1, =pcnt	@r1 = pennies
	ldr	r2, =ncnt	@r2 = nickles
	ldr	r3, =dcnt	@r3 = dimes
	ldr	r4, =qcnt	@r4 = quarters ( put on stack )
	str	r4, [sp, #-4]!
	bl	scanf
	ldr	r1, =pcnt	@@accumulate pennies
	ldr	r1, [r1]
	ldr	r2, =ptot
	ldr	r3, =ptot	
	ldr	r2, [r2]
	add	r2, r2, r1
	str	r2, [r3]
	ldr	r1, =ncnt	@@accumulate nickles
	ldr	r1, [r1]
	ldr	r2, =ntot
	ldr	r3, =ntot
	ldr	r2, [r2]
	add	r2, r2, r1
	str	r2, [r3]	
	ldr	r1, =dcnt	@@accumulate dimes
	ldr	r1, [r1]
	ldr	r2, =dtot
	ldr	r3, =dtot
	ldr	r2, [r2]
	add	r2, r2, r1
	str	r2, [r3]
	ldr	r1, =qcnt	@@accumulate quarters
	ldr	r1, [r1]
	ldr	r2, =qtot
	ldr	r3, =qtot
	ldr	r2, [r2]
	add	r2, r2, r1
	str 	r2, [r3]	
	add	sp, sp, #4	@pop quarter address from stack
	add	r5, r5, #1	@increment i
	cmp	r5, #5
	blt	loop		@if i < 5 loop
				
	ldr	r0, =out1	@Output total num of coins for each denomination
	ldr	r1, =ptot
	ldr	r1, [r1]
	ldr	r2, =ntot
	ldr	r2, [r2]
	ldr	r3, =dtot
	ldr	r3, [r3]
	ldr	r4, =qtot
	ldr	r4, [r4]
	str	r4, [sp, #-4]!
	bl	printf
	add	sp, sp, #4	@pop quarters from stack

	mov	r4, #0		@find total
	ldr	r0, =qtot	
	ldr	r0, [r0]
	mov	r1, #25
	bl	mully
	add	r4, r4, r0
	ldr	r0, =dtot
	ldr	r0, [r0]
	mov	r1, #10
	bl	mully
	add	r4, r4, r0
	ldr	r0, =ntot
	ldr	r0, [r0]
	mov	r1, #5
	bl	mully
	add	r4, r4, r0
	ldr	r0, =ptot
	ldr	r0, [r0]
	add	r4, r4, r0
		
	mov	r0, r4		@Output total value
	bl	divide100	@divide for dollar values
	mov	r3, r0		@store dollars in r3
	mov	r1, #100	
	bl	mully		@multiply dollar vlue by 100
	mov	r1, r3
	sub	r2, r4, r0	@total - dollar*100 = cent total
	ldr	r0, =out2
	bl	printf

	mov	r0, r4		@find average
	bl	divide4

	mov	r5, r0		@store average in r5
	bl	divide100
	mov	r3, r0
	mov	r1, #100
	bl	mully
	mov	r1, r3
	sub	r2, r5, r0
	ldr	r0, =out3
	bl	printf

	mov	r0, r4
	mov	r1, #13
	bl	mully	
	
	mov	r5, r0
	bl	divide100
	mov	r3, r0
	mov	r1, #100
	bl	mully
	mov	r1, r3
	sub	r2, r5, r0
	ldr	r0, =out4
	bl	printf


	ldmfd	sp!,{ r4-r5, lr }
	mov	pc, lr
@@--------------------DIVIDE---------------------
divide4:stmfd	sp!,{ lr }
	lsr	r0, r0, #2
	ldmfd	sp!,{ lr }
	mov	pc, lr
divide100:	stmfd	sp!, { lr }
		mov	r1, #656
		bl	mully
		lsr	r0, r0, #16
		ldmfd	sp!, { lr }
		mov	pc, lr
@@------------------MULTIPLY--------------------
@@--r0 is arg1 and return register
@@--r1 is arg2 and is either 1, 5, 10, or 25
mully:	stmfd	sp!,{ lr }
	cmp	r1, #1
	beq	end
	cmp	r1, #5
	beq	mul5
	cmp	r1, #10
	beq	mul10
	cmp	r1, #100
	beq	mul100
	cmp	r1, #13
	beq	mul13
	cmp	r1, #656
	beq	mul656
mul25:	mov	r1, #0
	mov	r2, r0, lsl #4	@times 16
	add	r1, r0, lsl #3	@times 8
	add	r2, r2, r1	@x16 + x8
	add	r0, r2, r0
	b	end
mul10:	mov	r2, r0, lsl #3		@times 8
	add	r0, r2, r0, lsl #1	@x8 + x2
	b	end
mul5:	mov	r2, r0, lsl #2		@times 4
	add	r0, r2, r0		@x4 + x1
	b	end
mul100: mov	r2, r0, lsl #7		@times 128
	sub	r2, r2, r0, lsl #5      @x128 - x32
	add	r0, r2, r0, lsl #2	@+x4
	b	end
mul13:	mov	r2, r0, lsl #4
	sub	r2, r2, r0, lsl #2
	add	r0, r2, r0
	b	end
mul656:	mov	r2, r0, lsl #9
	add	r2, r2, r0, lsl #7
	add	r0, r2, r0, lsl #4
end:	ldmfd	sp!,{ lr }
	mov	pc, lr	
