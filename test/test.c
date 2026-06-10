#include "funzioni.h"
#include "strutture.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int main()
{
    char nome_tmp[MAX_NOME], matricola_tmp[MAX_MATRICOLA], corso_tmp[MAX_CORSO_LAU], divisore[2];
    FILE *fp;
    NodoStudente *ListaStudenti = NULL, *tmp;
    Turno *turnoAula = NULL;
    CodaAttesa *coda = NULL;
    OrarioVirtuale ora_attuale = {9, 0, 0};
    time_t ultimo_controllo = time(NULL);
    assert(ListaStudenti == NULL && turnoAula == NULL && coda == NULL);
    printf("Dichiarazione avvenuta correttamente!\n");
    alloca_memoria_strutture(&ListaStudenti, &turnoAula, &coda);
    assert(ListaStudenti != NULL && turnoAula != NULL && coda != NULL);
    printf("Iniziallizzazione avvenuta correttamente!\n");
    fopen_s(&fp, "oracolo.txt", "r");
    fscanf(fp, "%s", nome_tmp);
    fscanf(fp, "%s", matricola_tmp);
    fscanf(fp, "%s", corso_tmp);
    Registra_Studente(ListaStudenti, nome_tmp, matricola_tmp, corso_tmp);
    tmp = ListaStudenti;
    fscanf(fp, "%s", divisore);
    assert(strcmp(divisore, "/\n"));
    printf("File oracolo tipizzato correttamente!\n");
    fscanf(fp, "%s", nome_tmp);
    fscanf(fp, "%s", matricola_tmp);
    fscanf(fp, "%s", corso_tmp);
    Registra_Studente(ListaStudenti, nome_tmp, matricola_tmp, corso_tmp);
    assert(tmp == ListaStudenti);
    printf("Test d'inserimento doppia matricola passato cerettamente!\n");
    fscanf(fp, "%s", divisore);
    assert(strcmp(divisore, "/\n"));
    printf("File oracolo tipizzato correttamente!\n");
    fscanf(fp, "%s", nome_tmp);
    fscanf(fp, "%s", matricola_tmp);
    fscanf(fp, "%s", corso_tmp);
    Registra_Studente(ListaStudenti, nome_tmp, matricola_tmp, corso_tmp);
    assert(tmp == ListaStudenti);
    ListaStudenti = cerca_Studente(ListaStudenti, "A3K9L2MN7QR");
    assert(tmp != ListaStudenti);
    printf("Funzione di ricerca funziona correttamente!");
    effettua_prenotazione(ListaStudenti, turnoAula, coda, "A3K9L2MN7QR", Mattina, ora_attuale);
    assert(get_posti_occupati_totali != 0);
    check_in_studenti(turnoAula, ListaStudenti, coda, "A3K9L2MN7QR", ora_attuale);
    check_out_studenti(ListaStudenti, coda, turnoAula, ora_attuale, "A3K9L2MN7QR");
    int test=get_posti_occupati_totali(turnoAula);
    assert(test==0);
    effettua_prenotazione(ListaStudenti, turnoAula, coda, "A3K9L2MN7QR", Pomeriggio, ora_attuale);
    annulla_prenotazione("A3K9L2MN7QR", turnoAula, coda, ListaStudenti, ora_attuale);
    fclose(fp);
    return 0;
}