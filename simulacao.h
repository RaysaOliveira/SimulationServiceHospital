#ifndef SIMULACAO_H
#define	SIMULACAO_H
#include "fila.h"

#define TOTAL_SERVIDORES_FASE1 2
#define TOTAL_SERVIDORES_FASE2 2
#define TOTAL_SERVIDORES_FASE3 4
#define TOTAL_SERVIDORES_FASE4 2
#define TEMPO_MAX_ATENDIMENTO_FASE1 8
/**
 * O lambda é 1/a media do tempo de chegada;
 */
double lambda;

/** Vetor de servidores das fases.
 * Cada posição do vetor representa o estado
 * de um servidor. O valor 0 indica que o servidor
 * está livre e maior que 0 indica o id do utente sendo atendido.
 */
struct utente * servidores_fase1[TOTAL_SERVIDORES_FASE1];
struct utente * servidores_fase2[TOTAL_SERVIDORES_FASE2];

/** na fase 3, cada servidor é um médico
 */
struct utente * servidores_fase3[TOTAL_SERVIDORES_FASE3];
struct utente * servidores_fase4[TOTAL_SERVIDORES_FASE4];

/**
 * Inicializa todos os vetores de servidores em cada posição com NULL para indicar que todos 
 * os servidores estao livres;
 */
void inicializar_servidores_todas_fases();

/**
 * Procura o primeiro servidor livre e retorna o indice dele.
 *
 * @param servidores o servidor que quero fazer a busca
 * @param tamanho_vetor_servidor o tamanho do vetor do servidor
 * @return a posição do primeiro servidor livre ou -1 se nenhum servidor
 * estiver disponível 
 */
int procurar_indice_servidor_livre(struct utente *servidores[], int tamanho_vetor_servidor);


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
 * 1 = vermelho
 * 2 = laranja
 * 3 = amarelo
 * 4 = verde
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

