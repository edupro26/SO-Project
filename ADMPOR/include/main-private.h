#ifndef MAIN_PRIVATE_H_GUARD
#define MAIN_PRIVATE_H_GUARD

#include "main.h"

/* Verifica se n é um número*/
int isNumber(char n[]);

/* Função que verifica um comando op foi feito incorretamente*/
int check_request(char id1[], char id2[]);

/* Função que verifica um comando status foi feito incorretamente*/
int check_status(char id[]);

/* Função que imprime na consola o de ajuda para o utilizador.*/
void print_help();

/* Função que imprime na consola o estado de um pedido */
void print_status(int id, struct main_data* data);

#endif