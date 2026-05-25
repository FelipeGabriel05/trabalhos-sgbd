#include "sort.h"
using namespace std;

void merge(
    vector<Tupla> &tuplas,
    int esquerda,
    int meio,
    int direita,
    int indice_coluna
) {

    int n1 = meio - esquerda + 1;
    int n2 = direita - meio;

    vector<Tupla> L(n1);
    vector<Tupla> R(n2);

    // copia esquerda
    for(int i = 0; i < n1; i++) {
        L[i] = tuplas[esquerda + i];
    }

    // copia direita
    for(int j = 0; j < n2; j++) {
        R[j] = tuplas[meio + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = esquerda;

    while(i < n1 && j < n2) {

        if(L[i].colunas[indice_coluna] <= R[j].colunas[indice_coluna]) {

            tuplas[k] = L[i];

            i++;
        }
        else {

            tuplas[k] = R[j];

            j++;
        }

        k++;
    }

    // sobra esquerda
    while(i < n1) {

        tuplas[k] = L[i];

        i++;
        k++;
    }

    // sobra direita
    while(j < n2) {

        tuplas[k] = R[j];

        j++;
        k++;
    }
}

void mergeSort(
    vector<Tupla> &tuplas,
    int esquerda,
    int direita,
    int indice_coluna
) {

    if(esquerda >= direita) {
        return;
    }

    int meio = esquerda + (direita - esquerda) / 2;

    mergeSort(
        tuplas,
        esquerda,
        meio,
        indice_coluna
    );

    mergeSort(
        tuplas,
        meio + 1,
        direita,
        indice_coluna
    );

    merge(
        tuplas,
        esquerda,
        meio,
        direita,
        indice_coluna
    );
}