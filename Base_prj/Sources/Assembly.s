	AREA  text, CODE, READONLY
	EXPORT example1
	import mystate
	import myinput
	import myoutput
example1
	MOV R4, #0
	MOV R5, #0
	ADR R0, length1
	LDR R1, [R0]
	
	BX LR
	
	ALIGN
mystate1 DCW 1, 1, 1, 0, 0
myinput1 DCW 
myoutput1 DCW 0
length1 DCW 4

	END