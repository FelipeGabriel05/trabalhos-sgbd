#include "buffer.h"
#include <iostream>
#include <algorithm>

bool Buffer::inserir(Pagina p) {
    if(cheio()) {
        cout << "Buffer cheio" << endl;
        Evict();
    }

    frames.push_back(p);
    cout << "Pagina inserida no frame " << frames.size() - 1 << endl;
    return true;
}

void Buffer::limpar() {
    frames.clear();
    cout << "Buffer limpo\n";
}

bool Buffer::cheio() {
    if(frames.size() == capacidade) {
        return true;
    }
    return false;
}

int Buffer::qtdFrames() {
    return frames.size();
}

void Buffer::gerarRuns(Tabela &tabela, DiscoSimulado &disco, int indice_coluna) {
    cout << "........ Gerando Runs .........." << endl;

    for(size_t i = 0; i < tabela.paginas.size(); i++) {

        inserir(tabela.paginas[i]);

        // começa a ordenação se o buffer está cheio ou quase cheio
        if(cheio() || i == tabela.paginas.size() -1) {
            cout << "Ordenando paginas do buffer" << endl;

            // junta todas as tuplas
            vector<Tupla> Tuplas;

            // para cada página pega as tuplas presente nelas e junta em um vetor só
            for(Pagina p : frames) {
                for(Tupla t : p.tuplas) {
                    Tuplas.push_back(t);
                }
            }

            mergeSort(
                Tuplas,
                0,
                Tuplas.size() - 1,
                indice_coluna
            );

            cout << "Ordenacao " << Tuplas.size() << " Tuplas ordenadas" << endl;

            // Inicio da run 
            int inicio_run = disco.paginas.size();

            // recria páginas
            Pagina novaPagina;
            for(Tupla t : Tuplas) {
                if(!novaPagina.inserirTupla(t)) {
                    disco.salvarPagina(novaPagina);
                    novaPagina = Pagina();
                    novaPagina.inserirTupla(t);
                }
            }
 
            if(novaPagina.tuplas.size() > 0) {
                disco.salvarPagina(novaPagina);
            }

            // Fim da run
            int fim_run = disco.paginas.size() - 1;

            disco.runs.push_back({
                inicio_run,
                fim_run
            });

            cout << "Run salva no disco: paginas " << inicio_run << " ate " << fim_run << endl;

            // Limpa o buffer
            limpar();
        }
    }
    cout << "............................" << endl;
}
        
void Buffer::mergeRuns(DiscoSimulado &disco) {

}
 
// retorna apenas o índice a ser removido.
int Buffer::FIFO() {
    return 0;
}

void Buffer::Evict() {
    int indice = FIFO();
    cout << "Removendo pagina do frame " << indice << " usando FIFO" << endl;

    // faz a remoção
    frames.erase(frames.begin() + indice);
}

void Buffer::mostrarBuffer() {

    cout << "\n......... BUFFER ...............\n";

    for(size_t i = 0; i < frames.size(); i++) {

        cout << "Frame "
             << i
             << " -> "
             << frames[i].tuplas.size()
             << " tuplas"
             << endl;
    }

    cout << "..........................\n";
}