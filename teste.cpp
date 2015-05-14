#include<stdio.h>
#include<string.h>

void RemoveSpaces(char* source){
  char* i = source;
  char* j = source;
  while(*j != 0)
  {
    *i = *j++;
    if(*i != ' ')
      i++;
  }
  *i = 0;
}

int main(){
	char *aux;
	char frase[] = "COPY A  , B";
	
	aux = strtok(frase," \t");
	aux = strtok(NULL, ", ");
	aux = strtok(NULL, ", \t");
	//RemoveSpaces(aux);
	printf("|%s|\n", aux);
	
	
	return 0;
}