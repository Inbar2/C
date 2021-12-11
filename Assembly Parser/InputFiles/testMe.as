;file testMe;

		.entry	OUT
SEE: 	.extern FOUR
ASCIZ: 	.asciz "123"
start:	addi	$1,-5,$3
		jmp		wow
		move	$20,$4
loop:	bgt		$4,$2, OUT
		la		FOUR
		.dh 	-3
		.entry	start
		lh		$0,4,$10
		blt		$31,$9,see
OUT:	call	STR
		jmp		$4 
		call	FOUR
STR:	.asciz "123"
		.db		6
		.extern	wow
stop
see:	.dw		31,12
		.entry	see

