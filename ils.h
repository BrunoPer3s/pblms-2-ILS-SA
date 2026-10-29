#ifndef ILS_H
#define ILS_H

#include "instancia.h"

typedef struct {
    long   ils_max;
    double temp_inicial;
    double temp_final;
    double taxa_resfriamento;
    int    num_vizinhos;
} ParametrosILS;

void perturba(const Instancia* inst, int* ordem);
long executar_ils(const Instancia* inst, ParametrosILS p);

#endif
