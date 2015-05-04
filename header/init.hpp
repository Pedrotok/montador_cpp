#ifndef INIT_H
	#define INIT_H

#include<iostream>
#include<string>
#include<map>

//struct tipo_inst ;

//struct tipo_dir ;

struct tipo_inst {
  	int operando, opcode, tam;
} ;

struct tipo_dir {
  	int operando, tam;
} ;

void mapear_instrucao(std::map<std::string, tipo_inst> &inst);

void mapear_diretiva(std::map<std::string, tipo_dir> &dir);

#endif