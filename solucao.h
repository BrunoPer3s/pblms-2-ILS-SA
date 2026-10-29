#ifndef SOLUCAO_H
#define SOLUCAO_H

#include "instancia.h"

void ordem_topologica(const Instancia* inst, int* ordem);
long decodifica_pisoTetoBuscaBinaria(const Instancia* inst, const int* ordem, int* estacao, long* carga);
long decodifica_mediaCustoAlternada(const Instancia* inst, const int* ordem, int* estacao, long* carga);

extern int usa_mediaCustoAlternada;

long decodifica(const Instancia* inst, const int* ordem, int* estacao, long* carga);
void gera_vizinho(const Instancia* inst, int* ordem);

#endif
