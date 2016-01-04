#include "estatisticas.h"
#include "fila.h"
#include <stdio.h>
#include <math.h>

float calcular_media(float valores[], int tamanho_vetor){
    float soma = 0;
    for(int i = 0; i < tamanho_vetor; i++){
        soma += valores[i];
    }
    
    return soma/(float)tamanho_vetor;
}

float square(float num){
    return num * num;
}

float desvio_padrao(float valores[], int tamanho_vetor){
    float soma = 0, media = calcular_media(valores, tamanho_vetor);
    for(int i = 0; i < tamanho_vetor; i++){
        soma += square(valores[i] - media);
    }
    
    float n = tamanho_vetor;
    return sqrt(soma/(n-1));
}

float media_tempo_espera_fila_por_fase(simulacao *sim, int num_fase){
    int total_utentes = sim->fila_utentes_finalizados->quant_atual;
    node *no = sim->fila_utentes_finalizados->inicio;
    if(total_utentes == 0)
        return 0;
    
    struct status_fase status_fase;
    float valores[total_utentes];
    int i=0;
    while(no != NULL){
        float tempo_espera = calcular_tempo_espera_na_fila_fase(no->utente, num_fase);
        //printf("tempo espera %f\n", tempo_espera);
        valores[i++] = tempo_espera;
        no = no->prox;
    }
    
    return calcular_media(valores, total_utentes);
}

float media_duracao_atendimento_por_fase(simulacao *sim, int num_fase){
    int total_utentes = sim->fila_utentes_finalizados->quant_atual;
    node *no = sim->fila_utentes_finalizados->inicio;
    if(total_utentes == 0)
        return 0;
    
    float valores[total_utentes];
    int i=0;
    while(no != NULL){
        float duracao_atendimento = no->utente->status_fase[num_fase].duracao_atendimento;
        if(duracao_atendimento < 0 || duracao_atendimento > 100)
            printf("\t\t#id %d fase %d duracao %f\n", no->utente->id, num_fase, duracao_atendimento);
        valores[i++] = duracao_atendimento;
        no = no->prox;
    }
    if(i != total_utentes)
        printf("\t\t#i %d utentes %d\n", i, total_utentes);
    
    return calcular_media(valores, total_utentes);
}

float media_tempo_espera_fila_todas_fases(simulacao *sim){
    float valores[TOTAL_FASES];
    for(int i = 0; i < TOTAL_FASES; i++){
        valores[i] = media_tempo_espera_fila_por_fase(sim, i);
    }
    return calcular_media(valores, TOTAL_FASES);
}

float media_tempo_espera_fila_todas_fases_e_todas_simulacoes(struct estatisticas vetor_estatisticas[], int tamanho_vetor){
    float valores[tamanho_vetor];
    for(int i = 0; i < tamanho_vetor; i++){
        valores[i] = vetor_estatisticas[i].media_tempo_espera_fila_todas_fases;
    }
    
    return calcular_media(valores, tamanho_vetor);
}


float calcular_media_total_utentes_chegados_todas_simulacoes(struct estatisticas vetor_estatisticas[], int tamanho_vetor){
    float valores[tamanho_vetor];
    for(int i = 0; i < tamanho_vetor; i++){
        valores[i] = vetor_estatisticas[i].total_utentes_chegados;
    }
    
    return calcular_media(valores, tamanho_vetor);
}

void tempo_medio_espera_fila_todas_fases_todas_simulacoes(
    struct estatisticas *estatisticas_todas_simulacoes,
    struct estatisticas vetor_estatisticas[], int tamanho_vetor)
{
    float medias_tempo_espera_fila_todas_fases_por_simulacao[tamanho_vetor];
    
    for(int i = 0; i < tamanho_vetor; i++){
        medias_tempo_espera_fila_todas_fases_por_simulacao[i] = 
                vetor_estatisticas[i].media_tempo_espera_fila_todas_fases;
    }
    
    estatisticas_todas_simulacoes->media_tempo_espera_fila_todas_fases = 
            calcular_media(medias_tempo_espera_fila_todas_fases_por_simulacao, tamanho_vetor);
}

void tempo_medio_espera_fila_por_fase_todas_simulacoes(
    struct estatisticas *estatisticas_todas_simulacoes,
    struct estatisticas vetor_estatisticas[], int tamanho_vetor)
{
    float media_tempo_espera_fila_por_fase[TOTAL_FASES] = {0,0,0,0};
    for(int i = 0; i < tamanho_vetor; i++){
        for(int j=0; j<TOTAL_FASES; j++){
            media_tempo_espera_fila_por_fase[j] += vetor_estatisticas[i].media_tempo_espera_fila_por_fase[j];
        }
    }
    
    for(int j=0; j<TOTAL_FASES; j++){
        estatisticas_todas_simulacoes->media_tempo_espera_fila_por_fase[j] = 
                media_tempo_espera_fila_por_fase[j]/(float)tamanho_vetor;
    }    
}

float media_duracao_atendimento_todas_fases(simulacao *sim){
    float valores[TOTAL_FASES];
    for(int i = 0; i < TOTAL_FASES; i++){
        valores[i] = media_duracao_atendimento_por_fase(sim, i);
    }
    return calcular_media(valores, TOTAL_FASES);
}

