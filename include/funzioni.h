#ifndef FUNZIONI_H
#define FUNZIONI_H

#include "strutture.h"

/* ============================================================================
 * PROTOTIPI DELLE FUNZIONI
 * ============================================================================
 */

/* Lista studenti */
Studente *cerca_Studente(NodoStudente *testa, char *matricola);
Studente  crea_studente(char *matricola, char *nome, char *corsoLaurea);
void      Registra_Studente(NodoStudente *lista, char *nome, char *matricola, char *corso);
void      aggiungi_in_coda(CodaAttesa *coda, NodoStudente *s, OrarioAula turno, char *data);

/* Gestione prenotazioni */
void effettua_prenotazione(NodoStudente *ListaStudenti, Turno *turnoAula, CodaAttesa *coda,
                           char *matricola, OrarioAula fascia_scelta, OrarioVirtuale ora_attuale);
void annulla_prenotazione(char *matricola, Turno *turnoAula, CodaAttesa *coda,
                          NodoStudente *ListaStudenti, OrarioVirtuale ora_attuale);
/* Check-in / Check-out */
void check_in_studenti(Turno *turnoAula, NodoStudente *ListaStudenti,
                       CodaAttesa *coda, char *matricola, OrarioVirtuale ora_attuale);
void check_out_studenti(NodoStudente *ListaStudenti, CodaAttesa *coda,
                        Turno *turnoAula, OrarioVirtuale orario_virtuale, char *matricola);

/*  Report */
void report(Turno *turnoAula, CodaAttesa *coda);
void visualizza_studenti_per_stato(NodoStudente *ListaStudenti, Turno *TurnoAula, CodaAttesa *coda);

/* Storico */
void leggi_storico(void);
void Scrivi_storico(NodoStudente *ListaStudenti, char *matricola, char *operazione, OrarioVirtuale ora);

/* Orario virtuale */
void aggiorna_orario_automatico(OrarioVirtuale *ora, time_t *ultimo_aggiornamento,
                                Turno *Turnoaula, CodaAttesa *coda, NodoStudente *s);


/* Inizializzazione e getter */
void       alloca_memoria_strutture(NodoStudente **ListaStudenti, Turno **Turnoaula, CodaAttesa **coda);
OrarioAula get_fascia_aula(Turno *turnoAula);
int        get_posti_occupati_totali(Turno *turnoAula);


#endif /* FUNZIONI_H */
