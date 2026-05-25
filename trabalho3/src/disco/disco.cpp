#include "disco.h"
#include <iostream>
#include <fstream>
using namespace std;

void DiscoSimulado::salvarPagina(Pagina p) {
    paginas.push_back(p);
    cout << "Pagina salva no disco. " << "Total de paginas: " << paginas.size() << endl;
}

Pagina DiscoSimulado::lerPagina(int indice) {
    cout << "Lendo pagina" << indice << endl;
    return paginas[indice];
}

int DiscoSimulado::qtdPagina() {
    return paginas.size();
}

void DiscoSimulado::mostrarEstadoDisco() {
    cout << "\n........DISCO...........\n";
    cout << "Quantidade de paginas: " << paginas.size() << endl;
    for(size_t i = 0; i < paginas.size(); i++) {
        cout << "Pagina " << i << " -> " << paginas[i].tuplas.size() << " tuplas" << endl;
    }
    cout << "...........................\n";
}

void DiscoSimulado::mostrarConteudoDisco(int indice_coluna) {
    cout << "\n................ CONTEUDO DISCO .................\n";
    for(size_t i = 0; i < paginas.size(); i++) {
        cout << "\n[PAGINA " << i << "]\n";
        for(Tupla t : paginas[i].tuplas) {
            cout << t.colunas[indice_coluna] << endl;
        }
    }
    cout << ".........................................................\n";
}

void DiscoSimulado::mostrarRuns(int indice_coluna) {
    cout << "\n............... RUNS NO DISCO ...............\n";
    for(size_t r = 0; r < runs.size(); r++) {
        cout << "\n------- RUN " << r + 1 << " --------\n";
        int inicio = runs[r].first;
        int fim = runs[r].second;
        for(int i = inicio; i <= fim; i++) {
            cout << "\n[PAGINA " << i << "]\n";
            for(Tupla t : paginas[i].tuplas) {
                cout << t.colunas[indice_coluna] << endl;
            }
        }
    }
    cout << "\n===================================\n";
}

// CORRIGIDO: exporta apenas as páginas da run final (runs[0])
// antes exportava TODAS as páginas do disco, incluindo runs intermediárias
void DiscoSimulado::exportarCSV(string caminho, int indice_coluna) {

    ofstream arquivo(caminho);

    if(!arquivo.is_open()) {
        cout << "Erro ao criar CSV\n";
        return;
    }

    // Garante que existe pelo menos uma run final
    if(runs.empty()) {
        cout << "Nenhuma run disponivel para exportar\n";
        return;
    }

    // Percorre APENAS as páginas da run final (única run que sobrou após o merge)
    // runs[0].first = índice da primeira página da run final no disco
    // runs[0].second = índice da última página da run final no disco
    int inicio = runs[0].first;
    int fim    = runs[0].second;

    for(int i = inicio; i <= fim; i++) {
        for(Tupla t : paginas[i].tuplas) {
            for(size_t j = 0; j < t.colunas.size(); j++) {
                arquivo << t.colunas[j];
                if(j < t.colunas.size() - 1) {
                    arquivo << "\t";
                }
            }
            arquivo << "\n";
        }
    }

    arquivo.close();
    cout << "CSV exportado: " << caminho << endl;
}

// CORRIGIDO: salva runs TXT usando qtd_tuplas_ocup em vez de tuplas.size()
void DiscoSimulado::salvarRunsTXT(string caminho, int indice_coluna) {

    ofstream arquivo(caminho);

    if(!arquivo.is_open()) {
        cout << "Erro ao criar TXT\n";
        return;
    }

    arquivo << "========== RUNS ==========\n";
    arquivo << "Total de runs: " << runs.size() << "\n";

    for(size_t r = 0; r < runs.size(); r++) {

        arquivo << "\nRUN " << r + 1 << "\n";

        int inicio = runs[r].first;
        int fim    = runs[r].second;

        arquivo << "Paginas: " << inicio << " ate " << fim << "\n";

        for(int i = inicio; i <= fim; i++) {

            arquivo << "\nPAGINA " << i << "\n";

            // CORRIGIDO: usa qtd_tuplas_ocup para não ler posições vazias
            for(int j = 0; j < paginas[i].qtd_tuplas_ocup; j++) {
                arquivo << paginas[i].tuplas[j].colunas[indice_coluna] << "\n";
            }
        }
    }

    arquivo.close();
    cout << "Runs salvas em " << caminho << endl;
}
