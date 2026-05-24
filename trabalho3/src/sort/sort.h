#ifndef SORT_H
#define SORT_H

#include "../structs/dados.h"

void mergeSort(
    vector<Tupla> &tuplas,
    int esquerda,
    int direita,
    int indice_coluna
);

void merge(
    vector<Tupla> &tuplas,
    int esquerda,
    int meio,
    int direita,
    int indice_coluna
);

#endif