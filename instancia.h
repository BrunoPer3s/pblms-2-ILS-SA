#ifndef INSTANCIA_H
#define INSTANCIA_H

#include <vector>

struct Instancia {
    int n;
    int m;
    std::vector<int> tempo;
    std::vector<int>               num_predecessores;
    std::vector<std::vector<int>>  predecessores;
    std::vector<int>               num_sucessores;
    std::vector<std::vector<int>>  sucessores;
    long   soma_tempos;
    int    maior_tempo;
    double media;
};

Instancia* ler_instancia(const char* caminho, int m);
void liberar_instancia(Instancia* inst);

#endif
