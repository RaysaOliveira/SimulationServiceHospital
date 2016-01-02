#include "estatisticas.h"
#include "fila.h"
#include <stdio.h>

float media_tempo_espera_fila(simulacao *sim){
    float soma = 0, total_utentes = sim->fila_utentes_finalizados->quant_atual;
    node *no = sim->fila_utentes_finalizados->inicio;
    if(total_utentes == 0)
        return 0;
    
    struct status_fase status_fase;
    while(no != NULL){
        //@todo está calculando o tempo de espera apenas na fase 0, mas deveria calcular de todas ou não?
        status_fase = no->utente->status_fase[0];
        soma += calcular_tempo_espera_na_fila_fase(status_fase);
        no = no->prox;
    }
    
    return soma/total_utentes;
}

float media_tempo_espera_fila_todas_simulacaoes(struct estatisticas vetor_estatisticas[], int tamanho_vetor){
    float soma = 0;
    for(int i = 0; i < tamanho_vetor; i++){
        soma += vetor_estatisticas[i].media_tempo_espera_fila;
    }
    
    return soma/(float)tamanho_vetor;
}

struct estatisticas calcular_estatisticas(simulacao *sim){
    struct estatisticas estatisticas;
    estatisticas.media_tempo_espera_fila = media_tempo_espera_fila(sim);
    return estatisticas;
}

struct estatisticas calcular_todas_estatisticas_todas_simulacoes(struct estatisticas vetor_estatisticas[], int tamanho_vetor){
    struct estatisticas estatisticas_todas_simulacoes;
    float soma = 0;
    for(int i = 0; i < tamanho_vetor; i++){
        soma += vetor_estatisticas[i].media_tempo_espera_fila;
    }
    estatisticas_todas_simulacoes.media_tempo_espera_fila = soma/(float)tamanho_vetor;
    return estatisticas_todas_simulacoes;
}

void imprimir_estatisticas_uma_simulacao(struct estatisticas *estatisticas){
    printf("Média do tempo de espera na fila: %.2f minutos\n", estatisticas->media_tempo_espera_fila);
}