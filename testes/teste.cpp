#include<iostream>
#include<string>
#include<cstring>
#include<cstdio>
//#include <unistd.h>
#include<list>
#include<map>
#include<algorithm>

using namespace std;

//struct tipo_uso{
//	lista<int> end;
//	string rot;
//};

struct tipo_erro {
  	int linha;
  	string msg;
} ;

void tira_primeiro(char data[]){
	int i, tam;
	
	tam = strlen(data);
	tam--;
	for(i = 0; i < tam; i++){
		data[i] = data[i+1];
	}
	data[i] = '\0';
}

int convertHexa(char data[]){
	
	
}

int stringToInt(char data[]){
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

int converterNum(char data[]){
	char aux[100];
	int i, j, num;
	num = -1;
	
	if( (data[0] == 48) && (data[1] == 120) ){
		for(i = 2; (data[i] != '\0') ; i++){
			if( (data[i] < 48) || ( (data[i] > 57) && (data[i] < 65) ) || ( (data[i] > 70) && (data[i] < 97) ) || (data[i] > 102) ){
				return -1;
			}
		}
		//aux[j] = '\0';
		//cout << aux << endl;
		//num = stoul(data, nullptr, 16);
		//cout << num+1 << endl;
	}
	else
		num = stringToInt(data);
	
	return num;
}


int separar(char data[]){
	int i, num, j;
	string a, num_string;
	char aux[100];
	num = -1;
	//a.clear();
	for(i = 0; ( (data[i] != '\0') && (data[i] != '+') ); i++);
	if(data[i] == '+'){
		data[i] = '\0';
		i++;
		for(j= 0; (data[i] != '\0') ; i++, j++){
			aux[j] = data[i];
		//cout << "WAAAT\n";
		}
		aux[j] = '\0';

		num = stringToInt(aux);
		return num;
	}
	return 0;
	
}




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

int tem_virgula(char data[]){
	int i = 0;
	int flag = 0;

	while(data[i] != '\0'){
		if( (data[i] != ' ') && (data[i] != '\t') && (data[i] != ',') ){
			flag = 1;
		}
		if(data[i] == ','){
			if(flag == 1)
				return 1;
			else 
				return 0;
		}
		i++;
	}
	return 0;
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
	list<int> num_list, num_list2;
	map<string, list<int> > tab_uso;
	
	int num, tam, copy, i, j;
	
	char line_aux[] = ",ALOHAHA";
	
	char *aux, *op1, *op2 ;
	
	aux = line_aux;
	tira_primeiro(aux);
	
	
	cout << aux << endl;
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

	//num = converterNum(aux);
	//cout << "HI: " << num+1 << endl;

	return 0;
}