

		SECTION		TEXT
MOD_DIV:	BEGIN


LOAD VAL	; Loada o valor para a divisao com zero
DIV 		ZERO 		; É PARA DAR ERRO MESMO!
STORE		RESULT


	OUTPUT	RESULT
	
STOP	

SECTION	DATA
RESULT:	SPACE
VAL:	CONST	5
ZERO: CONST 0 
END