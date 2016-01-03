#ifndef SIMULACAO_H
#define	SIMULACAO_H
#include "tipos.h"


/**
 * O lambda é 1/(média do tempo de chegada);
 */
float lambda;

/**
 * Inicializa todas as fases da simulação, o que inclui criar
 * os vetores de servidores e de filas e inicializar cada servidor e cada fila
 * destes vetores.
 * 
 * @param sim Struc contendo todos os dados da simulação
 * @param total_filas_fase Vetor contendo o total de filas em cada fase
 * @param total_servidores_fase Vetor contendo o total de servidors em cada fase
 */
void inicializar_simulacao(simulacao *sim, 
        int total_filas_fase[TOTAL_FASES], 
        int total_servidores_fase[TOTAL_FASES],
        int tempo_max_atendimento_fases[TOTAL_FASES],
        float probabilidades_prioridades[TOTAL_PRIORIDADES], 
        float probabilidades_especialidade_medica[TOTAL_ESPECIALIDADES_MEDICAS]);

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
void inicializar_poisson(float mean);

/**
 * Gera um número pseudo aleatório seguindo a distribuição de Poisson.
 * Fonte: http://preshing.com/20111007/how-to-generate-random-timings-for-a-poisson-process/  
 */
float poisson();

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
 * Escolhe aleatoriamente uma de 4 especialidades;
 * @return um valor para representar a especialidade escolhida 
 * 1 = ginecologia
 * 2 = clinica geral
 * 3 = dermatologia
 * 4 = pediatria
 */
int escolher_especialidade();

/**
 * Gera aleatoriamente um valor uniform entre 0 e o simulacao.max_consulta_medicas_por_utente
 * para indicar o total de exames que o utente fará.
 * @param sim
 * @return 
 */
int gerar_total_exames(simulacao *sim);

/**
 * Gera um tempo aleatório de atendimento para um utente em uma fase.
 * @param fase Fase para qual deseja-se gerar um tempo de atendimento aleatório para um utente
 * @return número aleatorio que representa o tempo de atendimento na fase. 
 */
int gerar_duracao_atendimento(struct fase *fase);
/**
 * Decide aleatoriamente se o utente irá para outro médico ou não,
 * caso ele não tenha feito o máximo de consultas permitido.
 * 
 *
 * @return 1 se o utente vai para outro médico, ou 0 caso contrário
 */
int vai_para_outro_medico(simulacao *sim, struct utente *utente);

int total_utentes_atualmente_em_fila(struct fase fase);
int total_utentes_atualmente_em_fila_em_todas_as_fases(struct fase fases[TOTAL_FASES]);
int total_utentes_em_atendimento(struct fase fase);
int total_utentes_em_atendimento_em_todas_fases(struct fase fases[TOTAL_FASES]);


/**
 * Libera todas as variáveis alocadas dinamicamente para a simulação
 * @param sim
 */
void liberar_filas_servidores_e_utentes_simulacao(simulacao *sim);

void imprimir_parametros_simulacao(simulacao *sim);

#endif	/* SIMULACAO_H */


