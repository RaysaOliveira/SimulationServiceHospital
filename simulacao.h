#ifndef SIMULACAO_H
#define	SIMULACAO_H
#include "fila.h"

/**
 * Define o índice de cada vetor no vetor de fases do utente
 */
#define FASE1 0
#define FASE2 1
#define FASE3 2
#define FASE4 3

#define total_servidores_fase1 2
#define total_servidores_fase2 2
#define total_servidores_fase3 4
#define total_servidores_fase4 2
#define tempo_max_atendimento_fase1 8
#define total_filas_fase1 1
#define total_filas_fase2 4

/**
 * Struct que contêm todos os dados para gerenciar os servidores e as 
 * filas de utentes de uma determinada fase.
 */
struct fase{
    int total_servidores;
    int total_filas;
    
    /*Usar utente **servidores é o mesmo que usar utente *servidores[],
     ou seja, a variável é um vetor de ponteiros para um utente.
     No entanto, se for usado [] é preciso indicar o tamanho do vetor.
     Usando * no lugar de [] estamos dizendo que o vetor não tem um tamanho
     pre-definido*/
    struct utente **servidores;
    struct fila **filas;
};

/**
 * O lambda é 1/a media do tempo de chegada;
 */
double lambda;

/** Vetor de servidores das fases.
 * Cada posição do vetor representa o estado
 * de um servidor. O valor 0 indica que o servidor
 * está livre e maior que 0 indica o id do utente sendo atendido.
 */
struct utente * servidores_fase1[total_servidores_fase1];
struct utente * servidores_fase2[total_servidores_fase2];

/** na fase 3, cada servidor é um médico
 */
struct utente * servidores_fase3[total_servidores_fase3];
struct utente * servidores_fase4[total_servidores_fase4];

/**
 * Vetores de filas para cada fase. Se um determinado vetor
 * tiver mais de uma fila, indica que cada fila é para utentes
 * de uma determinada prioridade. Se tiver uma fila só,
 * todos os utentes vão para tal fila
 **/
struct fila * filas_fase1[total_filas_fase1];
struct fila * filas_fase2[total_filas_fase2];



/**
 * Inicializa todos os vetores de servidores em cada posição com NULL para indicar que todos 
 * os servidores estao livres;
 */
void inicializar_servidores_todas_fases();

/**
 * Verifica se o servidor na posição indicada está livre
 *
 * @param servidores o vetor de servidores que deseja verificar
 * @param posicao_a_verificar posição do servidor a ser verificado
 * @return 1 se o servidor estiver livre e 0 caso contrário 
 */
int servidor_esta_livre(struct utente *servidores[], int posicao_a_verificar);


/**
 * Inicializa o gerador
 * @param mean A média desejada para o gerador Poisson
 */
void poisson_init(double mean);

/**
 * Gera um número pseudo aleatório seguindo a distribuição de Poisson.
 * Fonte: http://preshing.com/20111007/how-to-generate-random-timings-for-a-poisson-process/  
 */
double poisson();

/**
 * Seta os parâmetros a serem utilizados para executar a simulação,
 * seed, como probabilidades para cada tipo de utente, etc.
 * 
 * @param seed Semente a ser usada pelo random (é o valor inicial
 * da sequência de números aleatórios a ser gerada) 
 */
void inicializar_parametros_simulacao(long seed);

/**
 * Gera uma prioridade aleatoriamente para um utente na fase 2.
 * @param seed Semente a ser usada pelo random (é o valor inicial
 * da sequência de números aleatórios a ser gerada)
 * @return Retorna um valor inteiro entre 1 e 4 sendo:
 * 0 = vermelho
 * 1 = laranja
 * 2 = amarelo
 * 3 = verde
 */
int gerar_prioridade_fase2();

/** Gera prioridades na primeira fase da simulaçao.
 * @return o valor 1 para indicar que o utente é prioritario e 
 * 0 para indicar que o utente não é prioritário
 * 
 */
int eh_prioritario();

/**
 * Defini se o utente vai ser atendido novamente ou não. 
 * Ele nao será atendido novamente se ele ja foi atendido por 2 medicos.
 * @param total_atendimento total de vezes que o utente foi atendido.
 * @return 0 caso o utente não seja atendido e 1 se ele for atendido
 */
int gerar_atendimento(int *total_atendimento);

/**
 * Escolhe aleatoriamente uma de 4 especialidades;
 * @return um valor para representar a especialidade escolhida 
 * 1 = ginecologia
 * 2 = clinica geral
 * 3 = dermatologia
 * 4 = pediatria
 */
int escolher_especialidade();

/**
 * Defini se o utente vai fazer exame novamente ou não. 
 * @return 0 caso o utente não faça exame e 1 se ele fizer exame
 */
int gerar_exame();


/**
 * Escolhe aleatoriamente um de 4 tipos de exame;
 * @return um valor para representar o exame escolhido 
 * 1 = exame de mama
 * 2 = eletrocardiograma
 * 3 = exame de sangue
 * 4 = exame de urina
 */
int escolher_exame();

/**
 * 
 * @return número aleatorio que representa o tempo de atendimento na fase 1. 
 */
int gerar_tempo_atendimento_fase1();


#endif	/* SIMULACAO_H */

