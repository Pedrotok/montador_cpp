/*
Estudantes: Lucas Vanderlei Fernandes & Pedro Henrique Ferreira
Matricula: 110015975 & 110018907
Software Básico 1/2015
Trabalho 1

Descricao: Montador de duas passagens do Assembly inventado. Programa foi feito totalmente em linux, ubuntu.

Sobre: A pasta tem o codigo fonte do montador e do ligador. Para compilar o montador, apenas dê um make.
Para executar o montador, faça "./mont arq1 arq2", onde arq1 eh o arquivo de ENTRADA arq1.asm e arq2 eh a SAIDA arq2.o

Obs: Foi utilizado o C++11!
*/

#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<map>
#include<list>
#include<algorithm>
#include"header/init.hpp"


using namespace std;

struct tipo_erro {
  	int linha;
  	string msg;
} ;

struct tipo_simbolo {
	int end, tam, vlr_const;
	bool is_rotulo, is_const, valido;
};


// VARIAVEIS GLOBAIS 
map<string, tipo_inst> inst;
map<string, tipo_dir> dir;
list<tipo_erro> erro_list;
map<string, tipo_simbolo> tab_simb;
map<string, int> tab_def;
map<string, list<int> > tab_uso;


// funcao que recebe uma mensagem de erro e a linha em que o erro ocorreu. Coloca esse erro na lista de erros
void push_erro(string msg, int linha){
	tipo_erro erro;
	erro.msg = msg; 
	erro.linha = linha;
	erro_list.push_back(erro);
}

// param: uma string
// return: nada
// funcao: retira o ultimo char de uma string. Funcao usada para a instrucao COPY
void tira_primeiro(char data[]){
	int i, tam;
	
	tam = strlen(data);
	tam--;
	for(i = 0; i < tam; i++){
		data[i] = data[i+1];
	}
	data[i] = '\0';
}

// param: uma string
// return: 1 caso a string seja um rotulo/variavel/operando valido. 0 caso contrario
// funcao: isso ai
int is_valid(string data){
	int i, tam;
	
	if(data.length() > 100)
		return 0;
	
	if( (data[0] < 65) || ( (data[0] > 90) && (data[0] < 97) && (data[0] != 95) ) || (data[0] > 122) )
		return 0;
	
	tam = data.length();
	for(i = 1; i < tam; i++){
		if( (data[i] < 48) || ( (data[i] > 57) && (data[i] < 65) ) || ( (data[0] > 90) && (data[0] < 97) && (data[0] != 95) ) || (data[0] > 122) )
			return 0;
	}
	return 1;
}

// param: uma string e o endereco de uma var int 
// return: false, caso a string represente um numero invalido. True, caso contrario.
// funcao: converte a String data para um num int
bool stringToInt(char data[], int &num){
	int i;
	
	if( ((data[0] < 48) || (data[0] > 57)) && (data[0] != 45) )
		return false;
	for(i = 1; data[i] != '\0'; i++){
		if( (data[i] < 48) || (data[i] > 57) ){
			return false;
		}
	}
	num = stoi(data);
	return true;
}


// param: ponteiro para um vetor de char. Um numero em forma de string (decimal ou hexadecimal)
// return: -1, caso o numero passado seja invalido. O numero em decimal, caso o numero passado esteja em decimal ou hexadecimal
// funcao: recebe um numero(decimal ou hexa) em forma de string e retorna o numero como int
int converterNum(char data[], int &num){
	int i;
	i = 0;
	
	if(stringToInt(data, num))
		return true;
	
	if(data[0] == 45)
		i++;
	if( (data[i] == 48) && (data[i+1] == 88) ){
		i+=2;
		for(; (data[i] != '\0') ; i++){
			if( (data[i] < 48) || ( (data[i] > 57) && (data[i] < 65) ) || (data[i] > 70) ){
				return false;
			}
		}
		num = stoul(data, nullptr, 16);
		//cout << num+1 << endl;
		return true;
	}
	
	return false;
}


