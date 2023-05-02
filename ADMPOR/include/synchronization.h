#ifndef SYNCHRONIZATION_H_GUARD
#define SYNCHRONIZATION_H_GUARD

#include <semaphore.h>
	
// Nomes usados na criacao dos semaforos
#define STR_SEM_MAIN_CLI_FULL 	"sem_main_cli_full"
#define STR_SEM_MAIN_CLI_EMPTY "sem_main_cli_empty"
#define STR_SEM_MAIN_CLI_MUTEX "sem_main_cli_mutex"

#define STR_SEM_CLI_INTERMED_FULL 	"sem_cli_intermed_full"
#define STR_SEM_CLI_INTERMED_EMPTY "sem_cli_intermed_empty"
#define STR_SEM_CLI_INTERMED_MUTEX "sem_cli_intermed_mutex"

#define STR_SEM_INTERMED_ENTERP_FULL	"sem_intermed_enterp_full"
#define STR_SEM_INTERMED_ENTERP_EMPTY 	"sem_intermed_enterp_empty"
#define STR_SEM_INTERMED_ENTERP_MUTEX 	"sem_intermed_enterp_mutex"

//estrutura de 3 semáforos utilizada no modelo produtor/consumidor
struct prodcons {
	sem_t *full, *empty, *mutex;
};

//estrutura que agrega informação de todos os semáforos necessários pelo socps
struct semaphores {
	struct prodcons *main_cli;	// semáforos para acesso ao buffer entre a main e clientes
	struct prodcons *cli_intermed; // semáforos para acesso ao buffer entre clientes e intermediários
	struct prodcons *intermed_enterp; 	// semáforos para acesso ao buffer entre intermediários e empresas
};

/* Função que cria um novo semáforo com nome name e valor inicial igual a
* value. Pode concatenar o resultado da função getuid() a name, para tornar
* o nome único para o processo.
*/
sem_t * semaphore_create(char* name, int value);

/* Função que destroi o semáforo passado em argumento.
*/
void semaphore_destroy(char* name, sem_t* semaphore);

/* Função que inicia o processo de produzir, fazendo sem_wait nos semáforos
* corretos da estrutura passada em argumento.
*/
void produce_begin(struct prodcons* pc);

/* Função que termina o processo de produzir, fazendo sem_post nos semáforos
* corretos da estrutura passada em argumento.
*/
void produce_end(struct prodcons* pc);

/* Função que inicia o processo de consumir, fazendo sem_wait nos semáforos
* corretos da estrutura passada em argumento.
*/
void consume_begin(struct prodcons* pc);

/* Função que termina o processo de consumir, fazendo sem_post nos semáforos
* corretos da estrutura passada em argumento.
*/
void consume_end(struct prodcons* pc);

/* Função que faz wait a um semáforo.
*/
void semaphore_mutex_lock(sem_t* sem);

/* Função que faz post a um semáforo.
*/
void semaphore_mutex_unlock(sem_t* sem);


#endif
