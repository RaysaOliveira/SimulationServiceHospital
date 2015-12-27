#include "fila.h"
#include "simulacao.h"
#include <stdlib.h>
#include <stdio.h>


int vazia(fila *f){
    if(f->quant_atual==0)
        return 1;
    else return 0;
}

fila * inicializar_fila(){
    fila *f = malloc(sizeof(fila));
    f->inicio = NULL;
    f->fim = NULL;
    f->quant_atual = 0;
    f->total_utentes_chegados = 0;
    return f;
}

void inicializar_vetor_filas(fila * filas[], int tamanho_vetor_filas){
    for(int i=0; i<tamanho_vetor_filas; i++){
        filas[i] = inicializar_fila();
    }
}

void limpar_fila(fila *f){
    while(!vazia(f)) {
        struct utente * utente = remover_inicio(f);
        free(utente);
    }
}    

void limpar_vetor_filas(fila * filas[], int tamanho_vetor_filas){
    for(int i=0; i<tamanho_vetor_filas; i++){
        limpar_fila(filas[i]);
    }
}

int inserir(struct utente * utente, fila *f){
    //printf("inserir: endereco do utente %x \t endereco para onde o utente aponta %x \n", &utente, utente);
    
    node *novo = (node *)malloc(sizeof(node));//aloca tamanho do novo
    if(novo==NULL)
        return 0; //se retornar , náo tem mais espaço para alocar memória
    
    novo->utente = utente; //jogando a pessoa que recebi pra dentro do novo;
    novo->prox = NULL;// pq sempre insere no fim, logo o fim nunca tera proximo;
    if(vazia(f)){
        f->inicio= novo;
    } 
    else{
        f->fim->prox=novo; //o prox do final da fila aponta p onde o novo está apontando;
    }
    
    f->fim=novo;
    f->quant_atual++;
    f->total_utentes_chegados++;
    
    //se chegou aqui, retorna 1 para indicar que o novo foi inserido na fila
    return 1;
} 

struct utente *remover_inicio(fila *f){
    node * excluir = f->inicio; 
    struct utente *utente = NULL;
    if(!vazia(f)){
        utente=f->inicio->utente; // UTENTE vai receber O UTENTE do inicio da fila
        f->inicio=f->inicio->prox; // inicio recebe o prox do inicio
        free(excluir); 
        f->quant_atual--;
        if (f->quant_atual == 0)
            f->fim=NULL;
    }
    
    return utente;
}

void imprimir_utente(char mensagem[], struct utente * utente, int indice_fase){
    printf(
        "%s: id %3d Prior. %2d | chegada %3d inicio atend %3d dur. atend %3d part %3d espera %3d\n", 
        mensagem,
        utente->id, utente->fase[indice_fase].prioridade, 
        utente->fase[indice_fase].tempo_chegada,
            utente->fase[indice_fase].tempo_inicio_atendimento,
        utente->fase[indice_fase].duracao_atendimento,
        utente->fase[indice_fase].tempo_partida,
        calcular_tempo_espera_na_fila_fase(utente->fase[indice_fase]));
}

void listar(fila *f){
    if(vazia(f)) {
        printf("Fila Vazia! \n");
        return;
    }

    node * aux = f->inicio;
    while(aux!=NULL){ // se aux for null é que chegou no fim da fila e nao tem valor
        imprimir_utente("", aux->utente, FASE1);
        aux= aux->prox;
    }
}

int calcular_tempo_espera_na_fila_fase(struct fase fase){
    return fase.tempo_inicio_atendimento-fase.tempo_chegada; 
}

int calcular_tempo_partida_na_fila_fase(struct fase fase){
    return fase.tempo_inicio_atendimento + fase.duracao_atendimento;
}

struct utente * remover_utente_da_primeira_fila_com_clientes_em_espera(fila * filas[], int tamanho_vetor_filas){
    for(int i=0; i<tamanho_vetor_filas; i++){
        if(!vazia(filas[i])){
            return remover_inicio(filas[i]);                 
        }   
    }
    return NULL;
}

