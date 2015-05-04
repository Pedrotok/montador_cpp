#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int get_rotulo(char line[], int *cont_end, char rotulo[]){
	int i, j, flag, tam, cont, end;
	char token1[100], token2[100], aux[100];

	rotulo[0] = ' ';
	flag = 0;
	cont = 0;
	j = 0;
	for(i = 0;( (line[i] != '\0') && (line[i] != '\n') && (line[i] != ';' ) && (flag == 0) ); i++){
		if( (line[i] != ' ') && (line[i] != '\t') ){
			aux[j] = line[i];
			j++;
		}
		else{
			if(j != 0){
				aux[j] = '\0';
				j = 0;
				cont++;

				tam = strlen(aux);
				tam--;
				if(aux[tam] == ':'){
					aux[tam] = '\0';
					strcpy(rotulo, aux);
					//flag = 1;
				}
				else if(cont == 2){
					flag = 1;
				}
			}
		}
	}
	if(cont != 0){
		if(rotulo[0] == ' '){
			end = *cont_end;
			//atualiza cont_end
			return end;
		}
		else{
			//atualiza cont_end
			return -1;
		}
	}
	return -1;
}

int main(int argc, char *argv[]){
	char nome_prog[30], nome_obj[30], line[256], rotulo[100];
	FILE *fp;
	int cont_linha, cont_end, end;


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
				end = get_rotulo(line, &cont_end, rotulo);
				if(rotulo[0] != ' ')
					printf("rotulo %s, linha %d\n", rotulo, cont_linha);
				cont_linha++;
			}

			
		}
		else{
			printf("Arquivo nao encontrado\n");
		}
	}

	return 0;

}
