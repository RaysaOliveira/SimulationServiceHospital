/** 
 * Biblioteca criada para encapsular qual o gerator de números uniformes aleatórios
 * está sendo usado por todo o sistema. Se for preciso trocar o gerador,
 * basta trocar as chamadas existentes dentro das funções desta biblioteca pelas
 * chamadas do novo gerador desejado.
 *  
 * File:   estatisticas.h
 * Author: raysaoliveira
 *
 * Created on 2 de Janeiro de 2016, 20:52
 */

#ifndef RANDOM_H 
#define RANDOM_H

void inicializar_seed(long seed);

/***
 * @return um valor aleatoriamente distribuído entre 0 e 1
 */
float rnd();

#endif