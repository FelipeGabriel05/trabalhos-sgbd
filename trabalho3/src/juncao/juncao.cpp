#include "juncao.h"
#include <iostream>
#include <fstream>

using namespace std;

Tabela MergeJoin::executar(
    Tabela &tabela1,
    Tabela &tabela2,
    int indice1,
    int indice2
) {

    cout << "\n............. MERGE JOIN .............\n";

    Tabela resultado;

    // Marcadores de leitura da tabela1:
    // pag1 mostra qual página estamos lendo agora
    // tup1 mostra qual tupla dentro dessa página
    int pag1 = 0;
    int tup1 = 0;

    // Marcadores de leitura da tabela2 (mesma lógica)
    int pag2 = 0;
    int tup2 = 0;

    // Frame de saída: acumula as tuplas resultado até
    // encher (12 tuplas), depois é salvo e limpo
    Pagina paginaResultado;

    // Percorre as duas tabelas ao mesmo tempo enquanto
    // nenhuma delas acabar
    while(pag1 < (int)tabela1.paginas.size() && pag2 < (int)tabela2.paginas.size()) {

        Pagina &p1 = tabela1.paginas[pag1];
        Pagina &p2 = tabela2.paginas[pag2];

        // Se esgotou as tuplas reais da página atual da tabela1
        // passa para a próxima página.
        // Usamos qtd_tuplas_ocup em vez de tuplas.size() para
        // não ler posições vazias do array fixo de 12
        if(tup1 >= p1.qtd_tuplas_ocup) {
            pag1++;
            tup1 = 0;
            continue;
        }

        // Mesma verificação para tabela2
        if(tup2 >= p2.qtd_tuplas_ocup) {
            pag2++;
            tup2 = 0;
            continue;
        }

        Tupla &t1 = p1.tuplas[tup1];
        Tupla &t2 = p2.tuplas[tup2];

        // Pega os valores da coluna de junção das duas tuplas
        string chave1 = t1.colunas[indice1]; // ex: "Chardonnay" (Grapes)
        string chave2 = t2.colunas[indice2]; // ex: "Chardonnay" (Wines)


        // Caso 1: chaves iguais => encontramos um grupo de JOIN
        if(chave1 == chave2) {

            cout << "grupo de join: " << chave1 << endl;

            // Grupos que vão acumular todas as tuplas
            // com essa mesma chave em cada tabela.
            // Necessário para gerar o produto cartesiano
            // quando há duplicatas 
            // evita não analisar todos os casos.
            vector<Tupla> grupo1;
            vector<Tupla> grupo2;

            string chaveAtual = chave1;

            // Coleta todas as tuplas com essa chave na tabela1
            while(pag1 < (int)tabela1.paginas.size()) {

                Pagina &paginaAtual = tabela1.paginas[pag1];

                // Página esgotada => avança para a próxima
                if(tup1 >= paginaAtual.qtd_tuplas_ocup) {
                    pag1++;
                    tup1 = 0;
                    continue;
                }

                Tupla atual = paginaAtual.tuplas[tup1];

                // Encontrou uma tupla com chave diferente
                // o grupo desta chave acabou, para a coleta
                if(atual.colunas[indice1] != chaveAtual) {
                    break;
                }

                grupo1.push_back(atual);
                tup1++;

                // Se acabou a página após avançar, passa para a próxima
                if(tup1 >= paginaAtual.qtd_tuplas_ocup) {
                    pag1++;
                    tup1 = 0;
                }
            }

            // Coleta todas as tuplas com essa chave na tabela2
            // (mesma lógica da coleta acima)
            while(pag2 < (int)tabela2.paginas.size()) {

                Pagina &paginaAtual = tabela2.paginas[pag2];

                // Página esgotada => avança para a próxima
                if(tup2 >= paginaAtual.qtd_tuplas_ocup) {
                    pag2++;
                    tup2 = 0;
                    continue;
                }

                Tupla atual = paginaAtual.tuplas[tup2];

                // Chave diferente → grupo encerrado
                if(atual.colunas[indice2] != chaveAtual) {
                    break;
                }

                grupo2.push_back(atual);
                tup2++;

                // Fim de página → avança
                if(tup2 >= paginaAtual.qtd_tuplas_ocup) {
                    pag2++;
                    tup2 = 0;
                }
            }

            // Produto cartesiano: combina cada tupla do grupo1
            // com cada tupla do grupo2.
            //     Ex: grupo1 = [Chardonnay_grape]
            //     grupo2 = [Vinho_A, Vinho_B, Vinho_C]
            //     resultado = 3 tuplas combinadas
            for(Tupla a : grupo1) {
                for(Tupla b : grupo2) {

                    // Monta a tupla resultado juntando todas as
                    // colunas da tabela1 seguidas das da tabela2
                    Tupla novaTupla;

                    for(string c : a.colunas)
                        novaTupla.colunas.push_back(c);

                    for(string c : b.colunas)
                        novaTupla.colunas.push_back(c);

                    // Tenta inserir no frame de saída.
                    // Se o frame estiver cheio (12 tuplas),
                    // salva a página no resultado e limpa o frame
                    if(!paginaResultado.inserirTupla(novaTupla)) {

                        resultado.paginas.push_back(paginaResultado);
                        paginaResultado = Pagina();
                        paginaResultado.inserirTupla(novaTupla);
                    }
                }
            }
        }

        // Caso 2: chave1 < chave2
        // A tabela1 está "atrás" — avança ela para alcançar
        // a tabela2
        else if(chave1 < chave2) {

            tup1++;

            if(tup1 >= p1.qtd_tuplas_ocup) {
                pag1++;
                tup1 = 0;
            }
        }

        // Caso 3: chave2 < chave1
        // A tabela2 está "atrás" — avança ela para alcançar
        // a tabela1
        else {

            tup2++;

            if(tup2 >= p2.qtd_tuplas_ocup) {
                pag2++;
                tup2 = 0;
            }
        }
    }

    // Ao terminar o percurso, o frame de saída pode ter
    // tuplas que ainda não foram salvas (última página
    // parcial). Salva o que sobrou
    if(paginaResultado.qtd_tuplas_ocup > 0) {
        resultado.paginas.push_back(paginaResultado);
    }

    resultado.qtd_paginas = resultado.paginas.size();

    cout << "\nJOIN FINALIZADO\n";
    cout << "Paginas resultado: " << resultado.qtd_paginas << endl;

    return resultado;
}

void MergeJoin::salvarResultadoCSV(Tabela &tabela, string caminho) {

    ofstream arquivo(caminho);

    if(!arquivo.is_open()) {

        cout << "Erro ao criar arquivo resultado\n";
        return;
    }

    for(Pagina p : tabela.paginas) {
        for(Tupla t : p.tuplas) {
            for(size_t i = 0; i < t.colunas.size(); i++) {
                arquivo << t.colunas[i];
                if(i < t.colunas.size() - 1) {
                    arquivo << "\t";
                }
            }
            arquivo << "\n";
        }
    }

    arquivo.close();

    cout << "Resultado salvo em " << caminho << endl;
}