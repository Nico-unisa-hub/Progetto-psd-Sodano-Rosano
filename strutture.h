#ifndef STRUTTURE_H
#define STRUTTURE_H
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POSTI 100
#define MAX_NOME 16
#define MAX_MATRICOLA 12
#define MAX_CORSO_LAU 30

/* ============================================================================
 * STRUTTURE DATI PRINCIPALI
 * ============================================================================
 * Questa sezione definisce:
 * - studenti;
 * - posti aula;
 * - turni;
 * - orario virtuale;
 * - code di attesa.
 *
 * Le strutture rappresentano il modello logico
 * utilizzato dal sistema gestionale.
 * ============================================================================
 */

/* ===== ENUMERAZIONI ===== */
typedef enum { Prenotato, Libero, Occupato } StatoPosto;
typedef enum { Mattina, Pomeriggio, Sera } OrarioAula;

/* ===== STRUTTURE ===== */

typedef struct {
    int ora;
    int minuti;
    int secondi;
} OrarioVirtuale;

typedef struct {
    char Matricola[MAX_MATRICOLA];
    char Nome[MAX_NOME];
    char CorsoLaurea[MAX_CORSO_LAU];
} Studente;

typedef struct NodoStudente {
    Studente dato;
    struct NodoStudente *next;
    struct NodoStudente *prec;
} NodoStudente;

typedef struct {
    char  matricola[MAX_MATRICOLA];
    StatoPosto stato;
    OrarioVirtuale ora_prenotazione;
    int posti_occupati;
} Posti;

typedef struct {
    char  data[11];
    int posti_occupati;
    int presenti;
    int assenti;
    int esplusi_dalla_coda;
    int prenotazioni;
    int checkout;
    int accessi_effettivi[3];
    OrarioAula orario;
    Posti posti[MAX_POSTI];
} Turno;

/* Tag esplicito per permettere il self-reference nella struttura */
typedef struct CodaAttesa {
    char  data[11];
    Studente studente;
    char matricola[MAX_MATRICOLA];
    OrarioAula orario;
    struct CodaAttesa *head;
    struct CodaAttesa *tail;
    struct CodaAttesa *next;
    int dimensione;
} CodaAttesa;

#endif /* STRUTTURE_H */
