

	.data
desc:	.ascii	"This program will create a table for \nf(x) "
	.asciz	"= (5x^3-3x^2+2x+20)/11"
prmt1:	.asciz	"\nEnter a lower limit for x: "
prmt2:	.asciz	"\nEnter an upper limit for x: "
prmt3:	.asciz	"\nEnter a value for number of rows in table: "
prmt4:	.asciz  "\nHow many bits of precision?: "
prmt5:	.asciz	"\nHow many decimal places do you want to display?: "
inp:	.asciz	"%d"
inp2:	.asciz	"%s"
outtop:	.ascii	"\n-------------"
	.asciz	"\n  X   -  f(x)"
outtab:	.asciz	"\n-------------\n"
space:	.asciz	"  -  "
n:	.asciz	"\n"
	.align	2
xend:	.byte	0
	.skip	100
xstrt:	.byte	0
	.skip	100
	.align	2
rownum: .word	0
presc:	.word	0
dpr:	.word	0
	.text
	.globl main
@@--------------MAIN-----------------------------------------------------------
main:	stmfd	sp!,{ r4-r9, lr }
	bl	prompt
	ldr	r7, =presc
	ldr	r7, [r7]
	mov	r1, r7
	ldr	r0, =xstrt
	bl	strtoSfixed
	mov	r4, r0			@r4 contains start value as fixed point
	ldr	r0, =xend
	mov	r1, r7
	bl	strtoSfixed
	mov	r5, r0			@r5 contains end value as fixed point
	sub	r7, r5, r4		@add r5 and r4 and store value
	mov	r0, r7
	ldr	r8, =rownum
	ldr	r8, [r8]
	mov	r1, r8
	sub	r1, r1, #1
	bl	udiv32
	mov	r9, r0			@value to increment by for x
					@starting x
loop:	mov	r0, r4
	bl	funcx
	mov	r2, r0
	mov	r1, r4
	bl	output
	sub	r8, r8, #1
	add	r4, r4, r9
	cmp	r8, #0
	bne	loop

	ldmfd	sp!,{ r4-r9, lr }
	mov	pc, lr
@@--------------PROMPT---------------------------------------------------------
prompt:	stmfd	sp!,{ lr }
	ldr	r0, =desc		@Output description
	bl	printf
	ldr	r0, =prmt1		@Prompt for lower limit input for x
	bl	printf
	ldr	r0, =inp2		@read lower input of x
	ldr	r1, =xstrt
	bl	scanf
	ldr	r0, =prmt2		@Prompt for upper limit input for x
	bl	printf
	ldr	r0, =inp2		@Read upper input for x
	ldr	r1, =xend
	bl	scanf
	ldr	r0, =prmt3		@Prompt for row number
	bl	printf			
	ldr	r0, =inp		@Read row number
	ldr	r1, =rownum
	bl	scanf
	ldr	r0, =prmt4		@Prompt for bits of presc
	bl	printf
	ldr	r0, =inp		@Read bits of presc
	ldr	r1, =presc
	bl	scanf
	ldr	r0, =prmt5		@Prompt for decimal display num
	bl	printf
	ldr	r0, =inp		@Read decimal display num
	ldr	r1, =dpr
	bl	scanf
	ldmfd	sp!,{ lr }
	mov	pc, lr
@@-------------FUNCX-----------------------------------------------------------
funcx:	stmfd	sp!,{ r4-r10, lr }
	ldr	r9, =presc
	ldr	r9, [r9]
	mov	r1, #32
	sub	r10, r1, r9
	mov	r1, #5
	lsl	r1, r1, r9 @
	mov	r2, #3
	lsl	r2, r2, r9 @
	smull	r3, r4, r0, r0		@long r3:r4 -> x^2
	lsr	r3, r3, r9 @
	lsl	r4, r4, r10 @
	orr	r3, r3, r4		@r3 = x^2 X(i, 16)
	smull	r5, r4, r3, r0		@long r5:r4 -> x^3
	lsr	r5, r5, r9 @
	lsl	r4, r4, r10 @
	orr	r5, r5, r4		@r5 = x^3
	smull	r5, r4, r1, r5		@long r5:r4 -> 5x^3
	lsr	r5, r5, r9 @
	lsl	r4, r4, r10 @
	orr	r5, r5, r4		@r5 = 5x^3

	smull	r3, r4, r3, r2		@long r3:r4 -> 3x^2
	lsr	r3, r3, r9 @
	lsl	r4, r4, r10 @
	orr	r3, r3, r4		@r3 = 3x^2

	mov	r1, #2
	lsl	r1, r1, r9
	smull	r6, r4, r0, r1		@long r6:r4 -> 2x
	lsr	r6, r6, r9
	lsl	r4, r4, r10
	orr	r6, r6, r4		@r6 = 2x

	sub	r0, r5, r3

	add	r0, r0, r6

	mov	r8, #20
	lsl	r8, r8, r9
	add	r0, r0, r8	     	@r0 = top of f(x)
	mov	r1, #11

	bl	sdiv32
	ldmfd	sp!,{ r4-r10, lr }
	mov	pc, lr
@@--------------OUTPUT---------------------------------------------------------
output:	stmfd	sp!,{ r4-r7, lr }
	ldr	r6, =dpr
	ldr	r6, [r6]
	ldr	r7, =presc
	ldr	r7, [r7]
	sub	r6, r6, #1
	mov	r4, r1
	mov	r5, r2
	ldr	r0, =outtop
	bl	printf
	ldr	r0, =outtab
	bl	printf
	mov	r0, r4
	mov	r1, r7
	mov	r2, r6
	bl	printS
	ldr	r0, =space
	bl	printf
	mov	r0, r5
	mov	r1, r7
	mov	r2, r6
	bl	printS
	ldr	r0, =n
	bl	printf
	ldmfd	sp!,{ r4-r7, lr }
	mov	pc, lr

