#ifndef FUNZIONI_H
#define FUNZIONI_H

#include "strutture.h"

/* ============================================================================
 * PROTOTIPI DELLE FUNZIONI
 * ============================================================================
 */

/* Lista studenti */
void      Inserisci_Studente(NodoStudente *nodo, Studente s);
Studente *cerca_Studente(NodoStudente *testa, char *matricola);
Studente  crea_studente(char *matricola, char *nome, char *corsoLaurea);
void      Registra_Studente(NodoStudente *lista, char *nome, char *matricola, char *corso);
void      aggiungi_in_coda(CodaAttesa *coda, NodoStudente *s, OrarioAula turno, char *data);

/* Gestione prenotazioni */
void effettua_prenotazione(NodoStudente *ListaStudenti, Turno *turnoAula, CodaAttesa *coda,
                           char *matricola, OrarioAula fascia_scelta, OrarioVirtuale ora_attuale);
void annulla_prenotazione(char *matricola, Turno *turnoAula, CodaAttesa *coda,
                          NodoStudente *ListaStudenti, OrarioVirtuale ora_attuale);
void disponibilita_posti(OrarioAula *orario, Turno *turnoAula);

/* Check-in / Check-out */
void check_in_studenti(Turno *turnoAula, NodoStudente *ListaStudenti,
                       CodaAttesa *coda, char *matricola, OrarioVirtuale ora_attuale);
void check_out_studenti(NodoStudente *ListaStudenti, CodaAttesa *coda,
                        Turno *turnoAula, OrarioVirtuale orario_virtuale, char *matricola);

/* Stampa / Report */
void stampa_turno(OrarioAula orario, Turno turno);
void report(Turno *turnoAula, CodaAttesa *coda);
void visualizza_studenti_per_stato(NodoStudente *ListaStudenti, Turno *TurnoAula, CodaAttesa *coda);
void visualizza_situazione_corrente(Turno *turnoAula, CodaAttesa *coda);

/* Storico */
void leggi_storico(void);
void Scrivi_storico(NodoStudente *ListaStudenti, char *matricola, char *operazione, OrarioVirtuale ora);

/* Orario virtuale */
long orario_in_secondi(OrarioVirtuale o);
int  is_orario_valido(OrarioVirtuale adesso, OrarioAula orario);
void aggiorna_orario_automatico(OrarioVirtuale *ora, time_t *ultimo_aggiornamento,
                                Turno *Turnoaula, CodaAttesa *coda, NodoStudente *s);
void cambio_fascia_automatica(Turno *Turnoaula, NodoStudente *s,
                              CodaAttesa *coda, OrarioAula nuova_fascia);

/* Inizializzazione e getter */
void       inizializza_strutture(NodoStudente *ListaStudenti, Turno *turnoAula, CodaAttesa *coda);
void       alloca_memoria_strutture(NodoStudente **ListaStudenti, Turno **Turnoaula, CodaAttesa **coda);
OrarioAula get_fascia_aula(Turno *turnoAula);
int        get_posti_occupati_totali(Turno *turnoAula);
int        get_dimensione_coda(CodaAttesa *coda);
char      *get_data_aula(Turno *turnoAula);

#endif /* FUNZIONI_H */
