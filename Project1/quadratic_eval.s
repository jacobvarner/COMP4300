#
# Code to evaluate a quadratic
#
# Code corrected by Group 12
#

	.text

main:
	lw $t0, X
	lw $t1, A
	lw $t2, B
	lw $t3, C

	# mul $t0, $t0, $t4  # X**2
	mul $t4, $t0, $t0 # Destination should be the first argument
	# mul $t1, $t4, $t4  # A*X**2
	mul $t4, $t1, $t4 # Destination should be the first argument
	# mul $t0, $t2, $t5  # B*X
	mul $t5, $t0, $t2 # Destination should be the first argument
	add $t4, $t5, $t4  # A*X**2 + B*X
	# add $t3, $t4, $t4  # A*X**2 + B*X + C
	add $t4, $t3, $t4 # Destination should be the first argument

	la $a0, ans        # print a string
	li $v0, 4					 # 4 = 'print_str' instruction
	syscall

	move $a0, $t4      # now the result
	li $v0, 1					 # 1 = 'print_int' instruction
	syscall

 	la $a0, nl         # and a newline
	li $v0, 4					 # 4 = 'print_str' instruction
	syscall

	li $v0, 10         # 10 = 'exit' instruction
	syscall

	.data
X:	.word 3
A:	.word 7
B:	.word 5
C:	.word 4
ans:	.asciiz "Answer = "
nl:	.asciiz "\n"
