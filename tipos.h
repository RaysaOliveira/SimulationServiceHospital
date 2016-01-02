/* 
 * File:   tipos.h
 * Author: raysaoliveira
 *
 * Define todas as structs utiliadas no sistema
 */

#ifndef TIPOS_H
#define	TIPOS_H

#define TOTAL_FASES 4

struct status_fase{
    int tempo_chegada;
    int tempo_inicio_atendimento;
    int duracao_atendimento;
    int tempo_partida; 
};

struct utente{ //tydef renomear o tipo. Ex: struct pessoa para pessoa;
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
     * vetor para controlar o total de exames solicitados por cada médico. O tamanho max dele é 
     * a quantidade maxima de medicos que o utente pode consultar.
     * Cada posição do vetor indica se o médico do índice indicado 
     * já prescreveu exames ao utente ou não.
     * Usando tal vetor também é possível saber por quantos
     * médicos o utente já passou (basta contar o total de posições
     * cujo valor é diferente de -1)
     */
    int * exames_medicos;
};

typedef struct node{//no node tem pessoa e prox
    struct utente *utente;
    struct node * prox; //ponteiro pro proximo node;
} node;

struct fila{
    node * inicio;
    node * fim;
    int quant_atual;
    int total_utentes_chegados;
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
     * Tempo máximo que o cliente vai levar para ser atendido na fase
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
    double intervalo_medio_entre_chegadas_utentes;
    int total_minutos_simulacao;
    struct fase fases[TOTAL_FASES];
    /**
     * Fila para guardar todos os utentes que já finalizaram
     * todos os atendimentos necessários para eles.
     * Tal fila não faz parte de fato do fluxo da simulação,
     * é usada apenas para calcular as estatísicas no final.
     */
    struct fila *fila_utentes_finalizados;
    int minuto_atual;
} simulacao;


#endif	/* TIPOS_H */

