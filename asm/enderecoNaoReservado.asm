SECTION	TEXT
INPUT	Y
INPUT Y+1

INPUT Y+2
INPUT Y+3 ; <---- VAI DAR ERRO AQUI

LOAD	Y+2
ADD		Y+1
STORE	Y

ADD		X+1 ;	<--- ERRO AQUI!

OUTPUT	Y+1
OUTPUT Y+2
OUTPUT Y+3	; VAi dar erro aqui tbm
OUTPUT Y+7	; Aqui tbm!
OUTPUT X+1 	; TEM QUE DAR ERRO AQUI TBM!
STOP		; FIM!


SECTION	DATA
Y:	SPACE	3
X:	CONST	10