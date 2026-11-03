#include "util.h"
#include <random>
#include <chrono>

static std::mt19937 gerador;

void semear_gerador(unsigned semente) {
    gerador.seed(semente);
}

double aleatorio01(void) {
    return (double)gerador() / ((double)gerador.max() + 1.0);
}

int aleatorio_inteiro(int n) {
    std::uniform_int_distribution<int> distribuicao(0, n - 1);
    return distribuicao(gerador);
}

static std::chrono::steady_clock::time_point inicio_cronometro;

void inicia_cronometro(void) {
    inicio_cronometro = std::chrono::steady_clock::now();
}

double tempo_decorrido(void) {
    std::chrono::duration<double> decorrido = std::chrono::steady_clock::now() - inicio_cronometro;
    return decorrido.count();
}