// param: ponteiro para um vetor de char. Um operando (A, A+1, R+3, ...)
// return: numero depois do +, em caso de receber (A+2). 0, caso receba uma string sem '+'. -1, caso o num depois do '+' seja invalido
// funcao: recebe um operando (A, A+1, R+2), retorna o numero depois do '+' e a string antes do '+'
int separar(char data[]){
	int i, num, j;
	char aux[100];
	
	num = -1;
	//a.clear();
	for(i = 0; ( (data[i] != '\0') && (data[i] != '+') ); i++);
	if(data[i] == '+'){
		if(data[i+1] == '\0')
			return 0;
		data[i] = '\0';
		i++;
		for(j= 0; (data[i] != '\0') ; i++, j++){
			aux[j] = data[i];
		}
		aux[j] = '\0';

		if(stringToInt(aux, num)){
			if(num >= 0)
				return num;
		}
		return -1;
	}
	return 0;
	
}


// param: ponteiro de um arquivo e uma string
// return: 0, caso seja o fim do arquivo. 1, caso contrario
// funcao: pega toda a linha ate encontrar um ';' ou um '\n'
int getline2(ifstream &fp, string &line){
	char c;

	line.clear();
	while( (!fp.eof()) && (fp.get(c)) && (c != '\n') && (c != ';') && (c != '\r') ){
		line.push_back(c);
	}
	if( (c == ';') || (c== '\r') ){
		while( (!fp.eof()) && (fp.get(c)) && (c != '\n') );
	}
	if(fp.eof() && line.empty())
		return 0;
	else 
		return 1;
}


// param: contador de endereços, numero (caso seja A+1, num = 1) 
// return: endereco do operando
// funcao: coloca operando na tabela de uso e retorna o "endereco" dele no programa
int is_tabela_uso(int cont_end, int num, string aux){
	int oper;
	int end = cont_end;
	list<int> num_list;
	num_list.clear();
	
	end++;
	num_list = tab_uso[aux];
	num_list.push_back(end);
	tab_uso[aux] = num_list;
	oper = tab_simb[aux].end + num;
	return oper;
}



/*param: uma linha do programa, um ponteiro para o contador de enderecos, um ponteiro para o simbolo, o conjunto de instrucoes e de diretivas
  return: -1, caso a linha passada nao tenha simbolo. Ou retorna o endereco do simbolo (caso a linha passada tenha um simbolo)
  funcao: recebe uma linha (do programa), pega a primeira palavra da linha e ve se eh um simbolo, caso seja um simbolo, pegamos a proxima
    */
