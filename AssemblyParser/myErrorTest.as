;line
;label testing
                                                                         Outside80: .asciz "error cut"
:
 : 
 tree
tree: 
tree :
2tree:
.tree:
$tree:
tree::
tree::mvlo &23, &2
green tea:
tr?ee:
extern:
entry:
.asciz:
asciz:
asciz: space
move: 
dw:
define: db 1
define: .db 3
WAR: .entry K
WAR2: .extern E
NIGHT: .hello 3
blt $1, $3, asciz
blt $1, $3, .asciz
blt $1, $3, move
blt $1, $3, extern
  : blt $1 , $2, label
H1234567890A1234567890B1234567890ELLO: blt $1 , $2, label
HELLO:: blt $1 , $2, label

;asciz testing
.asciz
.asciz "	tab"
.asciz bob
.asciz "bob
.asciz bob"
.asciz a"bob"
.asciz "bob"a
.asciz "bob" a
.asciz"bob"
.ascizz "no"

;data testing
.hello
.db
.db,
.db open
.db $1
.db 1.1
.db 1&2
.db 1 2
.db 1 , , 2
.db 1 ,,,2
.db 1, 2,
.db 1, 2 ,
.db 1, 2,,,,
.db ,1, 2
.dh 1, --2
.dh 1	, +-2
.dh 1	, ++3
.db 128
.db -129
.db6,7
.dh
.dh a
.dh 1 , 2, num
.dh 65536
.dh 32768
.dh -32769
.dw 4294967296
.dw 2147483648
.dw 992147483648

.extern
.extern,
.extern ?
.extern 34
.extern "test"
.extern good bye
.extern dog, cat
.extern one,

;OK commands - NO errors
ASCIZ: .dh 3
BLT: .asciz "next"
blt $1, $3, Asciz
.asciz ""
.asciz "  "
.asciz """"""
	.asciz "bob"
	 .asciz " bob "
.asciz "bob""
.asciz "bob" "
.asciz "bob""bob"
.asciz "bob"bob"
.asciz "bob bob"
.asciz "bob\bob"
.asciz "123bob"
.asciz "b" "o" "b"
	.asciz "good with spaces at the end"	     
.asciz      	"spaces good "
.extern Good
.db 127
.db -128
.dh 32767
.dh -32768
.dw 2147483647
.dw -2147483648

