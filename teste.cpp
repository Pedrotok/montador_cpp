#include<iostream>
#include<string>
#include<cstring>
#include<cstdio>
//#include <unistd.h>
#include<list>
#include<algorithm>

using namespace std;


struct tipo_erro {
  	int linha;
  	string msg;
} ;

int is_valid(string data){
	int i;
	
	if( (data[0] < 65) || ( (data[0] > 90) && (data[0] < 97) && (data[0] != 95) ) || (data[0] > 122) )
		return 0;
	
	for(i = 1; i < data.length(); i++){
		if( (data[i] < 48) || ( (data[i] > 57) && (data[i] < 65) ) || ( (data[0] > 90) && (data[0] < 97) && (data[0] != 95) ) || (data[0] > 122) )
			return 0;
	}
	return 1;
}

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
	tipo_erro erro_temp;
	list<tipo_erro> erro;
	int num, tam, copy;
	
	char data[] = "Happy:s";
	
	char *aux;
	char frase[] = "COPY A, B";
	
	aux = strtok(frase," \t");
	cout << "|" << aux << "|" << endl;
	
	copy = 0;
	aux = strtok(NULL, " \t");
	tam = strlen(aux);
	tam--;
	if(aux[tam] == ','){
		aux[tam] = '\0';
		copy = 1;
		//oper1 = aux
		//aux = strtok(NULL, " \t");
		//oper2 = aux
		cout << "|" << aux << "|" << endl;
		aux = strtok(NULL, " \t");
		cout << "|" << aux << "|" << endl;
	}
	else{
		//oper1 = aux
		cout << "|" << aux << "|" << endl;
		aux = strtok(NULL, " \t");
		if(strcmp(aux, ",") == 0){
			aux = strtok(NULL, " \t");
			cout << "|" << aux << "|" << endl;
		}
	}
	//cout << "|" << aux << "|" << endl;
	aux = strtok(NULL, " \t");
	cout << "|" << aux << "|" << endl;
	aux = strtok(NULL, " \t");
	cout << "|" << aux << "|" << endl;
	aux = strtok(NULL, " \t");
	cout << "|" << aux << "|" << endl;
	
	
	
	
	//RemoveSpaces(aux);
	//erro.msg.clear();
	
	/*
	erro_temp.msg = "TESTE 1\n"; 
	erro_temp.linha = 1;
	erro.push_back(erro_temp);
	
	erro_temp.msg = "TESTE 2\n"; 
	erro_temp.linha = 2;
	erro.push_back(erro_temp);
	
	for(list<tipo_erro>::iterator list_iter = erro.begin();   list_iter != erro.end(); list_iter++){
		//cout << list_iter->linha <<endl;
	}*/
	
	//cout << is_valid(data) << endl;

	return 0;
}