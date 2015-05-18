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
	int i, j, k, num, aux, tam, factor, comeco;
	
	comeco = 0;
	factor = 1;
	aux = data[0] - 48;
	if( ((aux < 0) || (aux > 9)) && (aux != -3) ){
		cout << aux+1 << " passando aqui\n";
		return -1;
	}
	else if(aux == -3){
		factor = -1;
		comeco = 1;
		if(data[1] == '\0')
			return -1;
	}
	
	num = 0;
	tam = strlen(data);
	for(i = comeco, j = tam - 1; i < tam; i++, j--){
		aux = data[i] - 48;
		if( (aux < 0) || (aux > 9) )
			return -1;
		for(k = 0; k < j - comeco; k++)
			aux*=10;
		
		num += aux;
	}
	return num*factor;
}

int main(){
	tipo_erro erro_temp;
	list<tipo_erro> erro;
	int num, tam, copy;
	
	char data[] = "-";
	
	char *aux = data;
	
	
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
	num = pedroi(aux);
	cout << num+1 << endl;

	return 0;
}