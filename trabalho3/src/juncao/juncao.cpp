#include "juncao.h"
#include <iostream>

using namespace std;

Tabela MergeJoin::executar(
    Tabela &tabela1,
    Tabela &tabela2,
    int indice1,
    int indice2
) {

    cout << "\n............. MERGE JOIN .............\n";

    Tabela resultado;

    // ponteiros tabela 1
    int pag1 = 0;
    int tup1 = 0;

    // ponteiros tabela 2
    int pag2 = 0;
    int tup2 = 0;

    // página atual do resultado
    Pagina paginaResultado;

    // enquanto existir páginas nas duas tabelas
    while(pag1 < tabela1.paginas.size() && pag2 < tabela2.paginas.size()) {

        Pagina &p1 = tabela1.paginas[pag1];
        Pagina &p2 = tabela2.paginas[pag2];

        // segurança
        if(tup1 >= p1.tuplas.size()) {

            pag1++;
            tup1 = 0;
            continue;
        }

        if(tup2 >= p2.tuplas.size()) {

            pag2++;
            tup2 = 0;
            continue;
        }

        Tupla &t1 = p1.tuplas[tup1];
        Tupla &t2 = p2.tuplas[tup2];

        string chave1 =
            t1.colunas[indice1];

        string chave2 =
            t2.colunas[indice2];

        // CHAVES IGUAIS => FAZ JOIN
        if(chave1 == chave2) {

            cout
                << "MATCH: "
                << chave1
                << endl;

            Tupla novaTupla;

            // adiciona colunas tabela1
            for(string c : t1.colunas) {

                novaTupla.colunas.push_back(c);
            }

            // adiciona colunas tabela2
            for(string c : t2.colunas) {

                novaTupla.colunas.push_back(c);
            }

            // tenta inserir
            if(!paginaResultado.inserirTupla(novaTupla)) {

                // página cheia
                resultado.paginas.push_back(
                    paginaResultado
                );

                paginaResultado = Pagina();

                paginaResultado.inserirTupla(
                    novaTupla
                );
            }

            // avança tabela1
            tup1++;

            // acabou página?
            if(tup1 >= p1.tuplas.size()) {
                pag1++;
                tup1 = 0;
            }
        }

        // CHAVE1 MENOR
        else if(chave1 < chave2) {

            tup1++;

            if(
                tup1 >= p1.tuplas.size()
            ) {

                pag1++;
                tup1 = 0;
            }
        }

        // CHAVE2 MENOR
        else {

            tup2++;

            if(tup2 >= p2.tuplas.size()) {

                pag2++;
                tup2 = 0;
            }
        }
    }

    // salva última página
    if(paginaResultado.tuplas.size() > 0) {

        resultado.paginas.push_back(
            paginaResultado
        );
    }

    resultado.qtd_paginas = resultado.paginas.size();

    cout << "\n Join Finalizado \n";

    cout << "Paginas resultado: " << resultado.qtd_paginas << endl;

    return resultado;
}