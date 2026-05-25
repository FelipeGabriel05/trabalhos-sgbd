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

            if(!Tuplas.empty()) {
                mergeSort(
                    Tuplas,
                    0,
                    Tuplas.size() - 1,
                    indice_coluna
                );
            }

            cout << "Ordenacao " << Tuplas.size() << " Tuplas ordenadas" << endl;

            // Inicio da run 
            int inicio_run = disco.paginas.size();

            // recria páginas para serem salvas no disco
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
        
// fase de intercalação externa
void Buffer::mergeRuns(DiscoSimulado &disco, int indice_coluna) {

    cout << "\n.............. MERGE EXTERNO ..................\n";

    // Continua intercalando até sobrar apenas 1 run no disco
    while(disco.runs.size() > 1) {

        // Lista que vai guardar as novas runs criadas nessa passagem.
        //
        // pair<int,int> é simplesmente um par de dois números inteiros:
        //   .first  => índice da PRIMEIRA página da run no disco
        //   .second => índice da ÚLTIMA  página da run no disco
        //
        // Exemplo: {5, 8} significa "a run vai da página 5 até a página 8"
        //
        // vector<pair<int,int>> é uma lista que cresce conforme
        // novas runs são criadas. No final de cada passagem
        // essa lista substitui a lista antiga de runs do disco.
        vector<pair<int,int>> novasRuns;

        // Pega as runs de 4 em 4 (respeitando o limite do buffer)
        for(size_t grupo = 0; grupo < disco.runs.size(); grupo += 4) {

            // Lista das runs que estão sendo intercaladas agora.
            // Cada EstadoRun guarda um "marcador de leitura"
            // em qual página está no momento e qual tupla está lendo
            vector<EstadoRun> runsAtivas;

            // Carrega até 4 runs do grupo atual para a memória
            for(size_t i = grupo; i < grupo + 4 && i < disco.runs.size(); i++) {
                EstadoRun r;

                r.inicio = disco.runs[i].first;  // primeira página desta run
                r.fim    = disco.runs[i].second; // última página desta run

                // Começa a leitura do início da run
                r.paginaAtual = r.inicio;
                r.tuplaAtual  = 0;

                runsAtivas.push_back(r);

                cout
                    << "RUN carregada: "
                    << r.inicio
                    << " ate "
                    << r.fim
                    << endl;
            }

            // Frame de saída: aqui vão parar as tuplas já ordenadas.
            // Quando encher será descarregado no disco e limpo
            Pagina saida;

            // Anota em qual página do disco a nova run começa,
            // para registrar o intervalo {inicio, fim} no final
            int inicioNovaRun = disco.paginas.size();

            // Repete até todas as runs do grupo estarem esgotadas
            while(true) {

                // Procura qual run tem a menor tupla agora
                int   menorRun   = -1; // valor inicial
                Tupla menorTupla;      // tupla com o menor valor encontrado

                // Percorre todas as runs ativas e compara a tupla atual de cada uma
                // Quem tem o menor valor na coluna de junção é escolhida.
                for(size_t r = 0; r < runsAtivas.size(); r++) {

                    // Ignora runs que já foram completamente lidas passando para a próxima iteração
                    if(!runsAtivas[r].ativa) {
                        continue;
                    }

                    EstadoRun &run   = runsAtivas[r];
                    Pagina    &pagina = disco.paginas[run.paginaAtual];
                    Tupla      atual  = pagina.tuplas[run.tuplaAtual];

                    // Se essa tupla é menor do que a menor encontrada até agora
                    // ela passa a ser a nova candidata
                    if(menorRun == -1 || atual.colunas[indice_coluna] < menorTupla.colunas[indice_coluna]) {
                        menorRun   = r;
                        menorTupla = atual;
                    }
                }

                // Se não encontrou nenhuma run ativa, todas foram esgotadas.
                // O grupo terminou — sai do loop interno
                if(menorRun == -1) {
                    break;
                }

                // Tenta colocar a tupla vencedora no frame de saída.
                // Se o frame estiver cheio (12 tuplas), grava no disco primeiro
                if(!saida.inserirTupla(menorTupla)) {

                    // Frame cheio => salva a página no disco e limpa o frame
                    disco.salvarPagina(saida);
                    saida = Pagina();

                    // Agora há espaço — insere a tupla no frame limpo
                    saida.inserirTupla(menorTupla);
                }

                // Avança o marcador de leitura da run que acabou de ceder a tupla
                EstadoRun &run   = runsAtivas[menorRun];
                Pagina    &pagina = disco.paginas[run.paginaAtual];

                run.tuplaAtual++;

                // Se leu todas as tuplas da página atual, passa para a próxima página
                if(run.tuplaAtual >= pagina.tuplas.size()) {
                    run.paginaAtual++;
                    run.tuplaAtual = 0;
                }

                // Se passou da última página da run, essa run está esgotada
                if(run.paginaAtual > run.fim) {
                    run.ativa = false;
                }
            }

            // Ao terminar o grupo, pode ter sobrado tuplas no frame de saída
            // que ainda não foram gravadas — salva o que restou
            if(saida.tuplas.size() > 0) {
                disco.salvarPagina(saida);
            }

            // Registra o intervalo de páginas que forma a nova run recém-criada
            int fimNovaRun = disco.paginas.size() - 1;
            novasRuns.push_back({inicioNovaRun, fimNovaRun});

            cout << "Nova RUN criada: " << inicioNovaRun << " ate " << fimNovaRun << endl;
        }

        // Substitui a lista antiga de runs pela nova lista (menor número de runs,
        // cada uma cobrindo mais páginas ordenadas)
        disco.runs = novasRuns;

        cout << "\nPassagem concluida. Runs restantes: " << disco.runs.size() << endl;
    }

    cout << "\n............... MERGE FINALIZADO .............\n";
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

        cout << "Frame " << i << " -> " << frames[i].tuplas.size() << " tuplas" << endl;
    }

    cout << "..........................\n";
}