

	.data
n:	.word	0
prmt:	.asciz	"Please give me a natural number!\n"
inp:	.asciz	"%d\n"
	.align	2
primes:	.word	0
	.text
	.globl main
main:	stmfd	sp!,{r4-r7, lr}		@MAIN FUNCTION
	ldr	r0, =prmt	@print prompt
	bl	printf
	ldr	r0, =inp
	ldr	r1, =n
	bl	scanf		@get input
	mov	r4, #2		@r4 is i = 2
	ldr	r5, =n
	ldr	r5, [r5]	@r5 is n
	mov	r7, #0
loop1:	mov	r1, r4
	bl	isprime		@branch to isprime
	cmp	r1, #1
	ldreq	r0, =inp
	moveq	r1, r4
	bleq	printf
	addeq	r7, r7, #1
	add	r4, r4, #1	@increment i
	cmp	r4, r5		@compare i to n
	blt	loop1		@loop if i < n
	ldr	r0, =inp
	mov	r1, r7
	bl	printf

	ldmfd	sp!,{r4-r7, lr}
	mov	pc, lr
@@---------------------------------------------
	@@accepts r1 as parameter
	@@tests if r1 is a prime number
	@@returns r1 as 1 or 0
isprime:stmfd	sp!,{r4-r7, lr}		@ISPRIME FUNCTION
	mov	r7, r1			@r7 is now m
	mov	r4, r1			@r4 is now m
	mov	r0, r4
	mov	r1, #2
	bl	divide
	mov	r4, r0			@r4 is now m/2
	mov	r5, #3			@r5 is now i = 2
	mov	r6, #1			@r6 is bool isprime = true
loop2:	mov	r0, r7
	mov	r1, r5
	bl	divide 			@divide m / i
	cmp	r1, #0			@if remainder != 0
	moveq	r6, #0			@isprime = false
	cmp	r5, r4			@if i < m/2 loop
	add	r5, r5, #1		@increment i
	blt	loop2
	mov	r1, r6
	ldmfd	sp!,{r4-r7, lr}
	mov	pc, lr
@@----------------------------------------------
	@@r0 is dividend
	@@r1 is divisor		r0 / r1
	@@returns quotient into r0
	@@returns remainder into r1
@@----------------------------------------------
divide:stmfd	sp!,{lr}		@DIVIDE FUNCTION
	cmp	r1, #0
	beq	quitdiv
	mov	r2, r1
	mov	r1, r0
	mov	r0, #0
	mov	r3, #1
divstrt:cmp	r2, #0
	blt	divloop
	cmp	r2, r1
	lslls	r2, r2, #1
	lslls	r3, r3, #1
	bls	divstrt
divloop:cmp	r1, r2
	subge	r1, r1, r2
	addge	r0, r0, r3
	lsr	r2, r2, #1
	lsrs	r3, r3, #1
	bcc	divloop
	
quitdiv:ldmfd	sp!,{lr}
	mov	pc, lr




