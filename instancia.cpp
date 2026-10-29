#include "instancia.h"
#include <cstdio>

Instancia* ler_instancia(const char* caminho, int m) {
    FILE* arquivo = fopen(caminho, "r");
    if (!arquivo) {
        return nullptr;
    }

    Instancia* inst = new Instancia();
    inst->m = m;

    if (fscanf(arquivo, "%d", &inst->n) != 1) {
        fclose(arquivo);
        delete inst;
        return nullptr;
    }
    inst->tempo.resize(inst->n);
    for (int i = 0; i < inst->n; i++) {
        if (fscanf(arquivo, "%d", &inst->tempo[i]) != 1) {
            fclose(arquivo);
            delete inst;
            return nullptr;
        }
    }

    std::vector<int> origem, destino;
    int tarefa_antes, tarefa_depois;
    while (fscanf(arquivo, "%d,%d", &tarefa_antes, &tarefa_depois) == 2) {
        if (tarefa_antes == -1 && tarefa_depois == -1) {
            break;
        }
        origem.push_back(tarefa_antes - 1);
        destino.push_back(tarefa_depois - 1);
    }
    fclose(arquivo);

    inst->num_predecessores.assign(inst->n, 0);
    inst->num_sucessores.assign(inst->n, 0);
    inst->predecessores.resize(inst->n);
    inst->sucessores.resize(inst->n);
    for (size_t k = 0; k < origem.size(); k++) {
        int antes = origem[k], depois = destino[k];
        inst->sucessores[antes].push_back(depois);
        inst->predecessores[depois].push_back(antes);
        inst->num_sucessores[antes]++;
        inst->num_predecessores[depois]++;
    }

    inst->soma_tempos = 0;
    inst->maior_tempo = 0;
    for (int i = 0; i < inst->n; i++) {
        inst->soma_tempos += inst->tempo[i];
        if (inst->tempo[i] > inst->maior_tempo) {
            inst->maior_tempo = inst->tempo[i];
        }
    }
    inst->media = (double)inst->soma_tempos / (double)inst->m;

    return inst;
}

void liberar_instancia(Instancia* inst) {
    delete inst;
}
