#include"header/init.hpp"

using namespace std;



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