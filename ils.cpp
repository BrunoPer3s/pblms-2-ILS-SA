#include "ils.h"
#include "solucao.h"
#include "sa.h"
#include "util.h"
#include <vector>
#include <algorithm>

void perturba(const Instancia* inst, int* ordem) {
    int n = inst->n;
    std::vector<int>  predecessores_pendentes(n);
    std::vector<int>  tarefas_liberadas;
    std::vector<char> esta_liberada(n, 0);
    std::vector<int>  nova_ordem;
    tarefas_liberadas.reserve(n);
    nova_ordem.reserve(n);

    for (int i = 0; i < n; i++) {
        predecessores_pendentes[i] = inst->num_predecessores[i];
    }

    for (int i = 0; i < n; i++) {
        if (predecessores_pendentes[i] == 0) {
            tarefas_liberadas.push_back(i);
            esta_liberada[i] = 1;
        }
    }

    int semente = ordem[0];
    for (size_t x = 0; x < tarefas_liberadas.size(); x++) {
        if (tarefas_liberadas[x] == semente) {
            tarefas_liberadas[x] = tarefas_liberadas.back();
            tarefas_liberadas.pop_back();
            break;
        }
    }
    esta_liberada[semente] = 0;
    nova_ordem.push_back(semente);
    for (int k = 0; k < inst->num_sucessores[semente]; k++) {
        int s = inst->sucessores[semente][k];
        if (--predecessores_pendentes[s] == 0 && !esta_liberada[s]) {
            tarefas_liberadas.push_back(s);
            esta_liberada[s] = 1;
        }
    }

    while ((int)nova_ordem.size() < n) {
        int r = aleatorio_inteiro((int)tarefas_liberadas.size());
        int t = tarefas_liberadas[r];
        tarefas_liberadas[r] = tarefas_liberadas.back();
        tarefas_liberadas.pop_back();
        esta_liberada[t] = 0;
        nova_ordem.push_back(t);
        for (int k = 0; k < inst->num_sucessores[t]; k++) {
            int s = inst->sucessores[t][k];
            if (--predecessores_pendentes[s] == 0 && !esta_liberada[s]) {
                tarefas_liberadas.push_back(s);
                esta_liberada[s] = 1;
            }
        }
    }

    std::copy(nova_ordem.begin(), nova_ordem.end(), ordem);
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
