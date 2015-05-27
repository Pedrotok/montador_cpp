# montador_cpp
montador da linguagem hipotética da matéria de Software Básico da UnB

Integrantes do grupo: 
---------------------

* Lucas Vanderlei Fernandes - 11001597 
* Pedro Henrique Ferreira - 11/0018907

Instruções de compilação
------------------------
- Para compilar o montador, apenas de um make. Lembrando que usamos C++11 na implementação do montador;

- Para compilar o ligador, faça "g++ ligador.cpp -o ligador".

Instruções de execução
----------------------

1. Para executar o montador, é necessario passar dois argumentos onde:
	- argumento1: arquivo .asm que deseja compilar (sem a extensão .asm)
	- argumento2: nome do arquivo objeto de saida (sem a extensão .o)

Exemplo de uso: 

`./mont -o arq_entrada arq_saida`

2. Para executar o ligador, é necessário a passagem de três argumentos, onde:
	- argumento1 e argumento2: arquivos objetos .o que serão ligados (sem a extensão .o)
	- argumento2: nome do arquivo de saida (sem a extensão .e)

Exemplo de uso: 

`./ligador -o arq_obj1 arq_obj2 saida`

Descrição do trabalho
---------------------

O código do montador está praticamente todo no montador.cpp, com uma pequena parte no init.cpp. No init.cpp, apenas temos as funções que inicializam as structs de diretiva e instrução;

Código do ligador está todo no ligador.cpp.
