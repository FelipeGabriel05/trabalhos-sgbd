#include "read.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

static void removerCR(string &s) {
    if(!s.empty() && s.back() == '\r') {
        s.pop_back();
    }
}

// Função que carrega a tabela a partir de um CSV com colunas separadas por TAB
Tabela carregarCSV(string caminho) {
    Tabela tabela;

    ifstream arquivo(caminho);
    string linha;

    if(!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo\n";
        return tabela;
    }

    // Pula o cabeçalho e lê os nomes das colunas para o esquema
    getline(arquivo, linha);
    removerCR(linha);

    int indice = 0;
    string coluna;
    stringstream header(linha);

    while(getline(header, coluna, '\t')) {
        removerCR(coluna);
        tabela.esquema.nome_para_indice[coluna] = indice;
        indice++;
    }

    tabela.esquema.qtd_cols = indice;
    Pagina paginaAtual;

    while(getline(arquivo, linha)) {
        if(linha.empty()) continue;

        removerCR(linha);

        stringstream ss(linha);
        string valor;
        Tupla t;

        while(getline(ss, valor, '\t')) {
            // remove \r de cada campo individualmente
            removerCR(valor);
            t.colunas.push_back(valor);
        }

        // só insere se a página não estiver cheia
        if(!paginaAtual.inserirTupla(t)) {
            tabela.paginas.push_back(paginaAtual);
            paginaAtual = Pagina();
            paginaAtual.inserirTupla(t);
        }
    }

    // salva página parcial restante
    if(paginaAtual.qtd_tuplas_ocup > 0) {
        tabela.paginas.push_back(paginaAtual);
    }

    tabela.qtd_paginas = tabela.paginas.size();
    arquivo.close();
    return tabela;
} 
