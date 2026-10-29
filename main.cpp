#include "instancia.h"
#include "solucao.h"
#include "ils.h"
#include "util.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

static Instancia* abrir_instancia(const char* nome, int m) {
    Instancia* inst = ler_instancia(nome, m);
    if (inst) {
        return inst;
    }
    std::string caminho = std::string("INSTANCIAS/") + nome;
    inst = ler_instancia(caminho.c_str(), m);
    if (inst) {
        return inst;
    }
    caminho += ".IN2";
    return ler_instancia(caminho.c_str(), m);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <instancia> <m> <ma|pt>\n", argv[0]);
        return 1;
    }

    int m = atoi(argv[2]);
    if (m < 1) {
        fprintf(stderr, "Erro: m deve ser >= 1 (recebido '%s')\n", argv[2]);
        return 1;
    }

    if (!strcmp(argv[3], "ma")) {
        usa_mediaCustoAlternada = 1;
    } else if (!strcmp(argv[3], "pt")) {
        usa_mediaCustoAlternada = 0;
    } else {
        fprintf(stderr, "Erro: decodificacao '%s' invalida (use ma ou pt)\n", argv[3]);
        return 1;
    }

    Instancia* inst = abrir_instancia(argv[1], m);
    if (!inst) {
        fprintf(stderr, "Erro ao ler a instancia '%s'\n", argv[1]);
        return 1;
    }
    if (m > inst->n) {
        fprintf(stderr, "Erro: m (%d) maior que o numero de tarefas (%d)\n", m, inst->n);
        liberar_instancia(inst);
        return 1;
    }

    semear_gerador((unsigned)time(NULL));

    ParametrosILS p;
    p.ils_max           = 200000;
    p.temp_inicial      = 1000.0;
    p.temp_final        = 0.1;
    p.taxa_resfriamento = 0.97;
    p.num_vizinhos      = 300;

    long z = executar_ils(inst, p);
    printf("%ld\n", z);

    liberar_instancia(inst);
    return 0;
}
