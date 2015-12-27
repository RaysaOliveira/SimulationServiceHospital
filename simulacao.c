/* 
 *
 * @TODO A funcao poisson é apenas para teste. Verificar o artigo
 * http://preshing.com/20111007/how-to-generate-random-timings-for-a-poisson-process/ 
 * para implementa-la corretamente
 */

#include "simulacao.h"
#include "random.h"
#include <stdlib.h>
#include <math.h>

static long seed = 9346L;

void inicializar_parametros_simulacao(long seed1) {
    seed = seed1;
}

void poisson_init(double mean) {
    lambda = 1 / mean;
}

double poisson() {
    float aleatorio = ran0(&seed);
    return -logf(1.0f - aleatorio) / lambda;
}

void inicializar_vetores_de_ponteiros(void * servidores[], int tamanho_vetor_servidor){
    for(int i=0; i<tamanho_vetor_servidor; i++){
        servidores[i]=NULL;
    }

}
void inicializar_servidores_todas_fases(){
    /**
     * o tipo void **, quer dizer que é um vetor de ponteiro para qualquer coisa
     */
    inicializar_vetores_de_ponteiros((void **)servidores_fase1, total_servidores_fase1);
    inicializar_vetores_de_ponteiros((void **)servidores_fase2, total_servidores_fase2);
    inicializar_vetores_de_ponteiros((void **)servidores_fase3, total_servidores_fase3);
    inicializar_vetores_de_ponteiros((void **)servidores_fase4, total_servidores_fase4);
} 

int servidor_esta_livre(struct utente *servidores[], int posicao_a_verificar){
    if(servidores[posicao_a_verificar]==NULL)
        return 1;

    return 0;
}

int gerar_prioridade_fase2() {
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

int eh_prioritario() {
    float aleatorio = ran0(&seed);

    /*@todo
     * As probabilidades serao definidas e passadas por paramentro para a função;
     * Aqui vou utilizar estas probabilidades:
     * [0.0...0,7] - geral
     * ]0.7...1.0] - prioritario
     */
    if (aleatorio > 0.7) {
        return 0;
    }
    return 1;
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

int gerar_tempo_atendimento_fase1(){
    /*
     * Foi multiplicado por 100, pelo fato da função retornar
     * um valor float dentre 0.0 e 1.0, multiplicando por 100, será convertido
     * em um numero inteiro com ate 3 digitos. Como o meu tempo maximo de atendimento é 8
     * eu obtenho o resto dividido por ele, para limitar que ele é o maximo e somo 
     * +1 pq o resultado do resto vai ser entre 0 e 7. 
     */
    int aleatorio = ran0(&seed) * 100;
    return (aleatorio % tempo_max_atendimento_fase1) + 1;
}