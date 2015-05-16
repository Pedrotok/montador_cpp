#include<iostream>
#include<string>
#include<cstring>
//#include<cstdio>
//#include <unistd.h>
#include<list>
#include<algorithm>

using namespace std;


struct tipo_erro {
  	int linha;
  	string msg;
} ;


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

int pedroi(char data[]){
	int i, j, k, num, aux, tam;
	
	num = 0;
	tam = strlen(data);
	for(i = 0, j = tam - 1; i < tam; i++, j--){
		aux = data[i] - 48;
		if( (aux < 0) || (aux > 9) )
			return -1;
		for(k = 0; k < j; k++)
			aux*=10;
		
		num += aux;
	}
	return num;
}

int main(){
	tipo_erro erro;
	int num;
	
	char data[] = "530";
	char *aux = data;
	/*char *aux;
	char frase[] = "COPY A  , B";
	
	aux = strtok(frase," \t");
	aux = strtok(NULL, ", ");
	aux = strtok(NULL, ", \t");
	//RemoveSpaces(aux);*/
	erro.msg.clear();
	
	num = pedroi(aux);
	cout << num+1 << endl;
	return 0;
}