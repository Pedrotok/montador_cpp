#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]){
	char nome_prog[30], nome_obj[30], line[256], token[100];
	FILE *fp;
	int i, j, cont_linha, cont_end, tam, flag;


	cont_linha = 1;
	cont_end = 0;

	if(argc < 3){
		printf("passe 2 argumentos\n");
	}
	else{
		strcpy(nome_prog, argv[1]);
		strcpy(nome_obj, argv[2]);

		printf("prog: %s\n obj: %s\n", nome_prog, nome_obj);
		fp = fopen(nome_prog, "r");
		if(fp != NULL){
			//passagem 1
			while (fgets(line, sizeof(line), fp)) {
				printf("linha %d:\n", cont_linha);
				j = 0;
				flag = 0;
				for(i = 0; (line[i] != '\0') && (flag == 0); i++){
					if( (line[i] != ' ') && (line[i] != '\n') && (line[i] != '\t') ){
						token[j] = line[i];
						j++;
					}
					else{
						if(j != 0){
							token[j] = '\0';
							j = 0;

							analisador(token);
							tam = strlen(token);
							tam--;
							if(strcmp(token, ";") == 0){
								printf("Linha inutil\n");
								flag = 1;
							}
							else if(token[tam] == ':'){
								token[tam] = '\0';
								printf("rotulo %s, linha %d\n", token, cont_linha);
								flag = 2;
							}
						}
	
						
					}

					
				}
				cont_linha++;
			}

			//passagem 2
			while (fgets(line, sizeof(line), fp)) {
				printf("linha %d:\n", cont_linha);
				j = 0;
				for(i = 0; line[i] != '\0'; i++){
					if( (line[i] != ' ') && (line[i] != '\n') && (line[i] != '\t') ){
						token[j] = line[i];
						j++;
					}
					else{
						if(j != 0){
							token[j] = '\0';
							j = 0;
							printf("end %d - %s\n", cont_end, token);
							cont_end++;
						}
						else if(strcmp(token, ";") == 0 )
							printf("YOO\n");
					}

					
				}
				cont_linha++;
			}
		}
		else{
			printf("Arquivo nao encontrado\n");
		}
	}

	return 0;

}
