SECTION	TEXT

loopinf: JMP	loopinf+1 ; <--- VAI DAR ERRO AQUI JMP PARA O ENDERECO 30, nao mapeado!

		STOP


SECTION	DATA
VAL:	CONST	30 		; <---- UMA CONSTANTE COM UM VALOR BEM MAIOR QUE O TAMANHO DO ARQUIVO EM WORDS!
