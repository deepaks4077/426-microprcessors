	AREA  text, CODE, READONLY
	EXPORT Kalmanfilter_asm
Kalmanfilter_asm
	PUSH {R4-R8,R10,R11}
	;INITIALIZE THE LOOP;
	VLDR.F32 S0, [R2], #0x04 	; float q
	VLDR.F32 S1, [R2], #0x04 	; float r
	VLDR.F32 S2, [R2], #0x04 	; float x
	VLDR.F32 S3, [R2], #0x04 	; float p
	VLDR.F32 S4, [R2]				 	; float k
														; state data transfer complete
	;LDR R3, [R3]							; load value contained in length into R3
	MOV R4, #0								; loop tracker 
	VLDR.F32 S6, =0.0 				;	initial value for output[i]
	
loop 												; loop definition
	CMP R4, R3
	BGE endfor 								; exit if condition not met
	VADD.F32 S3, S3, S0 			; p = p + q
	VADD.F32 S8, S3, S1 			; p + r
	VDIV.F32 S4, S3, S8 			; k = p / p + r
	VLDR.F32 S5, [R0], #0x04 	; x_obs[i] = input_array[i]
	VSUB.F32 S7, S5, S6 			; x_obs - x_pred[n-1] 
	VFMA.F32 S6, S4, S7 			; x_pre[n] = x[n-1] + k*(message - x)  
 	VMRS R5, FPSCR						; transfer FPSCR flags to R5
	AND R5,R5,#7							; #15 FOR ALL FLAGS
	CMP R5, #0					
	BGT enderror  						; branch if greater than
	
	VMLS.F32 S3, S4, S3 			; p = (1-k)*p
														; take care of variables for next iteration
	VSTR.F32 S6, [R1]					; store into output array 
	ADD R1, #0x04							; increment output array index
	ADD R4, R4, #1						; increment loop index 
	B loop										; branch back to loop
endfor
	POP {R11,R10,R4-R8}
	MOV R0, #0								; Return value is 0 if correct execution
	BX LR
enderror
	POP {R11,R10,R4-R8}
	MOV R0, #1								; Return value is 1 upon error
	BX LR
	END
		
 