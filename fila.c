#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

int vazia(fila *f){
    if(f->quant_atual==0)
        return 1;
    else return 0;
}

void inicializar_fila(fila *f){
    f->inicio = NULL;
    f->fim = NULL;
    f->quant_atual = 0;
    f->total_utentes_chegados = 0;
}

void limpar_fila(fila *f){
    while(!vazia(f))
        remover_inicio(f);
}

int inserir(struct utente utente, fila *f){
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

struct utente remover_inicio(fila *f){
    node * excluir = f->inicio; 
    struct utente utente;
    utente.id = 0; //se o id for zero, significa que nao foi removido ngm pq a fila ta vazia
    if(!vazia(f)){
        utente=f->inicio->utente; // pessoa vai receber a pessoa do inicio da fila
        f->inicio=f->inicio->prox; // inicio recebe o prox do inicio
        free(excluir); 
        f->quant_atual--;
        if (f->quant_atual == 0)
            f->fim=NULL;
    }
    
    return utente;
}

void imprimir_utente(char mensagem[], struct utente utente){
    printf("%s - ID: %d \tPrioridade: %d \tchegada: %d t.atend: %d t.part: %d\n", 
            mensagem,
            utente.id, utente.prioridade, 
            utente.fase.tempo_chegada,
            utente.fase.tempo_atendimento,
            utente.fase.tempo_partida);
}

void listar(fila *f){
    if(vazia(f)) {
        printf("Fila Vazia! \n");
        return;
    }

    node * aux = f->inicio;
    while(aux!=NULL){ // se aux for null é que chegou no fim da fila e nao tem valor
        imprimir_utente("", aux->utente);
        aux= aux->prox;
    }
}


