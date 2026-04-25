#include "read.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

string lerPaginaCSV(int key) {
    ifstream arquivo("../bancodedados.csv");
    string linha;

    if (!arquivo.is_open()) return "";

    getline(arquivo, linha); // Pula cabeçalho

    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        stringstream ss(linha);
        string id_str, texto;

      
        if (!getline(ss, id_str, '\t')) continue;
        if (!getline(ss, texto)) continue;

        try {
            if (stoi(id_str) == key) {
                arquivo.close();
                return texto; 
            }
        } catch (...) {
            continue; 
        }
    }

    arquivo.close();
    return "Nao encontrado";
}