string primeira_passagem(string line, int cont_linha, int &cont_end, tipo_simbolo &simbolo_struct){
	char line_aux[256], *aux;
	int tam, space, num_const;
	string simbolo;
	list<int> num_list;
	tipo_erro erro;
	
	num_list.clear();
	
	simbolo_struct.end = -1;
	simbolo_struct.tam = 1;
	simbolo_struct.is_const = false;
	simbolo_struct.is_rotulo = false;
	simbolo_struct.valido = true;
	simbolo_struct.vlr_const = 1;
	
	strcpy (line_aux, line.c_str());
	aux = strtok(line_aux," \t"); // Pegamos o primeiro token
	
	simbolo.clear();
	// verificamos se a linha eh vazia
	if(aux != NULL){
		tam = strlen(aux);
		tam--;
		
		// verificamos se o token que pegamos eh um simbolo
		if(aux[tam] == ':'){ 
			aux[tam] = '\0';
			simbolo = aux;
			aux = strtok(NULL," \t"); // caso tenha simbolo, pegamos o prox comando (pra atualizarmos o end. da prox linha)
			tam = strlen(aux);
			tam--;
			while(aux[tam] == ':'){
				aux[tam] = '\0';
				erro.msg = "Erro sintatico. Simbolo ";
				erro.msg += aux;
				erro.msg += " esta declarado de forma invalida";
				erro.linha = cont_linha;
				erro_list.push_back(erro);
				
				if (tab_simb.find(aux) == tab_simb.end() ){
					tab_simb[aux] = simbolo_struct;
					tab_simb[aux].valido = false;
				}
				else{
					tab_simb[aux].valido = false;
					if(tab_simb[aux].is_rotulo == true)
						push_erro("Erro semantico. Rotulo repetido", cont_linha);
					else
						push_erro("Erro semantico. Declaracao repetida", cont_linha);
				}
				
				aux = strtok(NULL," \t");
			}
		}
		
		// se simbolo eh vazio, apenas atualizamos o contador de enderecos
		if(simbolo.empty()){
			if(inst.find(aux) != inst.end())
				cont_end += inst[aux].tam;
			else if(dir.find(aux) != dir.end()){
				cont_end += dir[aux].tam;
				if(strcmp(aux, "PUBLIC") == 0){
					aux = strtok(NULL," \t");
					if(aux != NULL){
						tab_def[aux] = -1;
					}
					else{
						push_erro("Erro sintatico. Esperava um operando, recebeu nada", cont_linha);
					}
				}
			}
			else{
				if(is_valid(aux))
					push_erro("Erro sintatico. Operacao invalida", cont_linha); 
				else
					push_erro("Erro lexico. Operacao invalida", cont_linha); 
			}
			return simbolo;
		}	
		
		// se existe simbolo, atualizamos o contador de enderecos e retornamos o endereco do simbolo
		else{
			if(!is_valid(simbolo)){
				simbolo_struct.valido = false;
				push_erro("Erro lexico. Nome de simbolo invalido", cont_linha);
			}
			simbolo_struct.end = cont_end;
			if(inst.find(aux) != inst.end()){
				simbolo_struct.is_rotulo = true;
				cont_end += inst[aux].tam;
			}
			else if(dir.find(aux) != dir.end()){
				cont_end += dir[aux].tam;
				// Podemos ter um valor depois de space, indicando o num de enderecos que sera alocado
				if(strcmp(aux, "SPACE") == 0){
					space = 1;
					aux = strtok(NULL," \t");
					if(aux != NULL){
						if(!stringToInt(aux, space) || (space < 1)){
							space = 1;
							push_erro("Erro sintatico. Esperava um numero maior que zero", cont_linha);
						}
					}
					simbolo_struct.tam = space;
					space--;
					cont_end += space;
					
				}
				else if(strcmp(aux, "CONST") == 0){
					simbolo_struct.is_const = true;
					aux = strtok(NULL," \t");
					if(aux != NULL){
						if(!converterNum(aux, num_const)){
							/*ERRO, NUMERO INVALIDO*/
							num_const = 1;  
							push_erro("Erro semantico. Esperava um numero inteiro decimal ou hexadecimal", cont_linha);
						}
						simbolo_struct.vlr_const = num_const;
					}
					else{
						/*ERRO, ESPERAVA NUMERO, RECEBEU NADA*/
						push_erro("Erro sintatico. Esperava um numero, recebeu nada", cont_linha);  
					}
				}
				else if(strcmp(aux, "EXTERN") == 0){
					tab_uso[simbolo] = num_list;
					simbolo_struct.is_rotulo = true;
				}
				else if(strcmp(aux, "BEGIN") == 0)
					simbolo_struct.is_rotulo = true;
			}
			else{
				if(is_valid(aux))
					push_erro("Erro sintatico. Operacao invalida", cont_linha); 
				else
					push_erro("Erro lexico. Operacao invalida", cont_linha); 
			}
		}
	}
	return simbolo;
}


