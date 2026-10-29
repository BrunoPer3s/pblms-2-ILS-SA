#include "util.h"
#include <random>

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
