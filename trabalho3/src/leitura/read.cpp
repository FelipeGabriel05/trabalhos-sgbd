#include "read.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// função que carrega a duas tabelas. Modificações feitas em relação aos últimos trabalhos.
Tabela carregarCSV(string caminho) {
    Tabela tabela;

    ifstream arquivo(caminho);
    string linha;

    if(!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo\n";
        return tabela;
    }

    // pula o cabeçalho
    getline(arquivo, linha);

    int indice = 0;
    string coluna;
    stringstream header(linha);

    while(getline(header, coluna, '\t')) {
        tabela.esquema.nome_para_indice[coluna] = indice;
        indice++;
    }

    tabela.esquema.qtd_cols = indice;
    Pagina paginaAtual;

    while(getline(arquivo, linha)) {
        if(linha.empty());
        stringstream ss(linha);
        string valor;
        Tupla t;

        while(getline(ss, valor, '\t')) {
            t.colunas.push_back(valor);
        }

        if(!paginaAtual.inserirTupla(t)) {
            tabela.paginas.push_back(paginaAtual);
            paginaAtual = Pagina();
            paginaAtual.inserirTupla(t);
        }
    }

    if(paginaAtual.qtd_tuplas_ocup > 0) {
        tabela.paginas.push_back(paginaAtual);
    }

    tabela.qtd_paginas = tabela.paginas.size();

    arquivo.close();
    return tabela;
}
