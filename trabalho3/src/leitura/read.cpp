#include "read.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// ifstream => Usado para abrir arquivos
// sstream => separa colunas da linha.

// função que carrega a tabela. Modificações feitas em relação aos últimos trabalhos.
Tabela carregarCSV(string caminho) {
    Tabela tabela;

    ifstream arquivo(caminho);
    string linha;

    if(!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo\n";
        return tabela;
    }

    // pula o cabeçalho. Não salva como tupla
    getline(arquivo, linha);

    int indice = 0;
    string coluna;
    stringstream header(linha);

    // lê as colunas do cabeçalho. No caso o nome dos campos da nossa tabela.
    while(getline(header, coluna, '\t')) {
        tabela.esquema.nome_para_indice[coluna] = indice;
        indice++;
    }

    // salva a quantidade de colunas dessa tabela.
    tabela.esquema.qtd_cols = indice;
    Pagina paginaAtual;

    while(getline(arquivo, linha)) {
        // se linha vazia pula para a próxima iteração
        if(linha.empty()) continue;
        stringstream ss(linha);
        string valor;
        Tupla t;

        // lê as colunas da linha
        while(getline(ss, valor, '\t')) {
            t.colunas.push_back(valor);
        }

        // só insere se a página não estiver cheia. Se ela estiver cria uma nova página.
        if(!paginaAtual.inserirTupla(t)) {
            tabela.paginas.push_back(paginaAtual);
            paginaAtual = Pagina();
            paginaAtual.inserirTupla(t);
        }
    }

    // se sobrou página parcialmente cheia. A condição vai salvar o que falta
    if(paginaAtual.qtd_tuplas_ocup > 0) {
        tabela.paginas.push_back(paginaAtual);
    }

    tabela.qtd_paginas = tabela.paginas.size();

    arquivo.close();
    return tabela;
}
