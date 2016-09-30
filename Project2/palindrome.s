## Jacob Varner & Tarence Beard Jr. -- 09/29/2016
## Partially assembled code to work with our simulator gprSim.
##
## Register usage:
## $29 - syscall parameter / return values
## $30 - syscall parameter / read
## $31 - syscall parameter / write
##
## syscall instructions:
## 0 - read string
## 1 - write string
## 2 - exit
##
## data values
## 0 = space to store string to be analyzed
## 1 = success string (ie, "This is a palindrome :)")
## 2 = failure string (ie, "This is not a palindrome :(")
##

.data
0x00000000 0
0x00000000 1
0x00000000 2

.text
la $30 0
li $29 0
syscall

li $0 0
li $1 0

lb $2 $1
beqz $2 2

addi $1 $1 1
b -4

subi $1 $1 2

bge $0 $1 6

lb $2 $0
lb $3 $1
bne $2 $3 7

addi $0 $0 1
subi $1 $1 1
b -7

la $30 1
li $29 1
syscall
b 4

la $30 2
li $29 1
syscall
b 0

li $29 2
syscall
