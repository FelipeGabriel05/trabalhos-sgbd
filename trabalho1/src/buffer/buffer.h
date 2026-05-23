#include "../leitura/read.h"
#include "../structs/pages.h"
#include <vector>
using namespace std;


enum class Politicas {FIFO, LRU, CLOCK, MRU };

class Buffer {

    public: 
        Buffer(Politicas p) {
            politicaAtual = p;
        }

        void DisplayStats();
        void DisplayCache(); 
        string Fetch(int key);

    private:
        Politicas politicaAtual;
        int contador_global = 0;
        size_t ponteiro_clock = 0;
        int cache_hit = 0;
        int cache_miss = 0;
        vector<pages> paginas;
        size_t capacidade = 5;
        pages* buscar(int key);
        bool inserir(pages p);
        void Evict();
        int FIFO();
        int LRU();
        int MRU();
        int CLOCK();       
};
