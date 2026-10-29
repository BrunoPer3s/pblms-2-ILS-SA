#include "solucao.h"
#include "util.h"
#include <vector>

void ordem_topologica(const Instancia* inst, int* ordem) {
    int n = inst->n;
    std::vector<int>  grau_entrada(n);
    std::vector<char> colocada(n, 0);

    for (int i = 0; i < n; i++) {
        grau_entrada[i] = inst->num_predecessores[i];
    }

    for (int pos = 0; pos < n; pos++) {
        int escolhida = -1;
        for (int i = 0; i < n; i++) {
            if (!colocada[i] && grau_entrada[i] == 0) {
                if (escolhida == -1 || inst->tempo[i] < inst->tempo[escolhida]) {
                    escolhida = i;
                }
            }
        }

        ordem[pos] = escolhida;
        colocada[escolhida] = 1;

        for (int k = 0; k < inst->num_sucessores[escolhida]; k++) {
            grau_entrada[inst->sucessores[escolhida][k]]--;
        }
    }
}

static long calcula_z(const long* carga, int m) {
    long z = 0;
    for (int s = 0; s < m; s++) {
        if (carga[s] > z) {
            z = carga[s];
        }
    }
    return z;
}

static int cabe_em_m_estacoes(const Instancia* inst, const int* ordem, long alvo) {
    int grupos = 1;
    long soma = 0;
    for (int indice = 0; indice < inst->n; indice++) {
        long t = inst->tempo[ordem[indice]];
        if (soma + t > alvo) {
            grupos++;
            soma = t;
        } else {
            soma += t;
        }
    }
    return grupos <= inst->m;
}

long decodifica_pisoTetoBuscaBinaria(const Instancia* inst, const int* ordem, int* estacao, long* carga) {
    int n = inst->n, m = inst->m;

    long piso = inst->maior_tempo;
    long teto = inst->soma_tempos;
    while (piso < teto) {
        long meio = piso + (teto - piso) / 2;
        if (cabe_em_m_estacoes(inst, ordem, meio)) {
            teto = meio;
        } else {
            piso = meio + 1;
        }
    }
    long alvo = piso;

    for (int s = 0; s < m; s++) {
        carga[s] = 0;
    }
    int estacao_atual = 0;
    for (int indice = 0; indice < n; indice++) {
        int i = ordem[indice];
        if (estacao_atual < m - 1 && carga[estacao_atual] > 0 && carga[estacao_atual] + inst->tempo[i] > alvo) {
            estacao_atual++;
        }
        estacao[i] = estacao_atual;
        carga[estacao_atual] += inst->tempo[i];
    }
    return calcula_z(carga, m);
}

long decodifica_mediaCustoAlternada(const Instancia* inst, const int* ordem, int* estacao, long* carga) {
    int n = inst->n, m = inst->m;
    for (int s = 0; s < m; s++) {
        carga[s] = 0;
    }
    int estacao_atual = 0;
    for (int indice = 0; indice < n; indice++) {
        int i = ordem[indice];
        if (estacao_atual < m - 1 && carga[estacao_atual] > 0) {
            int numero_estacao = estacao_atual + 1;
            if (numero_estacao % 2 == 1) {
                if (carga[estacao_atual] + inst->tempo[i] > inst->media) {
                    estacao_atual++;
                }
            } else {
                if (carga[estacao_atual] > inst->media) {
                    estacao_atual++;
                }
            }
        }
        estacao[i] = estacao_atual;
        carga[estacao_atual] += inst->tempo[i];
    }
    return calcula_z(carga, m);
}

int usa_mediaCustoAlternada = 0;

long decodifica(const Instancia* inst, const int* ordem, int* estacao, long* carga) {
    if (usa_mediaCustoAlternada) {
        return decodifica_mediaCustoAlternada(inst, ordem, estacao, carga);
    } else {
        return decodifica_pisoTetoBuscaBinaria(inst, ordem, estacao, carga);
    }
}

void gera_vizinho(const Instancia* inst, int* ordem) {
    int n = inst->n;
    std::vector<char> marca(n, 0);

    int pos_rho = -1, pos_e = -1, pos_d = -1;

    for (int tentativa = 0; tentativa < 2 * n; tentativa++) {
        int p = aleatorio_inteiro(n);
        int rho = ordem[p];

        for (int k = 0; k < inst->num_sucessores[rho]; k++) {
            marca[inst->sucessores[rho][k]] = 1;
        }
        int phi_d = p;

        for (int q = p + 1; q < n && !marca[ordem[q]]; q++) {
            phi_d = q;
        }

        for (int k = 0; k < inst->num_sucessores[rho]; k++) {
            marca[inst->sucessores[rho][k]] = 0;
        }

        for (int k = 0; k < inst->num_predecessores[rho]; k++) {
            marca[inst->predecessores[rho][k]] = 1;
        }
        int phi_e = p;

        for (int q = p - 1; q >= 0 && !marca[ordem[q]]; q--) {
            phi_e = q;
        }
        
        for (int k = 0; k < inst->num_predecessores[rho]; k++) {
            marca[inst->predecessores[rho][k]] = 0;
        }

        if (phi_d > p || phi_e < p) {
            pos_rho = p;
            pos_e = phi_e;
            pos_d = phi_d;
            break;
        }
    }
    if (pos_rho < 0) {
        return;
    }

    int esquerda = pos_rho - pos_e;
    int total    = pos_d - pos_e;
    int r = aleatorio_inteiro(total);
    int pos_mu = (r < esquerda) ? (pos_e + r) : (pos_rho + 1 + (r - esquerda));

    int rho = ordem[pos_rho];
    if (pos_mu > pos_rho) {
        for (int q = pos_rho; q < pos_mu; q++) {
            ordem[q] = ordem[q + 1];
        }
    } else {
        for (int q = pos_rho; q > pos_mu; q--) {
            ordem[q] = ordem[q - 1];
        }
    }
    ordem[pos_mu] = rho;
}
