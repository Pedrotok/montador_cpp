#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
//#include<cstdio>
//#include <unistd.h>
#include<map>
#include"header/init.hpp"

using namespace std;


// VARIAVEIS GLOBAIS 
map <string, tipo_inst> inst;
map <string, tipo_dir> dir;


// param: ponteiro de um arquivo e uma string
// return: 0, caso seja o fim do arquivo. 1, caso contrario
// funcao: pega toda a linha ate encontrar um ';' ou um '\n'
int getline2(ifstream &fp, string &line){
	char c;

	line.clear();
	while( (!fp.eof()) && (fp.get(c)) && (c != '\n') && (c != ';') ){
		line.push_back(c);
	}
	if(c == ';'){
		while( (!fp.eof()) && (fp.get(c)) && (c != '\n') );
	}
	if(fp.eof() && line.empty())
		return 0;
	else 
		return 1;
}


/*param: uma linha do programa, um ponteiro para o contador de enderecos, um ponteiro para o rotulo, o conjunto de instrucoes e de diretivas
  return: -1, caso a linha passada nao tenha rotulo. Ou retorna o endereco do rotulo (caso a linha passada tenha um rotulo)
  funcao: recebe uma linha (do programa), pega a primeira palavra da linha e ve se eh um rotulo, caso seja um rotulo, pegamos a proxima
    */
int get_rotulo(string line, int &cont_end, string &rotulo){
	char line_aux[256], *aux;
	int tam, end;
	
	strcpy (line_aux, line.c_str());
	aux = strtok(line_aux," \t"); // Pegamos o primeiro token
	
	// verificamos se a linha eh vazia
	if(aux != NULL){	
		rotulo.clear();

		tam = strlen(aux);
		tam--;
		
		// verificamos se o token que pegamos eh um rotulo
		if(aux[tam] == ':'){ 
			aux[tam] = '\0';
			rotulo = aux;
			aux = strtok(NULL," \t"); // caso tenha rotulo, pegamos o prox comando (pra atualizarmos o end. da prox linha)
		}
		
		// se rotulo eh vazio, apenas atualizamos o contador de enderecos
		if(rotulo.empty()){
			if(inst.find(aux) != inst.end())
				cont_end += inst[aux].tam;
			else if(dir.find(aux) != dir.end())
				cont_end += dir[aux].tam;
			else
				cout << "INSTRUCAO INVALIDA\n"; // MUDAR ESSA LINHA PLS
			return -1;
		}	
		
		// se existe rotulo, atualizamos o contador de enderecos e retornamos o endereco do rotulo
		else{
			/*FALTA VERIFICAR SE ROTULO EH VALIDO*/
			end = cont_end;
			if(inst.find(aux) != inst.end()){
				cont_end += inst[aux].tam;
				return end;
			}
			else if(dir.find(aux) != dir.end()){
				cont_end += dir[aux].tam;
				return end;
			}
			else{
				//cout << rotulo << ": " << cont_end << endl;
				cout << "DIRETIVA INVALIDA\n"; // MUDAR ESSA LINHA PLS
				return -1;
			}
		}
	}
	return -1;
}


