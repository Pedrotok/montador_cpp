SECTION	TEXT
		INPUT	Y	; RECEBE A ENTRADA
		LOAD	Y	
		JMPN	FIM				;TESTANDO CASOS 1 e 0
		COPY	ONE,	FIB_1
		COPY	FIB_1, RESULT_FIB
		SUB		ONE
		JMPN	FIM				; SAI DO PROGRAM CASO ENTRADA SEJA 0
		JMPZ	FIM
		STORE	AUX
		COPY	ONE,	FIB_2
		COPY	FIB_2,	RESULT_FIB
		SUB		ONE
		JMPN	FIM			; SAI DO PROGRAM CASO ENTRADA SEJA 1
		JMPZ	FIM

LOOP:	STORE	AUX		; LOOP COMECA AQUI
		LOAD	FIB_2
		ADD		FIB_1
		STORE	RESULT_FIB
		COPY	FIB_2,	FIB_1
		STORE	FIB_2
		LOAD	AUX
		SUB		ONE
		JMPZ	FIM
		JMP 	LOOP

FIM:	OUTPUT RESULT_FIB
		STOP


SECTION	DATA
Y:	SPACE
FIB_1:	SPACE 
FIB_2:	SPACE
ONE:	CONST 1
RESULT_FIB:	SPACE
AUX:	SPACE