; instruction 1st errors testing	
;R opcode
add 
add $1,$2, 7, $3
BAD: add $1 , 		$2 ,$-3	 	
add		$4 , , $5, $6 
add you, $1, $4
add$1,$2,$3
add ,$5
add $5 $5
add $6 $7 $8
add $7 ,$5
add $10 ,$20, $30, $40
add $2 ,$5 , $8, 45
add $5,$$6
add $1,	A2, $3
add $4,,$5,$6		
add $5,$6,$7,
add $11,$12,$13 , , 
add 25, $4, $5
add -1 , 2,3
add $8 , $32,$7
add $31 , $0, 1$
add $-1,$5,$6
add $1 , $5, label
addg &3,	$7, $8

;R opCode
move 
move $1 ,2
	move 2 , 4
move $3	$4
move $5 , $
move $12 , $45
move $1, label
move $2 , , $5
move , $3 , $5
move $4 ,$5,
move $9 , $5 ,,

;I opcode
BAD_1: addi $5, 8 , $9
1BAD: addi $1 , 2 , $3
a-Bad: addi $1 , 2 , $3
BadLine:				                                                              addi $1 , 2 , $3
CheckNum: addi $1 , 32768 ,$3	 
IopCode2:		addi $1 , -32769 ,$3	
IopCode2:	addi $1 , 		2 ,$3	
addi
addi ,$5
addi $5 5 
addi $2 5 $1
addi $9 ,$5
addi $10 ,20, $30, $40
addi $5 ,$4 , $8, 45
addi $5 ,5 , 8, 45
addi $5 ,5 , 9
addi $$5, 6, $10
addi $1,	A2, $3
addi $5	, ,	$6,$7		
addi 25, 4, $5
addi $5, 6	 7
addi $5,6,$7,
addi $5,6,$7 , , 
addi 1 , 2,3
addi $8 , 30,$32
addi $-1,5,$6
addi $1 , 5, label
addi $4, $6, asciz
addi "hello" $1

;BRANCHED INSTRUCTIONS
copy: blt $3,	label,$4
NEW: blt $3,$4, asciz
copy: bgt 	$3	,	$4	,	label
bgt 	$3	,	$4	,	entry
							                                                       LONG: blt $3,$4,label
blt
blt 	$3	,	$4
blt 	$3	,	$4, 
blt 	$3	,	$4, .extern
blt 	$3	,	$40	,	label
bne $1,2,label
blt 1,$2,label
blt $1,label,label
blt $1,$2,,label
blt $1,$2,label ,
blt $1,$2,label two ,
blt $1,$2,  label	hello
blt $1,$2,label, say ,
blt $1,$2,label , ,
blt $4 , $5,$6 
 blt $1 ,$2 ,-lable	
 $1 ,$2 ,lable	
	 "hello"	
addi $1,$2,label
nori $1,$2,label
lb $1,$2,label
lb $1,4,label                                                                                                                                                        

;J opcode
       							                                                           VERYLONG: jmp $3
jmp move
jmp $2,
jmp label,
jmp $3 ,
jmp label1 ,
jmp label2 , ,
jmp $4 $1
jmp label see
jmp $5, $1
jmp label, go
jmp $6, , $1
jmp label, , come
jmp $7 , $1 , ,
jmp label , go	,	,
jmp $8, label
jmp label, $1
jmp $hello9
jmp $label
jmp $99
jmp $1.0
	jmp "wow"
jmp bye1234567890?
jmp byeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
call $4
la $4
call TAG,
la TAG,
call MY TAG
la MY TAG
call MY TAG1,
la MY TAG2,
stop 123
stop $1
stop label
stop:

;OK
addi $1,2,$3
addi $31 , 0,$7
addi $31 , -11 ,$7
blt $1,$2,label
	blt $1,$2,label
 blt $1,$2,label
BLT:   blt $3, $4, label
Blt:   blt $3, $4, label
jmp $1
jmp $010
jmp bye1234567890
jmp label
jmp $1  
jmp label  
 	jmp $1  
 	jmp label 
 call TAG1
la TAG2
stop
STOP: stop