void segunda_passagem(string line, int &cont_end, map <string, int> tab_r, int &section) {
	char line_aux[256], *aux;
	int tam, i, inst_tam, inst_opcode, rot;
	
	strcpy (line_aux, line.c_str());
	aux = strtok(line_aux," \t"); // Pegamos o primeiro token
	
	// verificamos se a linha eh vazia
	if(aux != NULL){
		tam = strlen(aux);
		tam--;
		
		// verificamos se o token que pegamos eh um rotulo, se for, apenas ignoramos ele
		if(aux[tam] == ':'){ 
			aux = strtok(NULL," \t"); // Pegamos o proximo token que TEM que ser uma instrucao ou diretiva
		}
		
		// se rotulo eh vazio, apenas atualizamos o contador de enderecos
		if(aux != NULL){
			if(inst.find(aux) != inst.end()){
				inst_opcode = inst[aux].opcode;
				// Vamos tratar a instrucao COPY separadamente das outras, pq ela tem uma virgula entre os operandos
				inst_tam = inst[aux].tam;
				if(strcmp(aux, "COPY") == 0){
					i = 0;
					while(i < 2){
						aux = strtok(NULL, ", \t");
						if (tab_r.find(aux) == tab_r.end() ){
							/*ERRO, SIMBOLO NAO DEFINIDO*/
						}
						else{
							rot = tab_r[aux];
						}
						i++;
					}

				}
				else if(inst[aux].operando == 1){
					inst_opcode = inst[aux].opcode;
					aux = strtok(NULL," \t"); // Pegamos o proximo token que TEM que ser um rotulo
					if (tab_r.find(aux) == tab_r.end() ){
						/*ERRO, SIMBOLO NAO DEFINIDO*/
					}
					else{
						rot = tab_r[aux];
					}
				}
					
				// Vemos se tem mais algum token na linha
				//aux = strtok(NULL," \t\n");
				//if(aux != NULL){
					/*ERRO, COISAS A MAIS NA LINHA*/
				//}
				cout << cont_end << ": " << inst_opcode << " " << rot << " ";
				cont_end += inst_tam;

			}
			else if(dir.find(aux) != dir.end()){
				cont_end += dir[aux].tam;
				
				// caso aux == SECTION
				if(strcmp(aux, "SECTION") == 0){
					aux = strtok(NULL," \t");
					if(strcmp(aux, "TEXT") == 0){
						if(section == -1)
							section = 0;
						else{
							/*ERRO, SECTION TEXT JA FOI DEFINIDA*/
						}
					}
					else if(strcmp(aux, "DATA") == 0){
						if(section == 0)
							section = 1;
						else{
							/*ERRO, SECTION DATA JA FOI DEFINIDA OU FOI DEFINIDA ANTES DE SECTION TEXT*/
						}
					}
					else{
						/*ERRO, SECAO INVALIDA*/
					}
				}
				//else if
			
			}
			else
				cout << "OPERACAO INVALIDA\n"; /*ERRO, TOKEN INVALIDO*/
			
		}
	//return -1;
	}
	cout << "\n";
}





int main(int argc, char *argv[]){
	/*Declaracao de variaveis*/
	ifstream fp;
	string nome_prog, nome_obj, line, rotulo;
	int end, cont_end, cont_linha, section;
	map <string, int> tab_r;
	
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
		
		fp.open(nome_prog.data());
		if(fp.is_open()){
			//passagem 1
			while ( getline2(fp,line) ){
				cout << cont_end << " -> " << line << endl;
				end = get_rotulo(line, cont_end, rotulo);
				//colocar na tabela
				if(!rotulo.empty() )
					/*FALTA VERIFICAR SE ROTULO JA FOI DEFINIDO*/
					tab_r[rotulo] = end;

				//usleep(1000000);
				cont_linha++;
			}

			typedef map<string, int>::iterator it_type;
			for(it_type iterator = tab_r.begin(); iterator != tab_r.end(); iterator++) {
			    cout << iterator->first << " -> " << iterator->second << endl;
			}
			fp.clear();
			fp.seekg (0, fp.beg);
			
			//passagem 2
			/*section = -1: nenhuma SECTION foi definida
			  section =  0: estmaos na SECTION TEXT
			  section =  1: estamos na SECTION DATA
			*/
			section = -1;
			cont_end = 0;
			cont_linha = 1;
			while ( getline2(fp,line) ){
				cout << cont_linha << " ";
				segunda_passagem(line, cont_end,tab_r, section);
				cont_linha++;
			}
			
			fp.close();
			
		}
		else{
			cout << "LIXOO" << endl;
		}

		
	}
	
	return 0;
}