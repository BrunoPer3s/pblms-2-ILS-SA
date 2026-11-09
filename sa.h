#ifndef SA_H
#define SA_H

#include "instancia.h"

long busca_local_sa(const Instancia *inst, int *ordem, long z_inicial, double temp_inicial, double temp_final,
                    double taxa_resfriamento, int num_vizinhos);

#endif
