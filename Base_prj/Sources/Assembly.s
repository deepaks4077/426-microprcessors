	AREA  text, CODE, READONLY
	EXPORT Kalmanfilter_asm
Kalmanfilter_asm
	VLDR.F32 S0, [R2], #0x04 ; float q
	VLDR.F32 S1, [R2], #0x04 ; float r
	VLDR.F32 S2, [R2], #0x04 ; float x
	VLDR.F32 S3, [R2], #0x04 ; float p
	VLDR.F32 S4, [R2]				 ; float k
	; state data transfer complete
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
	;take care of variables for next iteration
	VMUL.F32 S10, S3, S4 ; p*k
	VSUB.F32 S3, S3, S10 ; p = (1-k)*p (===? VLMS.f32 S3, S4, S3 ==? p = (1-k)*p)
	VMOV.F32 S9, S6
	VSTR.F32 S9, [R1]
	ADD R1, #0x04
	ADD R4, R4, #1
	B loop
endfor
	END
		
 