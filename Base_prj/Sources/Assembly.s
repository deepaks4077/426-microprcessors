	AREA  text, CODE, READONLY
	EXPORT example1
	IMPORT  myoutput
example1
	;INITIALIZE THE LOOP
	LDR R0, =myinput		; input array
	LDR R1, =myoutput  ; output array 
	
	; transfer myoutput1 to myoutput HERE
	LDR R2, =mystate		; address of state data struct 
	VLDR.F32 S0, [R2], #0x04 ; float q
	VLDR.F32 S1, [R2], #0x04 ; float r
	VLDR.F32 S2, [R2], #0x04 ; float x
	VLDR.F32 S3, [R2], #0x04 ; float p
	VLDR.F32 S4, [R2]				 ; float k
	; state data transfer complete
	
	LDR R3, =length 	; int length
	MOV R4, #0					; loop tracker 
	VLDR.F32 S6, =0.0 	; initial value for output[i]
loop 
	CMP R4, R3
	BGT endfor ;execute body of loop
	VADD.F32 S3, S3, S0 ; p = p + q
	VADD.F32 S8, S3, S1 ; p + r
	VDIV.F32 S4, S3, S8 ; k = p / p + r
	VLDR.F32 S5, [R0], #0x04 ; x_obs[i] = input_array[i]
	VSUB.F32 S7, S5, S6 ; x_obs - x_pred[n-1] 
	VFMA.F32 S6, S4, S7 ; x_pre[n] = x[n-1] + k*(message - x)   	
	VMUL.F32 S10, S3, S4 ; p*k
																			;VLMS.f32 S3, S4, S3 ==? p = (1-k)*p
	VSUB.F32 S3, S3, S10 ; p = (1-k)*p
	;take care of variables for next iteration
	
	VMOV.F32 S9, S6
	VSTR.F32 S9, [R1]
	ADD R1, #0x04
	ADD R4, R4, #1
	B loop
endfor
	BX LR
	
	ALIGN
mystate DCFS 0.1, 0.1, 0.0, 0.1, 0.0
myinput DCFS 1.0, 1.0, 1.0, 1.0
myoutput1 DCFS 0.0, 0.0, 0.0, 0.0
length DCW 4
	END
		
 