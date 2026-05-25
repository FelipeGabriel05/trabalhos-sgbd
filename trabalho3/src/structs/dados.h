#ifndef DADOS_H
#define DADOS_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

// Quantidades de linhas da tabela que serão armazenados na página
class Tupla {
    public:
        vector<string> colunas;
};

// Conjunto de tuplas 
class Pagina {
    public:
        vector<Tupla> tuplas;
        int qtd_tuplas_ocup = 0;

        bool cheia() {
            if(qtd_tuplas_ocup == 12) {
                return true;
            } else {
                return false;
            }
        }

        bool inserirTupla(Tupla t) {
            if(cheia()) {
                return false;
            }

            tuplas.push_back(t);
            qtd_tuplas_ocup++;
            return true;
        }
};

class Esquema {
    public:
        int qtd_cols = 0;

        // Permite acessar colunas pelo nome, ou seja, mapeia
        // o nome da coluna para o índice dela
        // indice       nome da coluna
        // 0            chave_primaria
        // nome_para_indice["chave_primaria"] = 0;
        unordered_map<string, int> nome_para_indice;
};


// Tabela completa
class Tabela {
    public:
        vector<Pagina> paginas;
        int qtd_paginas;
        Esquema esquema;
};

#endif