	AREA text, CODE, READONLY
		EXPORT example1
example1
		MOV R1, #25
		MOV R2, #75
		ADD R1, R2, R1
		BX LR ;
		END