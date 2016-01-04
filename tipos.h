/* 
 * File:   tipos.h
 * Author: raysaoliveira
 *
 * Define todas as structs utiliadas no sistema
 */

#ifndef TIPOS_H
#define	TIPOS_H

#define TOTAL_FASES 4
#define TOTAL_PRIORIDADES 4
#define TOTAL_ESPECIALIDADES_MEDICAS 4

/**
 * Struct utilizada pelo utente para armazenar os tempos
 * em cada fase do atendimento.
 */
struct status_fase{
    int tempo_chegada;
    int tempo_inicio_atendimento;
    int duracao_atendimento;
    int tempo_partida; 
};

struct utente{
    int id;
    struct status_fase status_fase[TOTAL_FASES];
    int prioridade;  
    
    /**
     * vetor para controlar os retornos aos medicos. O tamanho máximo dele é 
     * a quantidade máxima de medicos que o utente pode consultar.
     * Cada posição do vetor indica se o utente retornou ou náo
     * no médico de mesmo número do índice indicado.
     * 0 indica que o utente nao retornou no medico da posicao indicada e
     * um que já retornou. 
     */
    int * retorno_medicos;
    
    /**
     * Indica os tipos de médico em que o utente foi atendido.
     */
    int * especialidades_medicas_consultadas;
    
    /**
     * vetor para controlar o total de exames solicitados por cada médico. O tamanho max dele é 
     * a quantidade maxima de medicos que o utente pode consultar.
     * Cada posição do vetor indica se o médico do índice indicado 
     * já prescreveu exames ao utente ou não.
     * Usando tal vetor também é possível saber por quantos
     * médicos o utente já passou (basta contar o total de posições
     * cujo valor é diferente de -1)
     */
    int * exames_medicos;
    
    /** Indica o total de atendimentos médicos completos do utente.
     * Representa o total de vezes que o utente seguiu o fluxo de ir ao médico,
     * fazer exames e retornar ao médico para apresentar tais exames.
     * Cada vez que ele concluir este fluxo, tal variável é incrementada
     */
    int total_atendimentos_concluidos;    
};

typedef struct node{//no node tem pessoa e prox
    struct utente *utente;
    struct node * prox; //ponteiro pro proximo node;
} node;

struct fila{
    node * inicio;
    node * fim;
    int quant_atual;
};

/**
 * Struct que contêm todos os dados para gerenciar os servidores e as 
 * filas de utentes de uma determinada fase.
 */
struct fase{
    /**
     * O número da fase começa em 0 (FASE1) e vai até TOTAL_FASES-1
     */
    int numero_fase;
    
    /**
     * Total de servidores da fase (total de elementos do vetor servidores)
     */
    int total_servidores;
    /**
     * Total de filas da fase (total de elementos do vetor filas)
     */
    int total_filas;
    
    /**
     * Após ser chamado pelo servidor, este é o tempo máximo que o utente vai 
     * levar para ser atendido pelo servidor na fase (logo, não é o tempo máximo de
     * espera, que normalmete será maior que este valor).
     */
    int tempo_max_atendimento;
    
    /** Vetor de servidores da fase.
     * Cada posição do vetor representa o estado
     * de um servidor. O valor NULL indica que o servidor
     * está livre, caso contrário indica o utente sendo atendido.
     * 
     * Usar utente **servidores é o mesmo que usar utente *servidores[],
     * ou seja, a variável é um vetor de ponteiros para um utente.
     * No entanto, se for usado [] é preciso indicar o tamanho do vetor.
     * Usando * no lugar de [] estamos dizendo que o vetor não tem um tamanho
     * pre-definido.
     */
    struct utente **servidores;
    
    /**
     * Vetor de filas da fase. Se um determinado vetor
     * tiver mais de uma fila, indica que cada fila é para utentes
     * de uma determinada prioridade. Se tiver uma fila só,
     * todos os utentes vão para tal fila
     **/
    struct fila **filas;
};

 typedef struct simulacao {
    long seed;
    int max_consulta_medicas_por_utente;
    float intervalo_medio_entre_chegadas_utentes;
    float probabilidade_de_utente_consultar_com_segundo_medico;
    
    /**
     * Cada valor define o intervalo de probabilidades para cada prioridade
     * dos utentes do sistema (que vai de 0 a 3).
     */
    float probabilidades_prioridades[TOTAL_PRIORIDADES];

    /**
     * Cada valor define o intervalo de probabilidades para cada especialidade
     * médica em que os utentes podem ser atendidos (que vai de 0 a 3).
     */
    float probabilidades_especialidade_medica[TOTAL_ESPECIALIDADES_MEDICAS];

    /**
     * Total de minutos para executar cada simulação
     */
    int total_minutos_simulacao;
    
    /**
     * Total de simulações a serem executadas
     */
    int total_simulacoes;
    int total_utentes_chegaram;
    
    struct fase fases[TOTAL_FASES];
    /**
     * Fila para guardar todos os utentes que já finalizaram
     * todos os atendimentos necessários para eles.
     * Tal fila não faz parte de fato do fluxo da simulação,
     * é usada apenas para calcular as estatísicas no final.
     */
    struct fila *fila_utentes_finalizados;
    int minuto_atual;
    
    /**Indica se em cada simulação serão impressos dados de cada utente a 
     medica que eles passam pelas fases do atendimento.
     0 indica que não serão impressos e 1 que serão.*/
    int imprimir_dados_utentes_individuais;
} simulacao;

/**
 * Armazena os dados para uma simulação, como médias e desvios padrões.
 */
struct estatisticas {
    int total_utentes_chegados;
    float media_total_utentes_chegados;
    float media_tempo_espera_fila_todas_fases;
    float media_tempo_espera_fila_por_fase[TOTAL_FASES];
    
    float media_duracao_atendimento_todas_fases;
    float media_duracao_atendimento_por_fase[TOTAL_FASES];
    
};


#endif	/* TIPOS_H */

