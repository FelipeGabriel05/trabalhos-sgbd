#include "disco.h"
#include <iostream>
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

    cout << "Quantidade de paginas: "
         << paginas.size()
         << endl;

    for(size_t i = 0; i < paginas.size(); i++) {

        cout << "Pagina "
             << i
             << " -> "
             << paginas[i].tuplas.size()
             << " tuplas"
             << endl;
    }

    cout << "...........................\n";
}

void DiscoSimulado::mostrarConteudoDisco(int indice_coluna) {

    cout << "\n................ CONTEUDO DISCO ...................\n";

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

                cout
                    << t.colunas[indice_coluna]
                    << endl;
            }
        }
    }

    cout << "\n===================================\n";
}