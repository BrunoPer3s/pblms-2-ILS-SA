#ifndef ILS_H
#define ILS_H

#include "instancia.h"
#include <vector>

typedef struct {
    long   ils_max;
    double temp_inicial;
    double temp_final;
    double taxa_resfriamento;
    int    num_vizinhos;
    int    para_no_limite_inferior;
} ParametrosILS;

struct ResultadoILS {
    long             z;
    long             limite_inferior;
    long             iteracoes;
    double           tempo_melhor;
    std::vector<int> melhor_ordem;
};

void perturba(const Instancia* inst, int* ordem);
ResultadoILS executar_ils(const Instancia* inst, ParametrosILS p);

#endif
