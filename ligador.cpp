#include<iostream>
#include <fstream>
#include <string>

using namespace std;

//Para resolver:
//Arquivos devem ser lidos na execucao
//Falta escrever no arquivo
//Comentar melhor

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

int checkString(int a, string S){
    int i, n;
    for(i=0;i<S.length();i++){
        if(S[i]==' '){
            i++;
            n=0;
            for(;i<S.length()&&S[i]!=' ';i++){
                n=n*10;
                n=n+('0'-S[i])*(-1);
            }

            if(n==a){
                return 1;
            }
            i++;
        }

    }
    return 0;
}

int main(int argc, char *argv[]){
    ofstream ligado;
    string line, cod1, cod2, def1, def2, use1, use2;
//Arquivo para Teste
    ifstream ent;
    ent.open ("arq1");
    ifstream ent2;
    ent2.open ("arq2");
//Fim do teste

    int i, n, offset=0, stop, cod1i[100],cod2i[100], cod1n=0,cod2n=0;
    ligado.open ("saida");

//Pegar os Arquivos e salva-los em strings diferentes para Uso, Definicao e Code
{


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

    while ( getline (ent2,line) )
    {
        if(line[6]=='U'&&line[7]=='S'&&line[8]=='E'){
            getline(ent2,line);
            while(line[0]!='T'||line[0]=='C'){
                use2=use2+line;
                getline(ent2,line);
                use2+=' ';            }
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
            break;
        }
    }
}
//Fim da analise dos arquivos

//Escrevendo os Code em um arquivo de INT
{
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

    for(i=0;i<cod2.length();i++){
        n=0;
        for(;cod2[i]!=' '&&i<cod2.length();i++){
            n=n*10;
            n=n+('0'-cod2[i])*(-1);
        }
        cod2i[cod2n]=n;
        cod2n++;
    }
}

    int j;
// Usando o offset no segunda arquivo
    for(i=0;i<cod2n;i++){
        for(j=0;j<opcodeCheck(cod2i[i]);j++){
            if(checkString(i+j+1,use2)==0){
                cod2i[i+j+1]+=offset;
            }
        }
        i+=j;
        if(cod2i[i+1]==14){
            break;
        }
    }

    string word="", word2;
    int wordi, end, n2;
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
        cout << cod1i[i] << ' ';//ligado
    }

    stop=1;
    for(i=0;i<cod2n;i++){
        if(cod2i[i]==14)
            stop=0;
        //cod2i[i]+=offset*stop;
        cout << cod2i[i] << ' ';//ligado
    }
//Fim da escrita do Code
    ligado.close();
    ent.close();
    ent2.close();

    return 0;
}
