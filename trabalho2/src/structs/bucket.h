#ifndef BUCKET_H
#define BUCKET_H
#include <vector>

struct Bucket {
    int id;                 // ID para sabermos qual arquivo .txt ler gravar
    int PL;                 // profundidade Local
    std::vector<int> chaves;// máximo de 5 chaves
};

#endif
