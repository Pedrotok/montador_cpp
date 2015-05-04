#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<map>
//#include<cstdio>
//#include <unistd.h>

using namespace std;

struct tipo_inst {
  	int operando, opcode;
  	int tam = operando++;
} ;

void mapear(map<string, tipo_inst> &inst){
	inst["ADD"].opcode = 1;
	inst["ADD"].operando = 2;

	inst["SUB"].opcode = 2;
	inst["SUB"].operando = 2;

	inst["MULT"].opcode = 3;
	inst["MULT"].operando = 2;

	inst["DIV"].opcode = 4;
	inst["DIV"].operando = 2;

	inst["JMP"].opcode = 5;
	inst["JMP"].operando = 2;

	inst["JMPN"].opcode = 6;
	inst["JMPN"].operando = 2;

	inst["JMPP"].opcode = 7;
	inst["JMPP"].operando = 2;

	inst["JMPZ"].opcode = 8;
	inst["JMPZ"].operando = 2;

	inst["COPY"].opcode = 9;
	inst["COPY"].operando = 3;

	inst["LOAD"].opcode = 10;
	inst["LOAD"].operando = 2;

	inst["STORE"].opcode = 11;
	inst["STORE"].operando = 2;

	inst["INPUT"].opcode = 12;
	inst["INPUT"].operando = 2;

	inst["OUTPUT"].opcode = 13;
	inst["OUTPUT"].operando = 2;

	inst["STOP"].opcode = 14;
	inst["STOP"].operando = 1;
}

int main(){
	map <string, tipo_inst> inst;
	mapear(inst);



	
	/*if( (inst.find("SUBA") != inst.end()) )
		cout << inst.size() << endl;
	else
		cout << "NAO DEU\n";*/
	cout << inst["LOAD"].tam;
	
	return 0;
}
