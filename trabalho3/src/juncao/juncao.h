#ifndef JOIN_H
#define JOIN_H

#include "../structs/dados.h"

class MergeJoin {

    public:

        Tabela executar(
            Tabela &tabela1,
            Tabela &tabela2,
            int indice1,
            int indice2
        );
};

#endif