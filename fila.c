#include "fila.h"
#include "tipos.h"
#include <stdlib.h>
#include <stdio.h>


int vazia(struct fila *f){
    if(f->quant_atual==0)
        return 1;
    else return 0;
}

struct fila * inicializar_fila(){
    struct fila *f = malloc(sizeof(struct fila));
    f->inicio = NULL;
    f->fim = NULL;
    f->quant_atual = 0;
    f->total_utentes_chegados = 0;
    return f;
}

void limpar_fila(struct fila *f){
    while(!vazia(f)) {
        struct utente * utente = remover_inicio(f);
        free(utente->exames_medicos);
        free(utente->retorno_medicos);
        free(utente->especialidades_medicas_consultadas);
        free(utente);
    }
}    

void limpar_vetor_filas(struct fila * filas[], int tamanho_vetor_filas){
    for(int i=0; i<tamanho_vetor_filas; i++){
        limpar_fila(filas[i]);
    }
}

int inserir(struct utente * utente, struct fila *f){
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

struct utente *remover_inicio(struct fila *f){
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
    char mensagem1[200];
    sprintf(mensagem1, "%s fase %d", mensagem, indice_fase);
    
    printf("%s: id %3d ", mensagem1, utente->id);
    if(utente->prioridade >= 0)
         printf("Prior %2d | ", utente->prioridade);
    else printf("      %2s | ", "-");
    
    printf("cheg. %3d ", utente->status_fase[indice_fase].tempo_chegada);
    int inicio = utente->status_fase[indice_fase].tempo_inicio_atendimento;
    if(inicio > 0)
         printf("inic. atend %3d ", inicio);
    else printf("            %3s ", "-");
    
    int duracao = utente->status_fase[indice_fase].duracao_atendimento;
    if(duracao > 0)
         printf("dur. atend %3d ", duracao);
    else printf("           %3s ", "-");
    
    int partida = utente->status_fase[indice_fase].tempo_partida;
    if(partida > 0)
         printf("part %3d ", partida);
    else printf("     %3s ", "-");
    
    int espera = calcular_tempo_espera_na_fila_fase(utente->status_fase[indice_fase]);
    if(espera >= 0)
         printf("espera %3d", espera);
    else printf("       %3s", "-");
    
    printf("\n                      Exa. ");
    for(int i = 0; i < 2 && utente->exames_medicos[i] != -1; i++)
         printf("%2d ", utente->exames_medicos[i]);
    
    printf("Esp. ");
    for(int i = 0; i < 2 && utente->especialidades_medicas_consultadas[i] != -1; i++)
         printf("%2d ", utente->especialidades_medicas_consultadas[i]);
    
    printf("\n");
}

void listar(struct fila *f){
    if(vazia(f)) {
        printf("Fila Vazia! \n");
        return;
    }

    node * aux = f->inicio;
    while(aux!=NULL){ // se aux for null é que chegou no fim da fila e nao tem valor
        imprimir_utente("", aux->utente, 0);
        aux= aux->prox;
    }
}

int calcular_tempo_espera_na_fila_fase(struct status_fase status_fase){
    return status_fase.tempo_inicio_atendimento-status_fase.tempo_chegada; 
}

int calcular_tempo_partida_do_utente_na_fila(struct status_fase status_fase){
    return status_fase.tempo_inicio_atendimento + status_fase.duracao_atendimento;
}

struct utente * pesquisar_todas_as_filas_e_remover_utente_maior_prioridade(struct fase * fase){
    for(int i=0; i < fase->total_filas; i++){
        if(!vazia(fase->filas[i])){
            return remover_inicio(fase->filas[i]);                 
        }   
    }
    return NULL;
}


/**
 * Inicialia qualquer vetor de inteiros com zero
 * @param vetor vetor que deseja-se inicializar
 * @param tamanho tamanho do vetor
 */
void inicializar_vetor(int vetor[], int tamanho, int valor_para_inicializar){
      for(int i=0; i<tamanho; i++){
          vetor[i]=valor_para_inicializar;
      }
}

struct utente * criar_e_inicializar_utente(int max_consulta_medicas_por_utente){
    struct utente *utente = (struct utente *)malloc(sizeof(struct utente));
    /* passando sizeof(int) para o malloc, ele vai alocar memória
     * para uma variável do tipo inteiro. O sizeof descobre quantos bytes
     * de memória o tipo indicado (neste caso int) precisa.
     * Usando apenas malloc(sizeof(int)) ele vai então alocar memória
     * para um único inteiro. Multiplicando o sizeof(int) por uma determinada
     * quantidade, vai fazer com que seja alocada memória para a quantidade
     * de números inteiros indicada, criando dinamicamente um vetor de inteiros.
     */
    utente->retorno_medicos= malloc(sizeof(int) * max_consulta_medicas_por_utente);
    utente->exames_medicos = malloc(sizeof(int) * max_consulta_medicas_por_utente);
    utente->especialidades_medicas_consultadas = malloc(sizeof(int) * max_consulta_medicas_por_utente);
    utente->total_atendimentos_concluidos = 0;
    
    /*O valor -1 indica que a posição nunca foi preenchida.
     No caso do vetor de exames, indica que o utente ainda não 
     passou pelo médico da posição indicada.
     No caso do vetor retorno_medicos indica que o utente ainda
     não foi atendido por aquele médico*/
    inicializar_vetor(utente->retorno_medicos, max_consulta_medicas_por_utente, -1);
    inicializar_vetor(utente->exames_medicos, max_consulta_medicas_por_utente, -1);
    inicializar_vetor(utente->especialidades_medicas_consultadas, max_consulta_medicas_por_utente, -1);
    return utente;
}
