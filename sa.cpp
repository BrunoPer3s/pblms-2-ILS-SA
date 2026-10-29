#include "sa.h"
#include "solucao.h"
#include "util.h"
#include <vector>
#include <algorithm>
#include <cmath>

long busca_local_sa(const Instancia* inst, int* ordem,
                    double temp_inicial, double temp_final,
                    double taxa_resfriamento, int num_vizinhos) {

    int n = inst->n;
    int m = inst->m;

    std::vector<int>  ordem_atual(ordem, ordem + n);
    std::vector<int>  ordem_vizinha(n);
    std::vector<int>  melhor_ordem(ordem, ordem + n);
    std::vector<int>  estacao(n);
    std::vector<long> carga(m);

    long z_atual  = decodifica(inst, ordem_atual.data(), estacao.data(), carga.data());
    long z_melhor = z_atual;

    double T = temp_inicial;
    while (T > temp_final) {
        for (int i = 0; i < num_vizinhos; i++) {
            ordem_vizinha = ordem_atual;
            gera_vizinho(inst, ordem_vizinha.data());
            long z_vizinho = decodifica(inst, ordem_vizinha.data(), estacao.data(), carga.data());
            long delta = z_vizinho - z_atual;

            if (delta < 0 || aleatorio01() < exp(-(double)delta / T)) {
                ordem_atual = ordem_vizinha;
                z_atual = z_vizinho;
            }

            if (z_vizinho < z_melhor) {
                z_melhor = z_vizinho;
                melhor_ordem = ordem_vizinha;
            }
        }
        T *= taxa_resfriamento;
    }

    std::copy(melhor_ordem.begin(), melhor_ordem.end(), ordem);
    return z_melhor;
}
