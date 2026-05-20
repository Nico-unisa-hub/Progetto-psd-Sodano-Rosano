typedef struct lista lista;

lista *elim_preontazione(lista *, int);
lista *chiudi_turno(lista *, int);
lista *inizializza_lista();
lista *nuova_aggiunta(lista *, char *, int, int);
void cancella_lista(lista *);
void visualizza_lista(lista *);
int ricevi_matricola(lista *);
int ricevi_turno(lista *);