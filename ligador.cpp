/*
Estudantes: Lucas Vanderlei Fernandes & Pedro Henrique Ferreira
Matricula: 110015975 & 110018907
Software Básico 1/2015
Trabalho 1

Sobre: Para compilar o ligador, faça "g++ ligador.cpp -o ligador".
Para executar o ligador, faça "./ligador arq1 arq2 saida", onde arq1 e arq2 sao os arquivos de entrada arq1.o e arq2.o, e apos a ligacao dos dois
arquivos, o arq1 sera o arquivo de "base" e o arq2 sera o relativo. O nome do arquivo de saida eh saida.e
*/

#include<iostream>
#include <fstream>
#include <string>

using namespace std;

//Funcao que retorna quantos argumentos o Opcode vai ter
int opcodeCheck(int a){
    if(a>0&&a<15){
        if(a==9){
            return 2;
        }else if (a==14){
            return 0;
        }else
            return 1;
    }else{
        return -1;
    }
}



int main(int argc, char *argv[]){
	//Funcao que verifica se um numero se encontra em uma string
	string arq1, arq2, saida;
    ofstream ligado;
    string line, cod1, cod2, def1, def2, use1, use2, rel2;
    ifstream ent, ent2;

    int i, n, offset=0, cod1i[100],cod2i[100], rel2i[100], cod1n=0,cod2n=0;


	if(argc < 4){
		cout << "passe 3 argumentos\n";
	}
	else{
		arq1.assign(argv[1]);
		arq2.assign(argv[2]);
		saida.assign(argv[3]);
		arq1.append(".o");
		arq2.append(".o");
		saida.append(".e");
		
		ent.open(arq1.data());
		ent2.open(arq2.data());
		ligado.open(saida.data());
		
		if( (ent.is_open()) && (ent2.is_open()) ){
			//Escrevendo o Code do arquivo 1 em um vetor de INT e calculando o offset
			while ( getline (ent,line) )
			{
				if(line[6]=='U'&&line[7]=='S'&&line[8]=='E'){
					getline(ent,line);
					while(line[0]!='T'||line[0]=='C'){
						use1=use1+line;
						getline(ent,line);
						use1+=' ';
					}
				}
				if(line[0]=='T'&&line[6]=='D'&&line[7]=='E'&&line[8]=='F'){
					getline(ent,line);
					while(line[0]!='C'){
						def1=def1+line;
						getline(ent,line);
						def1+=' ';
					}
				}
				if(line[0]=='C'&&line[1]=='O'&&line[2]=='D'&&line[3]=='E'){
					getline (ent,cod1);
					break;
				}
			}
			
			//Salvar o arquivo 2 em String: Uso e Definicao. E deixar a leitura no inicio do Code
			while ( getline (ent2,line) )
			{
				if(line[6]=='U'&&line[7]=='S'&&line[8]=='E'){
					getline(ent2,line);
					while(line[0]!='T'||line[0]=='C'){
						use2=use2+line;
						getline(ent2,line);
						use2+=' ';
					}
				}
				if(line[0]=='T'&&line[6]=='D'&&line[7]=='E'&&line[8]=='F'){
					getline(ent2,line);
					while(line[0]!='C'){
						def2=def2+line;
						getline(ent2,line);
						def2+=' ';
					}
				}
				if(line[0]=='C'&&line[1]=='O'&&line[2]=='D'&&line[3]=='E'){
					getline (ent2,cod2);
				}
				if(line[0] == 'R'){
					getline(ent2, rel2);
					break;
				}
			}

		{
			//Escrevendo o Code do arquivo 1 em um vetor de INT e calculando o offset
			for(i=0;i<cod1.length();i++){
				n=0;
				for(;cod1[i]!=' '&&i<cod1.length();i++){
					n=n*10;
					n=n+('0'-cod1[i])*(-1);
				}
				cod1i[cod1n]=n;
				cod1n++;
				offset++;
			}

			//Escrevendo o Code do arquivo 2 em um vetor de INT
			for(i=0;i<cod2.length();i++){
				n=0;
				for(;cod2[i]!=' '&&i<cod2.length();i++){
					n=n*10;
					n=n+('0'-cod2[i])*(-1);
				}
				cod2i[cod2n]=n;
				cod2n++;
			}
			
			//Escrevendo o mapa de bits do arquivo 2 em um vetor de INTs
			for(i=0;i<rel2.length();i++){
				rel2i[i]=rel2[i] - '0';
			}
		}

			int j;
		// Usando o offset no vetor do Code do arquivo 2 para adptar os enderecos
			for(i=0;i<cod2n;i++){
				if(rel2i[i])	// quando bit for igual a 1, significa que o endereco eh relativo, logo somamos o offset
					cod2i[i] += offset;
			}

			string word="", word2;
			int  end, n2;
		// Usando tabela USE do arq1, para ligar o arq1 ao arq2
			for(i=0;i<use1.length();i++){
				word="";
				while(use1[i]!=' '&&i<use1.length()){
					word+=use1[i];
					i++;
				}
				i++;
				n=0;
				for(;use1[i]!=' '&&i<use1.length();i++){
					n=n*10;
					n=n+('0'-use1[i])*(-1);
				}


				for(j=0;j<def2.length();j++){
					word2="";
					while(def2[j]!=' '){
						word2+=def2[j];
						j++;
					}
					if(word2==word){
						n2=0;
						j++;
						for(;def2[j]!=' '&&j<def2.length();j++){
							n2=n2*10;
							n2=n2+('0'-def2[j])*(-1);
						}
						end=n2+offset;
					}
				}
				if(i<use1.length())
					cod1i[n]+=end;
			}


		//Usando tabela USE do arq2 para ligar arq2 ao arq1
			for(i=0;i<use2.length();i++){
				word="";
				while(use2[i]!=' '&&i<use2.length()){
					word+=use2[i];
					i++;
				}
				i++;
				n=0;
				for(;use2[i]!=' '&&i<use2.length();i++){
					n=n*10;
					n=n+('0'-use2[i])*(-1);
				}


				for(j=0;j<def1.length();j++){
					word2="";
					while(def1[j]!=' '){
						word2+=def1[j];
						j++;
					}
					if(word2==word){
						n2=0;
						j++;
						for(;def1[j]!=' '&&j<def1.length();j++){
							n2=n2*10;
							n2=n2+('0'-def1[j])*(-1);
						}
						end=n2;
					}
				}
				cod2i[n]+=end;
			}


		//Escrevendo no arquivo de saida o arquivo ligado
			for(i=0;i<cod1n;i++){
				ligado << cod1i[i] << ' ';//ligado
			}

			for(i=0;i<cod2n;i++){
				//cod2i[i]+=offset*stop;
				ligado << cod2i[i] << ' ';//ligado
			}
		//Fim da escrita do Code
			ligado.close();
			ent.close();
			ent2.close();
		}
		else{
			cout << "Pelo menos um arquivo a ser ligado nao ser existe" << endl;
		}
	}

    return 0;
}

