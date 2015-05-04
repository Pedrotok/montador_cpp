#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
//#include<cstdio>
//#include <unistd.h>
#include<map>

using namespace std;

struct tipo_inst {
  	int operando, opcode, tam;
} ;

struct tipo_dir {
  	int operando, tam;
} ;


//param: objeto do tipo map (map[string] = tipo_inst)
//return: nada
//funcao: associa cada instrucao com um opcode e um num de operandos
void mapear_instrucao(map<string, tipo_inst> &inst){
	inst["ADD"].opcode = 1;
	inst["ADD"].operando = 1;
	inst["ADD"].tam = 2;

	inst["SUB"].opcode = 2;
	inst["SUB"].operando = 1;
	inst["SUB"].tam = 2;

	inst["MULT"].opcode = 3;
	inst["MULT"].operando = 1;
	inst["MULT"].tam = 2;

	inst["DIV"].opcode = 4;
	inst["DIV"].operando = 1;
	inst["DIV"].tam = 2;

	inst["JMP"].opcode = 5;
	inst["JMP"].operando = 1;
	inst["JMP"].tam = 2;

	inst["JMPN"].opcode = 6;
	inst["JMPN"].operando = 1;
	inst["JMPN"].tam = 2;

	inst["JMPP"].opcode = 7;
	inst["JMPP"].operando = 1;
	inst["JMPP"].tam = 2;

	inst["JMPZ"].opcode = 8;
	inst["JMPZ"].operando = 1;
	inst["JMPZ"].tam = 2;

	inst["COPY"].opcode = 9;
	inst["COPY"].operando = 2;
	inst["COPY"].tam = 3;

	inst["LOAD"].opcode = 10;
	inst["LOAD"].operando = 1;
	inst["LOAD"].tam = 2;

	inst["STORE"].opcode = 11;
	inst["STORE"].operando = 1;
	inst["STORE"].tam = 2;

	inst["INPUT"].opcode = 12;
	inst["INPUT"].operando = 1;
	inst["INPUT"].tam = 2;

	inst["OUTPUT"].opcode = 13;
	inst["OUTPUT"].operando = 1;
	inst["OUTPUT"].tam = 2;

	inst["STOP"].opcode = 14;
	inst["STOP"].operando = 0;
	inst["STOP"].tam = 1;
}

void mapear_diretiva(map<string, tipo_dir> &dir){
	dir["SECTION"].operando = 1;
	dir["SECTION"].tam = 0;

	dir["SPACE"].operando = 1;
	dir["SPACE"].tam = 1;

	dir["CONST"].operando = 1;
	dir["CONST"].tam = 1;

	dir["PUBLIC"].operando = 0;
	dir["PUBLIC"].tam = 0;

	dir["EXTERN"].operando = 0;
	dir["EXTERN"].tam = 0;

	dir["BEGIN"].operando = 0;
	dir["BEGIN"].tam = 0;

	dir["END"].operando = 0;
	dir["END"].tam = 0;
}


// param: ponteiro de um arquivo e uma string
// return: 0, caso seja o fim do arquivo. 1, caso contrario
// funcao: pega toda a linha ate encontrar um ';' ou um '\n'
int getline2(ifstream& fp, string& line){
	char c;
	int i;
	line = "";
	while( (!fp.eof()) && (fp.get(c)) && (c != '\n') && (c != ';') && c != EOF){
		line += c;
	}
	if(c == ';'){
		while( (!fp.eof()) && (fp.get(c)) && (c != '\n') );
	}
	if(fp.eof())
		return 0;
	else 
		return 1;
}


/*param: uma linha do programa, um ponteiro para o contador de enderecos, um ponteiro para o rotulo, o conjunto de instrucoes e de diretivas
  return: -1, caso a linha passada nao tenha rotulo. Ou retorna o endereco do rotulo (caso a linha passada tenha um rotulo)
  funcao: recebe uma linha (do programa), pega a primeira palavra da linha e ve se eh um rotulo, caso seja um rotulo, pegamos a proxima
    */
int get_rotulo(string line, int &cont_end, string &rotulo, map<string, tipo_inst> inst, map<string, tipo_dir> dir){
	char line_aux[256], *aux;
	int tam, i = 0, flag = 0, end;
	
	strcpy (line_aux, line.c_str());
	aux = strtok(line_aux," \t"); // Pegamos o primeiro token
	
	if(aux != NULL){	// verificamos se a linha eh vazia
		rotulo = "";

		tam = strlen(aux);
		tam--;
		if(aux[tam] == ':'){ // verificamos se o token que pegamos eh um rotulo
			aux[tam] = '\0';
			rotulo = aux;
			flag = 1;
			aux = strtok(NULL," \t");
		}
		if(rotulo == ""){
			if(inst.find(aux) != inst.end())
				cont_end += inst[aux].tam;
			else if(dir.find(aux) != dir.end())
				cont_end += dir[aux].tam;
			else
				cout << "INSTRUCAO INVALIDA\n"; // MUDAR ESSA LINHA PLS
			return -1;
		}	
		else{
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

}

int main(int argc, char *argv[]){
	/*Declaracao de variaveis*/
	ifstream fp;
	string nome_prog, nome_obj, line, rotulo;
	char line_aux[256], *aux;
	int end, cont_end, cont_linha;
	map <string, tipo_inst> inst;
	map <string, tipo_dir> dir;
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
				end = get_rotulo(line, cont_end, rotulo, inst, dir);
				//colocar na tabela
				if(rotulo != "")
					tab_r[rotulo] = end;

				//usleep(1000000);
				cont_linha++;
			}

			typedef map<std::string, int>::iterator it_type;
			for(it_type iterator = tab_r.begin(); iterator != tab_r.end(); iterator++) {
			    cout << iterator.first << " -> " << iterator.second << endl;
			    // Repeat if you also want to iterate through the second map.
			}
			
			
		}
		else{
			cout << "LIXOO" << endl;
		}

		
	}
	
	return 0;
}