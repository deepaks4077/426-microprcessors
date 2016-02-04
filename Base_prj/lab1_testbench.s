	AREA  text, CODE, READONLY
	EXPORT testbench_asm
	IMPORT Kalmanfilter_asm
	IMPORT myoutput
testbench_asm
	LDR R0, =myinput					; input array
	LDR R1, =myoutput  				; output array 
	LDR R2, =mystate					; address of state data struct 
	LDR R3, =length 					; int length
	LDR R4, =Kalmanfilter_asm  ; load the address of the filter subroutine
	BLX R4						
	
	ALIGN
mystate DCFS 0.1, 0.1, 0.0, 0.1, 0.0
myinput DCFS 1.0, 1.0, 1.0, 1.0
myoutput1 DCFS 0.0, 0.0, 0.0, 0.0
length DCW 4
	END
		
 