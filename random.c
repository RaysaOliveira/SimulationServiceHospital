#include <stdlib.h>
#include "random.h"
#include "twister.h"

void inicializar_seed(long seed){
    seedMT(seed);
}

float rnd () {
    return randomMT() / (float)twister_period();
}

