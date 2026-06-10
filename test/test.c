#include "funzioni.h"
#include "strutture.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int main()
{
    // ---- DICHIARAZIONE VARIABILI TEMPORANEE E STRUTTURE ----
    char nome_tmp[MAX_NOME], matricola_tmp[MAX_MATRICOLA], corso_tmp[MAX_CORSO_LAU], divisore[2];
    FILE *fp;
    
    // Inizializzazione dei puntatori principali a NULL
    NodoStudente *ListaStudenti = NULL, *tmp;
    Turno *turnoAula = NULL;
    CodaAttesa *coda = NULL;
    
    // Impostazione di un orario virtuale di partenza (ore 09:00:00)
    OrarioVirtuale ora_attuale = {9, 0, 0};
    time_t ultimo_controllo = time(NULL);
    
    // VERIFICA 1: Controllo che i puntatori siano stati inizializzati correttamente a NULL
    assert(ListaStudenti == NULL && turnoAula == NULL && coda == NULL);
    printf("Dichiarazione avvenuta correttamente!\n");
    
    // Allocazione dinamica della memoria per le strutture dati principali
    alloca_memoria_strutture(&ListaStudenti, &turnoAula, &coda);
    
    // VERIFICA 2: Controllo che l'allocazione sia andata a buon fine (puntatori non NULL)
    assert(ListaStudenti != NULL && turnoAula != NULL && coda != NULL);
    printf("Iniziallizzazione avvenuta correttamente!\n");
    
    // ---- LETTURA DA FILE E TEST DI INSERIMENTO ----
    // Apertura del file "oracolo.txt" in modalità lettura sicura
    fopen_s(&fp, "oracolo.txt", "r");
    
    // Lettura dei dati del PRIMO studente dal file
    fscanf(fp, "%s", nome_tmp);
    fscanf(fp, "%s", matricola_tmp);
    fscanf(fp, "%s", corso_tmp);
    
    // Registrazione del primo studente nella lista
    Registra_Studente(ListaStudenti, nome_tmp, matricola_tmp, corso_tmp);
    
    // Salvo il puntatore della testa della lista per i controlli successivi
    tmp = ListaStudenti;
    
    // Lettura del carattere divisore (es. "/") per verificare la formattazione del file
    fscanf(fp, "%s", divisore);
    assert(strcmp(divisore, "/\n")); // <-- NOTA: Vedi analisi bug sotto
    printf("File oracolo tipizzato correttamente!\n");
    
    // Lettura e registrazione del SECONDO studente
    fscanf(fp, "%s", nome_tmp);
    fscanf(fp, "%s", matricola_tmp);
    fscanf(fp, "%s", corso_tmp);
    Registra_Studente(ListaStudenti, nome_tmp, matricola_tmp, corso_tmp);
    
    // VERIFICA 3: Test inserimento doppia matricola. 
    // Controlla che la testa della lista non sia cambiata in modo imprevisto
    assert(tmp == ListaStudenti);
    printf("Test d'inserimento doppia matricola passato cerettamente!\n");
    
    // Lettura del secondo divisore
    fscanf(fp, "%s", divisore);
    assert(strcmp(divisore, "/\n"));
    printf("File oracolo tipizzato correttamente!\n");
    
    // Lettura e registrazione del TERZO studente
    fscanf(fp, "%s", nome_tmp);
    fscanf(fp, "%s", matricola_tmp);
    fscanf(fp, "%s", corso_tmp);
    Registra_Studente(ListaStudenti, nome_tmp, matricola_tmp, corso_tmp);
    assert(tmp == ListaStudenti);
    
    // ---- TEST DI RICERCA ----
    // Cerca uno studente specifico tramite matricola
    ListaStudenti = cerca_Studente(ListaStudenti, "A3K9L2MN7QR"); // <-- NOTA: Bug critico qui
    assert(tmp != ListaStudenti);
    printf("Funzione di ricerca funziona correttamente!");
    
    // ---- TEST FLUSSO PRENOTAZIONE, CHECK-IN E CHECK-OUT ----
    // Effettua una prenotazione per il turno della mattina
    effettua_prenotazione(ListaStudenti, turnoAula, coda, "A3K9L2MN7QR", Mattina, ora_attuale);
    
    // VERIFICA 4: Controllo che i posti occupati siano aumentati
    assert(get_posti_occupati_totali != 0); // <-- NOTA: Altro bug qui
    
    // Simulazione di Check-In (lo studente entra in aula)
    check_in_studenti(turnoAula, ListaStudenti, coda, "A3K9L2MN7QR", ora_attuale);
    
    // Simulazione di Check-Out (lo studente esce dall'aula)
    check_out_studenti(ListaStudenti, coda, turnoAula, ora_attuale, "A3K9L2MN7QR");
    
    // VERIFICA 5: Dopo il check-out, l'aula deve tornare vuota (0 posti occupati)
    int test = get_posti_occupati_totali(turnoAula);
    assert(test == 0);
    
    // ---- TEST ANNULLAMENTO ----
    // Prenota per il pomeriggio e subito dopo annulla la prenotazione
    effettua_prenotazione(ListaStudenti, turnoAula, coda, "A3K9L2MN7QR", Pomeriggio, ora_attuale);
    annulla_prenotazione("A3K9L2MN7QR", turnoAula, coda, ListaStudenti, ora_attuale);
    
    // Chiusura del file e termine del programma
    fclose(fp);
    return 0;
}