#ifndef SOLUCAO_H
#define SOLUCAO_H

#include "instancia.h"

static inline long calcula_z(const long* carga, int m) {
    long z = 0;
    for (int s = 0; s < m; s++) {
        if (carga[s] > z) {
            z = carga[s];
        }
    }
    return z;
}

void ordem_topologica(const Instancia* inst, int* ordem);
long decodifica_pisoTetoBuscaBinaria(const Instancia* inst, const int* ordem, int* estacao, long* carga);
long decodifica_mediaCustoAlternada(const Instancia* inst, const int* ordem, int* estacao, long* carga);

extern int usa_mediaCustoAlternada;

long decodifica(const Instancia* inst, const int* ordem, int* estacao, long* carga);
void gera_vizinho(const Instancia* inst, int* ordem);

#endif
