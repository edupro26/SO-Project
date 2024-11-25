#ifndef CLIENT_H_GUARD
#define CLIENT_H_GUARD

#include "main.h"

/* Função que recebe o nome do ficheiro de configuração e
* guarda o seu conteúdo na estrutura main_data
*/
void read_input_file(char* filename, struct main_data* data);

/* Função que lê cada linha do ficheiro de configuração e
* guarda o seu conteúdo na estrutura main_data
* (função auxiliar de read_input_file)
*/
void read_file(FILE *input, struct main_data* data);

/* Guarda o conteúdo de uma linha do ficheiro de configuração no 
* espaço correto da estruta main_data (função auxiliar de read_file)
*/
void save_to_main_data(char* buffer, int line, struct main_data* data);

/* Verifica se o ficheiro de configuração foi passado corretamente na consola
*/
int check_args(int argc, char* argv[]);


/* -------------- FUNÇÕES AUXILIARES -------------- */


/* Verifica se n é um número
*/
int isNumber(char* n);

/* Função que verifica se um comando op foi feito incorretamente
*/
int check_request(char* id1, char* id2);

/* Função que verifica se um comando status foi feito incorretamente
*/
int check_status(char* id);

/* Função que imprime na consola ajuda para o utilizador.
*/
void print_help();

/* Função que imprime na consola o estado de um pedido
*/
void print_status(int id, struct main_data* data);


#endif
