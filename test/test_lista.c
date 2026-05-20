#include "lista_attesa.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void test_inizializza(){
    lista *elemento=NULL;
    elemento=inizializza_lista();
    assert(elemento!=NULL);
    printf("Test d'inizializzazione: PASSATO!\n");
    cancella_lista(elemento);
}

void test_inserimento(){
    int tmp_matricola;
    lista *elemento=NULL, *tmp;
    elemento=inizializza_lista();
    tmp=elemento;
    elemento=nuova_aggiunta(elemento, "Paolo", 1, 171798731);
    assert(elemento==tmp);
    tmp_matricola=ricevi_matricola(elemento);
    assert(171798731==tmp_matricola);
    printf("Test primo inserimento: PASSATO!\n");
    elemento=nuova_aggiunta(elemento, "Luca", 1, 2019171615);
    assert(elemento!=tmp);
    tmp_matricola=ricevi_matricola(elemento);
    assert(2019171615==tmp_matricola);
    printf("Test inserimento: PASSATO!\n");
    cancella_lista(elemento);
    printf("Test di cancellazione: PASSATO!\n");
    elemento=NULL;
    elemento=nuova_aggiunta(elemento, "Franco", 1, 1910060404);
    assert(elemento!=NULL);
    tmp_matricola=ricevi_matricola(elemento);
    assert(tmp_matricola==1910060404);
    printf("Test inserimento vuoto: PASSATO!\n");
    cancella_lista(elemento);
}

void test_visualizzazione(){
    lista *elemento=NULL;
    elemento=inizializza_lista();
    elemento=nuova_aggiunta(elemento, "Paolo", 1, 171798731);
    visualizza_lista(elemento);
    elemento=nuova_aggiunta(elemento, "Luca", 1, 2019171615);
    visualizza_lista(elemento);
    elemento=nuova_aggiunta(elemento, "Franco", 1, 1910060404);
    visualizza_lista(elemento);
    cancella_lista(elemento);
}

void test_chiudi_turno(){
    lista *elemento=NULL, *tmp;
    elemento=chiudi_turno(elemento, 1);
    assert(elemento==NULL);
    printf("Test di chiusuara a vuoto: PASSATO!\n");
    elemento=inizializza_lista();
    elemento=nuova_aggiunta(elemento, "Paolo", 1, 171798731);
    elemento=chiudi_turno(elemento, 1);
    assert(elemento==NULL);
    printf("Test di chiusuara a elemento singolo: PASSATO!\n");
    elemento=nuova_aggiunta(elemento, "Paolo", 1, 171798731);
    elemento=nuova_aggiunta(elemento, "Luca", 1, 2019171615);
    elemento=chiudi_turno(elemento, 1);
    assert(elemento==NULL);
    printf("Test di chiusuara a piu' elementi: PASSATO!\n");
    elemento=nuova_aggiunta(elemento, "Franco", 2, 1910060404);
    tmp=elemento;
    elemento=chiudi_turno(elemento, 1);
    assert(elemento==tmp);
    printf("Test di chiusuara a elemento assente: PASSATO!\n");
    elemento=nuova_aggiunta(elemento, "Luca", 1, 2019171615);
    elemento=nuova_aggiunta(elemento, "Paolo", 1, 171798731);
    elemento=nuova_aggiunta(elemento, "Franco", 2, 1910060404);
    tmp=elemento;
    elemento=chiudi_turno(elemento, 2);
    assert(elemento!=tmp);
    printf("Test di chiusura del turno: PASSATO!\n");
    cancella_lista(elemento);
}

void test_cancella_prenotazione(){
    lista *elemento=NULL, *tmp;
    elemento=elim_preontazione(elemento, 1614060301);
    assert(elemento==NULL);
    printf("Test lista vuota: PASSATO!\n");
    elemento=inizializza_lista();
    elemento=nuova_aggiunta(elemento, "Paolo", 1, 171798731);
    elemento=elim_preontazione(elemento, 171798731);
    assert(elemento==NULL);
    printf("Test singolo elemento: PASSATO!\n");
    elemento=nuova_aggiunta(elemento, "Paolo", 1, 171798731);
    elemento=nuova_aggiunta(elemento, "Luca", 1, 2019171615);
    tmp=elemento;
    elemento=elim_preontazione(elemento, 171798731);
    assert(elemento==tmp);
    printf("Test elemento presente: PASSATO!\n");
    elemento=chiudi_turno(elemento, 1);
    elemento=nuova_aggiunta(elemento, "Luca", 1, 2019171615);
    elemento=nuova_aggiunta(elemento, "Paolo", 1, 171798731);
    elemento=nuova_aggiunta(elemento, "Franco", 2, 1910060404);
    tmp=elemento;
    elemento=elim_preontazione(elemento, 1614060301);
    assert(elemento!=tmp);
    printf("Test di annullamento della prenotazione: PASSATO!\n");
    cancella_lista(elemento);
}