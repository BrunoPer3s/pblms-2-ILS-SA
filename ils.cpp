#include "ils.h"
#include "solucao.h"
#include "sa.h"
#include "util.h"
#include <vector>
#include <algorithm>

void perturba(const Instancia* inst, int* ordem) {
    int n = inst->n;
    std::vector<int>  pred_fora(n);
    std::vector<int>  L;
    std::vector<char> em_L(n, 0);
    std::vector<int>  nova;
    L.reserve(n);
    nova.reserve(n);

    for (int i = 0; i < n; i++) {
        pred_fora[i] = inst->num_predecessores[i];
    }

    for (int i = 0; i < n; i++) {
        if (pred_fora[i] == 0) {
            L.push_back(i);
            em_L[i] = 1;
        }
    }

    int semente = ordem[0];
    for (size_t x = 0; x < L.size(); x++) {
        if (L[x] == semente) {
            L[x] = L.back();
            L.pop_back();
            break;
        }
    }
    em_L[semente] = 0;
    nova.push_back(semente);
    for (int k = 0; k < inst->num_sucessores[semente]; k++) {
        int s = inst->sucessores[semente][k];
        if (--pred_fora[s] == 0 && !em_L[s]) {
            L.push_back(s);
            em_L[s] = 1;
        }
    }

    while ((int)nova.size() < n) {
        int r = aleatorio_inteiro((int)L.size());
        int t = L[r];
        L[r] = L.back();
        L.pop_back();
        em_L[t] = 0;
        nova.push_back(t);
        for (int k = 0; k < inst->num_sucessores[t]; k++) {
            int s = inst->sucessores[t][k];
            if (--pred_fora[s] == 0 && !em_L[s]) {
                L.push_back(s);
                em_L[s] = 1;
            }
        }
    }

    std::copy(nova.begin(), nova.end(), ordem);
}

long executar_ils(const Instancia* inst, ParametrosILS p) {
    int n = inst->n, m = inst->m;
    std::vector<int>  ordem(n);
    std::vector<int>  melhor_ordem(n);
    std::vector<int>  estacao(n);
    std::vector<long> carga(m);

    long limite_inferior = inst->maior_tempo;
    long limite_media = (inst->soma_tempos + m - 1) / m;
    if (limite_media > limite_inferior) {
        limite_inferior = limite_media;
    }

    ordem_topologica(inst, ordem.data());
    decodifica_mediaCustoAlternada(inst, ordem.data(), estacao.data(), carga.data());

    long melhor_z = busca_local_sa(inst, ordem.data(), p.temp_inicial, p.temp_final,
                                   p.taxa_resfriamento, p.num_vizinhos);
    melhor_ordem = ordem;

    long iteracao = 0;
    while (iteracao < p.ils_max && melhor_z > limite_inferior) {
        ordem = melhor_ordem;
        perturba(inst, ordem.data());
        long z = busca_local_sa(inst, ordem.data(), p.temp_inicial, p.temp_final,
                                p.taxa_resfriamento, p.num_vizinhos);
        iteracao++;
        if (z < melhor_z) {
            melhor_z = z;
            melhor_ordem = ordem;
        }
    }

    return melhor_z;
}
