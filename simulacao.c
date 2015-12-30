/* 
 *
 * @TODO A funcao poisson é apenas para teste. Verificar o artigo
 * http://preshing.com/20111007/how-to-generate-random-timings-for-a-poisson-process/ 
 * para implementa-la corretamente
 */

#include "simulacao.h"
#include "random.h"
#include "tipos.h"
#include "fila.h"
#include <stdlib.h>
#include <math.h>

static long seed = 9346L;

void inicializar_seed(long seed1) {
    seed = seed1;
}

void inicializar_poisson(double mean) {
    lambda = 1.0/mean;
}

double poisson() {
    float aleatorio = ran0(&seed);
    return -logf(1.0f - aleatorio) / lambda;
}

void inicializar_servidores_fase(struct fase *fase){
    //cria o vetor de servidores com o total de servidores indicados na fase
    fase->servidores = malloc(sizeof(struct utente *) * fase->total_servidores);
    for(int i=0; i<fase->total_servidores; i++){
        //inicializa cada servidor com null para indicar que todos estão livres
        fase->servidores[i]=NULL;
    }
}

void inicializar_filas_fase(struct fase *fase){
    //cria o vetor de filas com o total de filas indicados na fase
    fase->filas = malloc(sizeof(struct fila *) * fase->total_filas);
    for(int i = 0; i < fase->total_filas; i++){
        //inicializa cada fila
        fase->filas[i] = inicializar_fila();
    }
}

void inicializar_fases(struct fase fases[TOTAL_FASES], 
        int total_filas_fase[TOTAL_FASES], 
        int total_servidores_fase[TOTAL_FASES],
        int tempo_max_atendimento_fases[TOTAL_FASES]){
    
    for(int i = 0; i < TOTAL_FASES; i++){
        fases[i].numero_fase = i;
        fases[i].total_filas = total_filas_fase[i];
        fases[i].total_servidores = total_servidores_fase[i];
        fases[i].tempo_max_atendimento= tempo_max_atendimento_fases[i];
        
        inicializar_servidores_fase(&fases[i]);        
        inicializar_filas_fase(&fases[i]);
    }
}

int servidor_esta_livre(struct utente *servidores[], int posicao_a_verificar){
    if(servidores[posicao_a_verificar]==NULL)
        return 1;

    return 0;
}

int gerar_prioridade() {
    float aleatorio = ran0(&seed);

    /*
     * @todo É preciso definir tais probabilidades por parâmetro de linha de comando
     * Exemplos de probabilidades. É preciso ordenar as prioridades para que 
     * o código funcione:
     [0.0 .. 0.1] = prioridade 0
     ]0.1 .. 0.3] = prioridade 3
     ]0.3 .. 0.6] = prioridade 1
     ]0.6 .. 1.0] = prioridade 2
     */

    if (aleatorio <= 0.1)
        return 0;
    if (aleatorio <= 0.3)
        return 3;
    if (aleatorio <= 0.6)
        return 1;

    return 2;
}

int gerar_atendimento(int *total_atendimento) {
    float aleatorio = ran0(&seed);

    /*@todo
     * As probabilidades serao definidas e passadas por paramentro para função;
     * Aqui vou utilizar estas probabilidades:
     * [0.0...0,5] - ser atendido
     * ]0.5...1.0] nao ser atendido
     */

    if (aleatorio > 0.5 && *total_atendimento < 2) {
        (*total_atendimento)++;
        return 1;
    }
    return 0;
}

int escolher_especialidade() {
    float aleatorio = ran0(&seed);

    /*
     * @todo É preciso definir tais probabilidades por parâmetro de linha de comando
     * Exemplos de probabilidades. É preciso ordenar as prioridades para que 
     * o código funcione:
     [0.0 .. 0.1] = especialidade 1
     ]0.1 .. 0.3] = especialidade 2
     ]0.3 .. 0.6] = especialidade 3
     ]0.6 .. 1.0] = especialidade 4
     */

    if (aleatorio <= 0.1)
        return 1;
    if (aleatorio <= 0.3)
        return 4;
    if (aleatorio <= 0.6)
        return 2;
    
    return 3;
}

int gerar_exame() {
    float aleatorio = ran0(&seed);

    /*@todo
     * As probabilidades serao definidas e passadas por paramentro para a função;
     * Aqui vou utilizar estas probabilidades:
     * [0.0...0,5] - fazer exame
     * ]0.5...1.0] - nao fazer
     */
    if (aleatorio > 0.5) {
        return 1;
    }
    return 0;
}

int escolher_exame() {

    float aleatorio = ran0(&seed);

    /*
     * @todo É preciso definir tais probabilidades por parâmetro de linha de comando
     * Exemplos de probabilidades. É preciso ordenar as prioridades para que 
     * o código funcione:
     [0.0 .. 0.1] = exame 1
     ]0.1 .. 0.3] = exame 2
     ]0.3 .. 0.6] = exame 3
     ]0.6 .. 1.0] = exame 4
     */

    if (aleatorio <= 0.1)
        return 1;
    if (aleatorio <= 0.3)
        return 4;
    if (aleatorio <= 0.6)
        return 2;

    return 3;
}

int gerar_tempo_atendimento_fase(struct fase *fase){
    int tempo_minimo_atendimento = 5;
    /*
     * Foi multiplicado por 100, pelo fato da função retornar
     * um valor float dentre 0.0 e 1.0, multiplicando por 100, será convertido
     * em um numero inteiro com ate 3 digitos. Como o meu tempo maximo de atendimento é 
     * definido pelo campo tempo_max_atendimento na fase,
     * obtem-se o resto dividido por tal valor para garantir
     * que o valor aleatório máximo será o definido em tal variável. 
     * Como o resultado do resto vai ser entre 0 e tempo_max_atendimento-1,
     * mas deseja-se entre tempo_minimo_atendimento e tempo_max_atendimento,
     * soma-se tempo_minimo_atendimento ao resto.
     * Porém, inicialmente tinhamos valores entre 0 e tempo_max_atendimento-1,
     * agora teremos entre 1 e tempo_max_atendimento+tempo_minimo_atendimento,
     * ou seja, o valor máximo será maior que o tempo máximo definido.
     * Para evitar isso subtraiu-se o tempo_minimo_atendimento do tempo_max_atendimento
     * no momento de calcular o resto. 
     */
    int aleatorio = ran0(&seed) * 100;
    int resto = (aleatorio % (fase->tempo_max_atendimento - tempo_minimo_atendimento));
    
    return resto + tempo_minimo_atendimento;
}