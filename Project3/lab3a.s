.text

li $1, 0
li $2, 32

subi $2, $2, 1
nop
nop
nop
nop
nop
bge  $2, $1, -6
nop

li $2, 10
syscall