string segunda_passagem(string line, int cont_linha, int &cont_end, int &section, int &begin, int &stop, string &relativo) {
	char line_aux[256], *aux;
	string line_file, simbolo;
	int tam, i, inst_tam, space, end, num;
	int inst_opcode, oper1, oper2, num_const, tem_virg, flag_rel;
	list<int> num_list;
	
	flag_rel = 0;
	tem_virg = 0;
	inst_opcode = -1;
	num = 0;
	oper1 = -1;
	oper2 = -1;
	simbolo.clear();
	line_file.clear();
	num_list.clear();
	
	strcpy (line_aux, line.c_str());
	aux = strtok(line_aux," \t"); // Pegamos o primeiro token
	
	// verificamos se a linha eh vazia
	if(aux != NULL){
		tam = strlen(aux);
		tam--;
		
		// verificamos se o token que pegamos eh um simbolo, se for, apenas ignoramos ele
		if(aux[tam] == ':'){
			aux[tam] = '\0';
			simbolo = aux;
			aux = strtok(NULL," \t"); // Pegamos o proximo token que TEM que ser uma instrucao ou diretiva
			tam = strlen(aux);
			tam--;
			while(aux[tam] == ':'){
				aux = strtok(NULL," \t");
			}
		}
		
		if(aux != NULL){
			if(begin == 2){
				push_erro("Erro semantico. Nao pode ter codigo depois do END", cont_linha); 
			}
			// CASO SEJA INSTRUCAO
			else if(inst.find(aux) != inst.end()){
				if(section == 0){
					if(begin == 0)
						begin = -1; // Nao pode ter mais BEGIN no codigo
						
					inst_opcode = inst[aux].opcode;
					inst_tam = inst[aux].tam;
					// COPY
					if(inst_opcode == 9){
						flag_rel = 2;
						aux = strtok(NULL, " \t");
						if(aux != NULL){
							tam = strlen(aux);
							tam--;
							
							// verificamos se o token que pegamos eh um simbolo, se for, apenas ignoramos ele
							if(aux[tam] == ','){
								aux[tam] = '\0';
								//aux = strtok(NULL," \t"); // Pegamos o proximo token que TEM que ser uma instrucao ou diretiva
								tem_virg++;
								
							}
							num += separar(aux);
							
							if (num == -1) {
								push_erro("Erro sintatico. Esperava a soma de um numero a variavel. Recebeu valor invalido", cont_linha); 
							}
							if(tab_uso.find(aux) != tab_uso.end() ){
								oper1 = is_tabela_uso(cont_end, num, aux);
								flag_rel=3;
							}
							else if (tab_simb.find(aux) != tab_simb.end() ){
								if(tab_simb[aux].is_rotulo) {
									push_erro("Erro semantico. Argumento eh um rotulo, esperava uma variavel", cont_linha); 
								}
								else if( (num != 0) && (num >= tab_simb[aux].tam) ){
									push_erro("Erro semantico. Acesso de memoria nao alocada", cont_linha); 
								}
								else{
									oper1 = tab_simb[aux].end + num;
								}
							}
							else{
								push_erro("Erro semantico. Operando nao declarado", cont_linha); 
							}
							// OPERANDO 2
							aux = strtok(NULL, " \t");
							if(aux != NULL){
								tam = strlen(aux);
								tam--;
								if(strcmp(aux, ",") == 0){
									tem_virg++;
									aux = strtok(NULL, " \t");
								}
								if(aux != NULL){
								
									// verificamos se o token que pegamos eh um simbolo, se for, apenas ignoramos ele
									if(aux[0] == ','){
										tira_primeiro(aux);
										//aux = strtok(NULL," \t"); // Pegamos o proximo token que TEM que ser uma instrucao ou diretiva
										tem_virg++;
									}
									num += separar(aux);
									
									if (num == -1) {
										push_erro("Erro sintatico. Esperava a soma de um numero a variavel. Recebeu valor invalido", cont_linha); 
									}
									if(tab_uso.find(aux) != tab_uso.end() ){
										end = cont_end;
										end++;
										oper2 = is_tabela_uso(end, num, aux);
										if(flag_rel==3)
											flag_rel=5;
										else flag_rel=4;
									}
									else if (tab_simb.find(aux) != tab_simb.end() ){
										if(tab_simb[aux].is_const) {
											push_erro("Erro semantico. Argumento eh uma constante, esperava uma variavel", cont_linha); 
										}
										else if(tab_simb[aux].is_rotulo) {
											push_erro("Erro semantico. Argumento eh um rotulo, esperava uma variavel", cont_linha); 
										}
										else if( (num != 0) && (num >= tab_simb[aux].tam) ){
											push_erro("Erro semantico. Acesso de memoria nao alocada", cont_linha); 
										}
										else{
											oper2 = tab_simb[aux].end + num;
										}
									}
									else{
										push_erro("Erro semantico. Operando nao declarado", cont_linha); 
									}
								}
								else{
									/*ERRO, SIMBOLO NAO DEFINIDO*/
									push_erro("Erro sintatico. Esperava dois operandos", cont_linha); 
								}
							}
							else{
								/*ERRO, SIMBOLO NAO DEFINIDO*/
								push_erro("Erro sintatico. Esperava dois operandos", cont_linha); 
							}
						}
						else{
							/*ERRO, SIMBOLO NAO DEFINIDO*/
							push_erro("Erro sintatico. Esperava dois operandos, recebeu nada", cont_linha); 
						}
						
						if(tem_virg != 1){
							push_erro("Erro sintatico. Operandos devem ser separados por uma virgula", cont_linha); 
						}

					}
					// ADD, SUB, MULT, DIV, LOAD, OUTPUT
					else if( (inst_opcode == 1) || (inst_opcode == 2) || (inst_opcode == 3) || (inst_opcode == 4) || (inst_opcode == 10) ||(inst_opcode == 13) ){
						aux = strtok(NULL," \t"); // Pegamos o proximo token que TEM que ser um simbolo/operando
						if(aux != NULL){
							/*TRATAR CASO SEJA ALGO DO TIPO A+1*/
							num += separar(aux);
							
							if (num == -1) {
								push_erro("Erro sintatico. Esperava a soma de um numero a variavel. Recebeu valor invalido", cont_linha); 
							}
							if(tab_uso.find(aux) != tab_uso.end() ){
								oper1 = is_tabela_uso(cont_end, num, aux);
								flag_rel=1;
							}
							else if (tab_simb.find(aux) != tab_simb.end() ){
								if(tab_simb[aux].is_rotulo) {
									push_erro("Erro semantico. Argumento eh um rotulo, esperava uma variavel", cont_linha); 
								}
								else if( (num != 0) && (num >= tab_simb[aux].tam) ){
									push_erro("Erro semantico. Acesso de memoria nao alocada", cont_linha); 
								}
								else{
									oper1 = tab_simb[aux].end + num;
								}
								// CHECANDO DIVISAO POR ZERO
								if(inst_opcode == 4){
									if( (tab_simb[aux].is_const) && (tab_simb[aux].vlr_const == 0) ){
										push_erro("Erro semantico. Divisao por zero", cont_linha); 
									}
								}
							}
							else{
								push_erro("Erro semantico. Operando nao declarado", cont_linha); 
							}
						}
						else{
							push_erro("Erro sintatico. Esperava um operando, recebeu nada", cont_linha); 
						}
					}
					// JMP, JMPN, JMPP, JMPZ
					else if( (inst_opcode == 5) || (inst_opcode == 6) || (inst_opcode == 7) || (inst_opcode == 8) ){
						aux = strtok(NULL," \t"); // Pegamos o proximo token que TEM que ser um simbolo
						if(aux != NULL){
							/*TRATAR CASO SEJA ALGO DO TIPO A+1*/
							num += separar(aux);
							
							if (num == -1) {
								push_erro("Erro sintatico. Esperava a soma de um numero a variavel. Recebeu valor invalido", cont_linha); 
							}
							if(tab_uso.find(aux) != tab_uso.end() ){
								oper1 = is_tabela_uso(cont_end, num, aux);
								flag_rel=1;
							}
							else if (tab_simb.find(aux) != tab_simb.end() ){
								if(tab_simb[aux].is_const) {
									push_erro("Erro semantico. Argumento eh uma constante, esperava um rotulo", cont_linha); 
								}
								else if(!tab_simb[aux].valido){
									push_erro("Erro semantico. Pulo para rotulo invalido", cont_linha);
								}
								else if(!tab_simb[aux].is_rotulo){
									push_erro("Erro semantico. Argumento eh uma variavel, esperava um rotulo", cont_linha); 
								}
								else if( (num != 0) && (num >= tab_simb[aux].tam) ){
									push_erro("Erro semantico. Acesso de memoria nao alocada", cont_linha); 
								}
								else{
									oper1 = tab_simb[aux].end + num;
								}
							}
							else{
								push_erro("Erro semantico. Operando nao declarado", cont_linha); 
							}
						}
						else{
							push_erro("Erro sintatico. Esperava um operando, recebeu nada", cont_linha); 
						}
					}
					// INPUT, STORE (nao pode modificar CONST)
					else if( (inst_opcode == 11) || (inst_opcode == 12) ){
						aux = strtok(NULL," \t"); // Pegamos o proximo token que TEM que ser um simbolo/operando
						if(aux != NULL){
							/*TRATAR CASO SEJA ALGO DO TIPO A+1*/
							num += separar(aux);
							
							if (num == -1) {
								push_erro("Erro sintatico. Esperava a soma de um numero a variavel. Recebeu valor invalido", cont_linha); 
							}
							if(tab_uso.find(aux) != tab_uso.end() ){
								oper1 = is_tabela_uso(cont_end, num, aux);
								flag_rel=1;
							}
							else if (tab_simb.find(aux) != tab_simb.end() ){
								if(tab_simb[aux].is_const) {
									push_erro("Erro semantico. Variavel declarada como CONST nao pode ter seu valor alterado", cont_linha); 
								}
								else if(tab_simb[aux].is_rotulo) {
									push_erro("Erro semantico. Rotulos nao podem ser alterados", cont_linha); 
								}
								else if( (num != 0) && (num >= tab_simb[aux].tam) ){
									push_erro("Erro semantico. Acesso de memoria nao alocada", cont_linha); 
								}
								else{
									oper1 = tab_simb[aux].end + num;
								}
							}
							else{
								push_erro("Erro semantico. Operando nao declarado", cont_linha); 
							}
						}
						else{
							push_erro("Erro sintatico. Esperava um operando, recebeu nada", cont_linha); 
						}
					}
					//STOP
					else{
						stop = 1;
						flag_rel = -1;
					}
						
					// Vemos se tem mais algum token na linha
					aux = strtok(NULL," \t\n\0");
					if(aux != NULL){
						push_erro("Erro sintatico. Mais operandos que o necessario", cont_linha); 
					}
					else{
						line_file = to_string(inst_opcode);
						if(oper1 != -1){
							line_file.append(" ");
							line_file.append(to_string(oper1) );
						}
						if(oper2 != -1){
							line_file.append(" ");
							line_file.append(to_string(oper2) );
						}
						line_file.append(" ");
						
						/* MAPA DE BITS QUE SERA COLOCADO NO ARQUIVO, CASO PRECISE*/
						if(flag_rel == -1)
							relativo.append("0");
						else if(flag_rel == 0)
							relativo.append("01");
						else if(flag_rel == 1)
							relativo.append("00");
						else if(flag_rel == 2)
							relativo.append("011");
						else if(flag_rel == 3)
							relativo.append("001");
						else if(flag_rel == 4)
							relativo.append("010");
						else
							relativo.append("000");
					}
					cont_end += inst_tam;
					
					return line_file;

				}
				else{
					push_erro("Erro semantico. Instrucao na secao errada", cont_linha); 
				}
			}
			// CASO SEJA DIRETIVA
			else if(dir.find(aux) != dir.end()){
				cont_end += dir[aux].tam;
				
				// caso aux == SECTION
				if(strcmp(aux, "SECTION") == 0){
					aux = strtok(NULL," \t");
					if(strcmp(aux, "TEXT") == 0){
						if(section == 1){
							push_erro("Erro semantico. SECTION TEXT nao pode ser definida depois de SECTION DATA", cont_linha); 
						}
						else if(section != -1){
							push_erro("Erro semantico. SECTION TEXT ja foi definida", cont_linha); 
						}
						section = 0;
					}
					else if(strcmp(aux, "DATA") == 0){
						if(section == -1){
							/*ERRO, SECTION DATA JA FOI DEFINIDA OU FOI DEFINIDA ANTES DE SECTION TEXT*/
							push_erro("Erro semantico. SECTION DATA nao pode ser definida antes de SECTION TEXT", cont_linha); 
						}
						else if(section != 0){
							/*ERRO, SECTION DATA JA FOI DEFINIDA OU FOI DEFINIDA ANTES DE SECTION TEXT*/
							push_erro("Erro semantico. SECTION DATA ja foi definida", cont_linha); 
						}
						section = 1;
					}
					else{
						push_erro("Erro sintatico. Argumento de SECTION desconhecido", cont_linha); 
						/*ERRO, SECAO INVALIDA*/
					}
				}
				else if(strcmp(aux, "SPACE") == 0){
					if(simbolo.empty()){
						push_erro("Erro sintatico. Diretiva SPACE so pode ser usada apos declaracao de uma variavel", cont_linha); 
					}
					if(section == 1){
						space = 1;
						aux = strtok(NULL," \t");
						if(aux != NULL){
							if(!stringToInt(aux, space) || (space < 1)){
								space = 1;
								/*ERRO, NUMERO INVALIDO*/
							}
						}
						for(i = 0; i < space; i++){
							relativo.append("0");
							line_file.append("0 ");
						}
						space--;
						cont_end += space;
					}
					else{
						push_erro("Erro semantico. Diretiva SPACE na secao errada", cont_linha); 
					}
					
				}
				else if(strcmp(aux, "CONST") == 0){
					if( (section == 0) || (section == -1) ){
						push_erro("Erro semantico. Diretiva CONST so pode ser usada no SECTION DATA", cont_linha);
					}
					if(simbolo.empty()){
						push_erro("Erro sintatico. Diretiva CONST so pode ser usada apos declaracao de uma variavel", cont_linha);
					}
					
					aux = strtok(NULL," \t");
					if(aux != NULL){
						if(!converterNum(aux, num_const)){
							/*ERRO, NUMERO INVALIDO*/
							num_const = 1;  
						}
						line_file.append(to_string(num_const) );
						line_file.append(" ");
						relativo.append("0");
					}
					
				}
				else if(strcmp(aux, "PUBLIC") == 0){
					if(section == 0){
						if( (begin == 0) || (begin == -1) || (cont_end != 0)){
							push_erro("Erro semantico. Diretiva PUBLIC deve ser usada logo apos o BEGIN", cont_linha); 
						}
					}
					else{
						push_erro("Erro semantico. Diretiva PUBLIC na secao errada", cont_linha);  
					}
					aux = strtok(NULL," \t\n\0");
				}
				else if(strcmp(aux, "EXTERN") == 0){
					if(simbolo.empty()){
						push_erro("Erro sintatico. EXTERN tem que ter um nome", cont_linha); 
					}
					if(section == 0){
						if( (begin == 0) || (begin == -1) || (cont_end != 0)){
							push_erro("Erro semantico. Diretiva EXTERN deve ser usada logo apos o BEGIN", cont_linha); 
						}
					}
					else{
						push_erro("Erro semantico. Diretiva EXTERN na secao errada", cont_linha); 
					}
				}
				else if(strcmp(aux, "BEGIN") == 0){
					if(simbolo.empty()){
						push_erro("Erro sintatico. BEGIN tem que ter um nome", cont_linha); 
					}
					if(section != 0){
						push_erro("Erro semantico. Diretiva BEGIN deve ser usada somente dentro de SECTION TEXT", cont_linha); 
					}
					else if(begin == -1){
						push_erro("Erro semantico. Diretiva BEGIN deve ser usada logo apos SECTION TEXT", cont_linha); 
					}
					else if(begin == 1){
						push_erro("Erro semantico. Programa deve ter somente um BEGIN", cont_linha); 
					}
					else if(begin == 0)
						begin = 1;
				}
				else if(strcmp(aux, "END") == 0){
					if(section == 1){
						if(begin != 1){
							push_erro("Erro semantico. Diretiva END usada sem um BEGIN", cont_linha); 
						}
						else
							begin = 2;
					}
					else{
						push_erro("Erro semantico. Diretiva END deve ser usada somente dentro de SECTION DATA", cont_linha); 
					}

					
				}
				aux = strtok(NULL," \t\n\0");
				if(aux != NULL){
					push_erro("Erro sintatico. Mais operandos que o necessario", cont_linha); 
				}
			}
			//Else "Erro ja tratado"
		}

	}
	return line_file;
}




