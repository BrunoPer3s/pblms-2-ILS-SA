#include "instancia.h"
#include "solucao.h"
#include "ils.h"
#include "util.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

static Instancia* abrir_instancia(const char* nome, int m, std::string& caminho) {
    caminho = nome;
    Instancia* inst = ler_instancia(caminho.c_str(), m);
    if (inst) {
        return inst;
    }
    caminho = std::string("INSTANCIAS/") + nome;
    inst = ler_instancia(caminho.c_str(), m);
    if (inst) {
        return inst;
    }
    caminho += ".IN2";
    return ler_instancia(caminho.c_str(), m);
}

static std::string nome_arquivo_saida(const std::string& caminho, int m, const char* decodificacao) {
    size_t inicio = caminho.find_last_of("/\\");
    if (inicio == std::string::npos) {
        inicio = 0;
    } else {
        inicio++;
    }
    std::string nome = caminho.substr(inicio);
    size_t ponto = nome.find_last_of('.');
    if (ponto != std::string::npos) {
        nome = nome.substr(0, ponto);
    }
    return nome + "_m" + std::to_string(m) + "_" + decodificacao + ".txt";
}

static int escreve_saida(const char* arquivo_saida, const Instancia* inst, const std::string& caminho,
                         const char* decodificacao, unsigned semente, const ResultadoILS& resultado,
                         double tempo_total) {
    FILE* saida = fopen(arquivo_saida, "w");
    if (!saida) {
        return 0;
    }

    int n = inst->n, m = inst->m;
    std::vector<int>  estacao(n);
    std::vector<long> carga(m);
    decodifica(inst, resultado.melhor_ordem.data(), estacao.data(), carga.data());

    fprintf(saida, "Instancia: %s | m: %d | decodificacao: %s | seed: %u\n",
            caminho.c_str(), m, decodificacao, semente);
    fprintf(saida, "FO (tempo de ciclo z): %ld\n", resultado.z);
    fprintf(saida, "Lower bound: %ld\n", resultado.limite_inferior);
    fprintf(saida, "Tempo total: %.2f seg\n", tempo_total);
    fprintf(saida, "Tempo ate melhor solucao: %.2f seg\n", resultado.tempo_melhor);
    fprintf(saida, "Iteracoes ILS: %ld\n", resultado.iteracoes);
    fprintf(saida, "--------------------------------------------------\n");
    for (int s = 0; s < m; s++) {
        fprintf(saida, "Estacao %d (carga %ld): ", s + 1, carga[s]);
        for (int i = 0; i < n; i++) {
            if (estacao[i] == s) {
                fprintf(saida, "%d ", i + 1);
            }
        }
        fprintf(saida, "\n");
    }

    fclose(saida);
    return 1;
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

    std::string caminho;
    Instancia* inst = abrir_instancia(argv[1], m, caminho);
    if (!inst) {
        fprintf(stderr, "Erro ao ler a instancia '%s'\n", argv[1]);
        return 1;
    }
    if (m > inst->n) {
        fprintf(stderr, "Erro: m (%d) maior que o numero de tarefas (%d)\n", m, inst->n);
        liberar_instancia(inst);
        return 1;
    }

    unsigned semente = (unsigned)time(NULL);
    semear_gerador(semente);

    ParametrosILS p;
    p.ils_max                 = 5000;
    p.temp_inicial            = 1000.0;
    p.temp_final              = 0.1;
    p.taxa_resfriamento       = 0.97;
    p.num_vizinhos            = 300;
    p.para_no_limite_inferior = 1;

    inicia_cronometro();
    ResultadoILS resultado = executar_ils(inst, p);
    double tempo_total = tempo_decorrido();

    printf("%ld\n", resultado.z);

    std::string arquivo_saida = nome_arquivo_saida(caminho, m, argv[3]);
    if (!escreve_saida(arquivo_saida.c_str(), inst, caminho, argv[3], semente, resultado, tempo_total)) {
        fprintf(stderr, "Erro ao criar o arquivo de saida '%s'\n", arquivo_saida.c_str());
    }

    liberar_instancia(inst);
    return 0;
}
