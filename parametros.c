#include <string.h>
#include "parametros.h"

/**
 * Tenta abrir o arquivo de parâmetros da simulação na pasta atual.
 * Se o arquivo não existir, tenta abrir ele em 3 pastas acima do nível
 * de hierarquia de diretórios (para procurar o arquivo na pasta
 * de código fonte do projeto)
 * @param nome_arquivo
 * @return 
 */
FILE * abrir_arquivo(char *nome_arquivo){
    char nome_arquivo_pasta_codigo_fonte[400];
    sprintf(nome_arquivo_pasta_codigo_fonte, "../../../%s", nome_arquivo);
    //tenta abrir o arquivo na pasta atual do exe
    FILE *arquivo = fopen(nome_arquivo, "r");
    //se não conseguiu abrir o arquivo na pasta atual, tenta na pasta do projeto
    if(arquivo == NULL)
        arquivo = fopen(nome_arquivo_pasta_codigo_fonte, "r");    
    
    return arquivo;
}

int strpos(char *haystack, char *needle)
{
   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack;
   return -1;   // Not found = -1.
}

void obter_valor_parametro_string(char *linha_arquivo, char *nome_parametro, char *valor_parametro){
    valor_parametro[0]='\0'; //apaga o conteúdo da string
    //verifica se a linha contém o parâmetro desejado
    if(strstr(linha_arquivo, nome_parametro)!=NULL){
        int idx_separador = strpos(linha_arquivo, "=");
        if(idx_separador!=-1){
            idx_separador++;
            strncpy(valor_parametro, linha_arquivo+idx_separador, strlen(linha_arquivo)-idx_separador);
        }
    }
}

float obter_valor_parametro_float(char *linha_arquivo, char *nome_parametro){
    char valor_parametro_string[400];
    obter_valor_parametro_string(linha_arquivo, nome_parametro, valor_parametro_string);
    if(strlen(valor_parametro_string) != 0){
        float valor_parametro_float = atof(valor_parametro_string);
        //printf("\tparâmetro %s: %.2f\n", nome_parametro, valor_parametro_float);
        return valor_parametro_float;
    }

    return -1;    
}

float obter_valor_parametro_indexado_float(char *linha_arquivo, char *nome_parametro, int indice_parametro){
    char novo_nome_parametro[400];
    sprintf(novo_nome_parametro, "%s%d", nome_parametro, indice_parametro);
    return obter_valor_parametro_float(linha_arquivo, novo_nome_parametro);
}

void carregar_parametros_arquivo(simulacao *sim,
        int total_filas_fase[TOTAL_FASES], 
        int total_servidores_fase[TOTAL_FASES],
        int tempo_max_atendimento_fase[TOTAL_FASES],
        float probabilidades_prioridades[TOTAL_PRIORIDADES], 
        float probabilidades_especialidade_medica[TOTAL_ESPECIALIDADES_MEDICAS]){    
    #define TAM_MAX_LINHA 400
    #define NOME_ARQUIVO "parametros-simulacao.txt"

    FILE *arquivo = abrir_arquivo(NOME_ARQUIVO);
    if(arquivo != NULL){
        printf("Carregando ficheiro %s com os parâmetros da simulação\n", NOME_ARQUIVO);
        char linha_arquivo[TAM_MAX_LINHA];
        while(fgets(linha_arquivo, TAM_MAX_LINHA-1, arquivo)!=NULL){
            float valor_parametro = obter_valor_parametro_float(linha_arquivo, "max_consulta_medicas_por_utente");
            if(valor_parametro > 0)
                sim->max_consulta_medicas_por_utente = valor_parametro;

            valor_parametro = obter_valor_parametro_float(linha_arquivo, "total_minutos_simulacao");
            if(valor_parametro > 0)
                sim->total_minutos_simulacao = valor_parametro;
            
            valor_parametro = obter_valor_parametro_float(linha_arquivo, "total_simulacoes");
            if(valor_parametro > 0)
                sim->total_simulacoes = valor_parametro;

            valor_parametro = obter_valor_parametro_float(linha_arquivo, "probabilidade_de_utente_consultar_com_segundo_medico");
            if(valor_parametro != -1)
                sim->probabilidade_de_utente_consultar_com_segundo_medico = valor_parametro;

            for(int i = 0; i < TOTAL_FASES; i++) {
                valor_parametro = obter_valor_parametro_indexado_float(linha_arquivo, "total_servidores_fase", i);
                if(valor_parametro > 0)
                    total_servidores_fase[i] = valor_parametro;
            }
            
            for(int i = 0; i < TOTAL_FASES; i++) {
                valor_parametro = obter_valor_parametro_indexado_float(linha_arquivo, "total_filas_fase", i);
                if(valor_parametro > 0)
                    total_filas_fase[i] = valor_parametro;
            }

            for(int i = 0; i < TOTAL_FASES; i++) {
                valor_parametro = obter_valor_parametro_indexado_float(linha_arquivo, "tempo_max_atendimento_fase", i);
                if(valor_parametro > 0)
                    tempo_max_atendimento_fase[i] = valor_parametro;
            }

            for(int i = 0; i < TOTAL_PRIORIDADES; i++) {
                valor_parametro = obter_valor_parametro_indexado_float(linha_arquivo, "probabilidades_prioridade", i);
                if(valor_parametro > 0)
                    probabilidades_prioridades[i] = valor_parametro;
            }
            
            for(int i = 0; i < TOTAL_ESPECIALIDADES_MEDICAS; i++) {
                valor_parametro = obter_valor_parametro_indexado_float(linha_arquivo, "probabilidades_especialidade_medica", i);
                if(valor_parametro > 0)
                    probabilidades_especialidade_medica[i] = valor_parametro;
            }
                        
            valor_parametro = obter_valor_parametro_float(linha_arquivo, "imprimir_dados_utentes_individuais");
            if(valor_parametro > 0)
                sim->imprimir_dados_utentes_individuais = valor_parametro;
            
        }
        fclose(arquivo);
    }
    else printf("O ficheiro %s com os parâmetros da simulação não foi encontrado. Os parâmetros default serão utilizados\n", NOME_ARQUIVO);
    printf("\n");
}