void media_duracao_atendimento_por_fase_todas_simulacoes(
    struct estatisticas *estatisticas_todas_simulacoes,
    struct estatisticas vetor_estatisticas[], int tamanho_vetor)
{
    float media_duracao_atendimento_por_fase[TOTAL_FASES] = {0,0,0,0};
    for(int i = 0; i < tamanho_vetor; i++){
        for(int j=0; j<TOTAL_FASES; j++){
            media_duracao_atendimento_por_fase[j] += vetor_estatisticas[i].media_duracao_atendimento_por_fase[j];
        }
    }
    
    for(int j=0; j<TOTAL_FASES; j++){
        estatisticas_todas_simulacoes->media_duracao_atendimento_por_fase[j] = 
                media_duracao_atendimento_por_fase[j]/(float)tamanho_vetor;
    }    
}
   
void media_duracao_atendimento_todas_fase_todas_simulacoes(
    struct estatisticas *estatisticas_todas_simulacoes,
    struct estatisticas vetor_estatisticas[], int tamanho_vetor)
{
    float medias_duracao_atend_todas_fases_por_simulacao[tamanho_vetor];
    
    for(int i = 0; i < tamanho_vetor; i++){
        medias_duracao_atend_todas_fases_por_simulacao[i] = 
                vetor_estatisticas[i].media_duracao_atendimento_todas_fases;
    }
    
    estatisticas_todas_simulacoes->media_duracao_atendimento_todas_fases = 
            calcular_media(medias_duracao_atend_todas_fases_por_simulacao, tamanho_vetor);
}


struct estatisticas calcular_estatisticas(simulacao *sim){
    struct estatisticas estatisticas;
    estatisticas.media_tempo_espera_fila_todas_fases = media_tempo_espera_fila_todas_fases(sim);
    estatisticas.total_utentes_chegados = sim->total_utentes_chegaram;
    /*Para cada simulação náo há média de total de utentes chegados, pois 
    tem-se o total exato de utentes chegados.
    Assim, seta -1 para indicar que não há valor para tal campo*/
    estatisticas.media_total_utentes_chegados = -1;
    
    for(int i = 0; i < TOTAL_FASES; i++){
        estatisticas.media_tempo_espera_fila_por_fase[i] = media_tempo_espera_fila_por_fase(sim, i);
        estatisticas.media_duracao_atendimento_por_fase[i] = media_duracao_atendimento_por_fase(sim, i);
    }
    estatisticas.media_duracao_atendimento_todas_fases = media_duracao_atendimento_todas_fases(sim);

    return estatisticas;
}

struct estatisticas calcular_todas_estatisticas_todas_simulacoes(struct estatisticas vetor_estatisticas[], int tamanho_vetor){
    struct estatisticas estatisticas_todas_simulacoes;
    estatisticas_todas_simulacoes.total_utentes_chegados = 0;
    
    for(int i = 0; i < tamanho_vetor; i++)
        estatisticas_todas_simulacoes.total_utentes_chegados += vetor_estatisticas[i].total_utentes_chegados;
    
    estatisticas_todas_simulacoes.media_total_utentes_chegados = 
            calcular_media_total_utentes_chegados_todas_simulacoes(vetor_estatisticas, tamanho_vetor);
    
    tempo_medio_espera_fila_todas_fases_todas_simulacoes(
            &estatisticas_todas_simulacoes, vetor_estatisticas, tamanho_vetor);
    tempo_medio_espera_fila_por_fase_todas_simulacoes(
            &estatisticas_todas_simulacoes, vetor_estatisticas, tamanho_vetor);
    
    media_duracao_atendimento_por_fase_todas_simulacoes(
            &estatisticas_todas_simulacoes, vetor_estatisticas, tamanho_vetor);
    media_duracao_atendimento_todas_fase_todas_simulacoes(
            &estatisticas_todas_simulacoes, vetor_estatisticas, tamanho_vetor);

    return estatisticas_todas_simulacoes;
}

void imprimir_estatisticas_uma_simulacao(struct estatisticas *estatisticas){
    for(int i = 0; i < TOTAL_FASES; i++) {
        printf("\tMédia do tempo de espera na fila para fase %d: %.2f minutos\n", i, estatisticas->media_tempo_espera_fila_por_fase[i]);
    }   
    printf("\tMédia do tempo de espera na fila para todas a fases: %.2f minutos\n\n", estatisticas->media_tempo_espera_fila_todas_fases);

    for(int i = 0; i < TOTAL_FASES; i++) {
        printf("\tMédia de duração atendimento para fase %d: %.2f minutos\n", i, estatisticas->media_duracao_atendimento_por_fase[i]);
    }  
    printf("\tMédia de duração de atendimento para todas a fases: %.2f minutos\n\n", estatisticas->media_duracao_atendimento_todas_fases);
    
    if(estatisticas->media_total_utentes_chegados != -1)
        printf("\tMédia do total de utentes chegados no sistema: %.2f\n", estatisticas->media_total_utentes_chegados);
    printf("\tTotal de utentes chegados: %d\n", estatisticas->total_utentes_chegados);
}