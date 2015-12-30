#ifndef SIMULACAO_H
#define	SIMULACAO_H
#include "tipos.h"

/**
 * Define o índice de cada vetor no vetor de fases do utente
 */
#define FASE1 0
#define FASE2 1
#define FASE3 2
#define FASE4 3
#define TOTAL_FASES 4


/**
 * O lambda é 1/a media do tempo de chegada;
 */
double lambda;

/**
 * Inicializa todas as fases da simulação, o que inclui criar
 * os vetores de servidores e de filas e inicializar cada servidor e cada fila
 * destes vetores.
 * 
 * @param fases Vetor de fases a ser inicializado
 * @param total_filas_fase Vetor contendo o total de filas em cada fase
 * @param total_servidores_fase Vetor contendo o total de servidors em cada fase
 */
void inicializar_fases(struct fase fases[TOTAL_FASES], 
        int total_filas_fase[TOTAL_FASES], 
        int total_servidores_fase[TOTAL_FASES],
        int tempo_max_atendimento_fases[TOTAL_FASES]);

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
void inicializar_poisson(double mean);

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
void inicializar_seed(long seed);

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
int gerar_prioridade();


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
 * Gera um tempo aleatório de atendimento para um utente em uma fase.
 * @param fase Fase para qual deseja-se gerar um tempo de atendimento aleatório para um utente
 * @return número aleatorio que representa o tempo de atendimento na fase. 
 */
int gerar_tempo_atendimento_fase(struct fase *fase);


#endif	/* SIMULACAO_H */

