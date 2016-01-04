#include <stdlib.h>
#include "random.h"
#include "twister.h"

void inicializar_seed(long seed){
    seedMT(seed);
}

float rnd () {
    //divide pelo valor max do twiste para gerar valores entre 0 e 1
    return randomMT() / (float)twister_period();
}