int main(int argc, char *argv[]){
	/*Declaracao de variaveis*/
	ifstream fp;
	ofstream fp_obj;
	string nome_prog, nome_obj, line, simbolo, line_file, code,relativo;
	int cont_end, cont_linha, section, begin, flag, stop;
	list<int> num_list;
	tipo_simbolo simbolo_struct;
	tipo_erro erro;
	
	/*inicializacao de variaveis*/
	cont_end = 0;
	cont_linha = 1;
	mapear_instrucao(inst);
	mapear_diretiva(dir);
	
	if(argc < 3){
		cout << "passe 2 argumentos\n";
	}
	else{
		nome_prog.assign(argv[1]);
		nome_obj.assign(argv[2]);
		nome_prog.append(".asm");
		
		fp.open(nome_prog.data());
		if(fp.is_open()){
			//passagem 1
			while ( getline2(fp,line) ){
				transform(line.begin(), line.end(), line.begin(), ::toupper); //deixar toda a linha em CAPS LOCK
				//cout << cont_linha << " -> " << line << endl;
				simbolo = primeira_passagem(line, cont_linha, cont_end, simbolo_struct);
				//colocar na tabela
				if(!simbolo.empty() ){
					if (tab_simb.find(simbolo) == tab_simb.end() ){
						tab_simb[simbolo] = simbolo_struct;
					}
					else{
						if(tab_simb[simbolo].is_rotulo == true)
							push_erro("Erro semantico. Rotulo repetido", cont_linha);
						else
							push_erro("Erro semantico. Declaracao repetida", cont_linha);
					}
				}

				cont_linha++;
			}
			
			//Atualizamos o endereco na tabela de definicao (procurando o simbolo na tab de simbolos)
			typedef map<string, int>::iterator it_type;
			for(it_type iterator = tab_def.begin(); iterator != tab_def.end(); iterator++) {
				simbolo = iterator->first;
				
				if(tab_simb.find(simbolo) != tab_simb.end() )
					tab_def[simbolo] = tab_simb[simbolo].end;
				else{
					push_erro("Erro semantico. Operando nao declarado", cont_linha); 
				}
			    //cout << iterator->first << " -> " << iterator->second << endl;
			}
			
			fp.clear();
			fp.seekg (0, fp.beg);
			
			//passagem 2
			/*section = -1: nenhuma SECTION foi definida
			  section =  0: estamos na SECTION TEXT
			  section =  1: estamos na SECTION DATA
			*/
			/*begin =  0: Nao existe nenhum BEGIN no programa, mas ainda pode ter
			  begin = -1: Nao existe nenhum BEGIN no programa e nao pode mais ter
			  begin =  1: Existe um BEGIN aberto no programa
			  begin =  2: BEGIN ja foi fechado com um END
			*/
			nome_obj.append(".o");
			code.clear(); //string contendo o codigo
			relativo.clear(); //string contendo o mapa de bits
			section = -1;
			begin = 0;
			stop = 0; // variavel para saber se o programa tem um stop
			cont_end = 0;
			cont_linha = 1;
			while ( getline2(fp,line) ){
				transform(line.begin(), line.end(), line.begin(), ::toupper); //deixar toda a linha em CAPS LOCK
				line_file = segunda_passagem(line, cont_linha, cont_end, section, begin, stop, relativo);
				code.append(line_file);
				cont_linha++;
			}
			fp.close();
			if(begin == 1){
				push_erro("Erro semantico. BEGIN nao foi fechado com um END", cont_linha); 
			}
			if( (begin != 2) && (stop != 1) )
				push_erro("Erro semantico. Programa tem que ser terminado por um STOP ou um END", cont_linha); 
			

			//Escritura no arquivo de saida			
			flag = 0;
			if(!erro_list.empty()){
				for(list<tipo_erro>::iterator list_iter = erro_list.begin();   list_iter != erro_list.end(); list_iter++){
					cout << "linha " << list_iter->linha << " -> " << list_iter->msg << endl;
				}
			}
			else{
				fp_obj.open(nome_obj.data());
				if(!tab_uso.empty()){
					flag = 1;
					fp_obj << "TABLE USE\n";
					map<string, list<int> >::iterator it1;
					list<int>::iterator it2;
					for(it1 = tab_uso.begin(); it1 != tab_uso.end(); it1++) {
						num_list = it1->second;
						for(it2 = num_list.begin(); it2 != num_list.end(); it2++)
							fp_obj << it1->first << " " << *it2 << endl;
					}
					fp_obj << endl;
				}
				if(!tab_def.empty()){
					flag = 1;
					fp_obj << "TABLE DEFINITION\n";
					for(it_type iterator = tab_def.begin(); iterator != tab_def.end(); iterator++) {
						simbolo = iterator->first;
						fp_obj << iterator->first << " " << iterator->second << endl;
					}
					fp_obj << endl;
				}
				if(flag == 1)
					fp_obj << "CODE\n";
				
				fp_obj << code << endl;
				if(flag == 1){
					fp_obj << endl << "R\n";
					fp_obj << relativo;
				}
				fp_obj.close();
			}
			
		}
		else{
			cout << "Arquivo a ser montado nao existe" << endl;
		}

		
	}
	
	return 0;
}