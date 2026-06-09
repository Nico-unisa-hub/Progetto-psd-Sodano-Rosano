/*
 * ============================================================================
 * FILE: funzioni.c
 * DESCRIZIONE: Implementazione delle funzioni del sistema di gestione
 *              dell'aula studio. Include la gestione della lista studenti,
 *              prenotazioni, check-in/check-out, coda d'attesa, storico
 *              accessi e orario virtuale.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "strutture.h"
#include "funzioni.h"

/* =========================
   FUNZIONI LISTA STUDENTI
   ========================= */

/*
 * Funzione: cerca_Studente
 * ------------------------
 * Scorre la lista collegata degli studenti e restituisce un puntatore
 * al primo nodo la cui matricola corrisponde a quella cercata.
 * Usata da quasi tutte le funzioni del sistema per verificare
 * l'esistenza di uno studente prima di operare su di esso.
 *
 * Parametri:
 *   testa     : puntatore alla testa della lista degli studenti
 *   matricola : stringa contenente la matricola da cercare
 *
 * Pre-condizione:
 *   matricola deve essere una stringa valida e terminata con '\0'.
 *
 * Post-condizione:
 *   Ritorna il puntatore alla struttura Studente corrispondente,
 *   oppure NULL se la matricola non e' presente nella lista.
 *
 * Ritorna:
 *   Puntatore allo Studente trovato, NULL se assente.
 */
Studente *cerca_Studente(NodoStudente *testa, char *matricola)
{
    while (testa != NULL)
    {
        if (strcmp(testa->dato.Matricola, matricola) == 0)
            return &testa->dato;
        testa = testa->next;
    }
    return NULL;
}

/*
 * Funzione: Inserisci_Studente
 * ----------------------------
 * Aggiunge un nuovo nodo in coda alla lista degli studenti, previa
 * verifica che la matricola non sia gia' presente (niente duplicati).
 * ATTENZIONE: il parametro "nodo" e' passato per valore, quindi
 * le modifiche alla testa non si propagano al chiamante; la gestione
 * della testa e' demandata all'esterno tramite puntatore doppio.
 *
 * Parametri:
 *   nodo : puntatore al primo nodo della lista (testa)
 *   s    : struttura Studente da inserire
 */
void Inserisci_Studente(NodoStudente *nodo, Studente s)
{
    if (cerca_Studente(nodo, s.Matricola) != NULL)
    {
        printf("Lo studente con matricola %s e' gia' presente nella lista.\n", s.Matricola);
        return;
    }

    NodoStudente *nodo_temp = (NodoStudente *)malloc(sizeof(NodoStudente));
    if (nodo_temp == NULL)
    {
        fprintf(stderr,"Errore allocazione memoria\n");
        exit(1);
    }
    nodo_temp->dato = s;
    nodo_temp->next = NULL;
    nodo_temp->prec = NULL;

    if (nodo == NULL)
    {
        /*
         * Lista vuota: il nuovo nodo diventa la testa.
         * La propagazione al chiamante e' gestita esternamente
         * con un puntatore doppio; qui aggiorniamo solo la struttura interna.
         */
        nodo = nodo_temp;
    }
    else
    {
        /* Scorre fino all'ultimo nodo per inserire in coda */
        NodoStudente *temp = nodo;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = nodo_temp;
        nodo_temp->prec = temp;
    }

    printf("Studente aggiunto alla lista degli studenti.\n");
}

/*
 * Funzione: crea_studente
 * -----------------------
 * Alloca e popola una struttura Studente con i dati forniti.
 * Utilizza strncpy con terminatore esplicito per evitare
 * buffer overflow su stringhe troppo lunghe.
 *
 * Parametri:
 *   matricola    : codice identificativo dello studente
 *   nome         : nome e cognome dello studente
 *   corsoLaurea  : nome del corso di laurea
 *
 * Ritorna:
 *   Struttura Studente inizializzata per valore.
 */
Studente crea_studente(char *matricola, char *nome, char *corsoLaurea)
{
    Studente studente;
    strncpy(studente.Matricola, matricola, MAX_MATRICOLA - 1);
    studente.Matricola[MAX_MATRICOLA - 1] = '\0';
    strncpy(studente.Nome, nome, MAX_NOME - 1);
    studente.Nome[MAX_NOME - 1] = '\0';
    strncpy(studente.CorsoLaurea, corsoLaurea, MAX_CORSO_LAU - 1);
    studente.CorsoLaurea[MAX_CORSO_LAU - 1] = '\0';
    return studente;
}

/*
 * Funzione: Registra_Studente
 * ---------------------------
 * Punto di ingresso pubblico per la registrazione di un nuovo studente.
 * Verifica duplicati, crea la struttura, la inserisce nella lista
 * e registra l'evento nello storico con orario virtuale azzerato.
 *
 * Parametri:
 *   lista     : lista degli studenti gia' registrati
 *   nome      : nome del nuovo studente
 *   matricola : matricola univoca del nuovo studente
 *   corso     : corso di laurea del nuovo studente
 */
void Registra_Studente(NodoStudente *lista, char *nome, char *matricola, char *corso)
{
    if (cerca_Studente(lista, matricola) != NULL)
    {
        printf("Lo studente e' gia' registrato.\n");
        return;
    }
    Inserisci_Studente(lista, crea_studente(matricola, nome, corso));

    /* Timestamp virtuale a zero perche' la registrazione avviene all'avvio */
    OrarioVirtuale ora_zero = {0, 0, 0};
    Scrivi_storico(lista, matricola, "REGISTRAZIONE", ora_zero);
}

/* =========================
   GESTIONE PRENOTAZIONI
   ========================= */

/*
 * Funzione: aggiungi_in_coda
 * --------------------------
 * Aggiunge un nuovo elemento alla coda d'attesa dell'aula studio.
 * Gestisce correttamente sia il caso di coda vuota (aggiornando
 * head e tail) sia l'inserimento in fondo a una coda esistente.
 * NOTA: il campo "data" e' un array di char dentro la struttura,
 * non un char**; la copia avviene con strncpy (max 10 caratteri).
 *
 * Parametri:
 *   coda  : puntatore alla struttura CodaAttesa da aggiornare
 *   s     : nodo della lista studenti del soggetto da accodare
 *   turno : fascia oraria per cui si prenota (Mattina/Pomeriggio/Sera)
 *   data  : stringa della data nel formato "DD/MM/YYYY"
 */
void aggiungi_in_coda(CodaAttesa *coda, NodoStudente *s, OrarioAula turno, char *data)
{
    if (coda == NULL) return;

    CodaAttesa *temp = (CodaAttesa *)malloc(sizeof(CodaAttesa));
    if (temp == NULL)
    {
        fprintf(stderr,"[ERRORE] mancata allocazione memoria\n");
        exit(1);
    }

    temp->studente = s->dato;
    temp->orario   = turno;
    strncpy(temp->data, data, 10);
    temp->data[10] = '\0';
    temp->next     = NULL;
    temp->head     = NULL;
    temp->tail     = NULL;
    temp->dimensione = 0;

    if (coda->head == NULL)
    {
        /* Primo elemento: head e tail coincidono */
        coda->head = temp;
        coda->tail = temp;
    }
    else
    {
        /* Inserimento in coda: aggiorna il vecchio tail */
        coda->tail->next = temp;
        coda->tail = temp;
    }

    coda->dimensione++;

    printf("[CODA] Studente %s aggiunto alla lista d'attesa (Posizione: %d).\n",
           s->dato.Matricola, coda->dimensione);
}

/*
 * Funzione: effettua_prenotazione
 * --------------------------------
 * Gestisce l'intera logica di prenotazione di un posto in aula.
 * I casi gestiti sono, nell'ordine:
 *   1. Matricola non registrata -> errore.
 *   2. Fascia gia' conclusa     -> errore.
 *   3. Prenotazione gia' presente su posto fisico -> avviso.
 *   4. Gia' in coda per la stessa fascia           -> avviso.
 *   5. Fascia futura: inserimento in lista d'attesa anticipata.
 *   6. Fascia corrente con posto libero: prenotazione immediata.
 *   7. Fascia corrente senza posti: inserimento in coda.
 *
 * Parametri:
 *   ListaStudenti  : lista di tutti gli studenti registrati
 *   turnoaAula     : stato corrente del turno (posti, orario, contatori)
 *   coda           : coda d'attesa dell'aula
 *   matricola      : matricola dello studente che prenota
 *   fascia_scelta  : fascia oraria desiderata
 *   ora_attuale    : timestamp virtuale per lo storico
 */
void effettua_prenotazione(NodoStudente *ListaStudenti, Turno *turnoaAula,
                           CodaAttesa *coda, char *matricola,
                           OrarioAula fascia_scelta, OrarioVirtuale ora_attuale)
{
    int i;

    if (cerca_Studente(ListaStudenti, matricola) == NULL)
    {
        fprintf(stderr,"[ERRORE] Matricola %s non registrata. Impossibile prenotare.\n", matricola);
        return;
    }

    if (fascia_scelta < turnoaAula->orario)
    {
        fprintf(stderr,"[ERRORE] La fascia scelta e' gia' conclusa.\n");
        return;
    }

    /* Verifica se lo studente occupa gia' un posto fisico */
    for (i = 0; i < MAX_POSTI; i++)
    {
        if (turnoaAula->posti[i].stato != Libero &&
            strcmp(turnoaAula->posti[i].matricola, matricola) == 0)
        {
            printf("[AVVISO] Hai gia' una prenotazione al posto %d.\n", i + 1);
            return;
        }
    }

    /* Verifica se lo studente e' gia' in coda per la stessa fascia */
    {
        CodaAttesa *curr = coda->head;
        while (curr != NULL)
        {
            if (strcmp(curr->studente.Matricola, matricola) == 0 &&
                curr->orario == fascia_scelta)
            {
                printf("[AVVISO] Sei gia' in lista d'attesa per questa fascia.\n");
                return;
            }
            curr = curr->next;
        }
    }

    /* Prenotazione anticipata: la fascia non e' ancora iniziata */
    if (fascia_scelta > turnoaAula->orario)
    {
        printf("[PRENOTAZIONE ANTICIPATA] Verrai inserito in lista d'attesa.\n");
        aggiungi_in_coda(coda, ListaStudenti, fascia_scelta, turnoaAula->data);
        turnoaAula->prenotazioni++;
        Scrivi_storico(ListaStudenti, matricola,
                       (fascia_scelta == Mattina ? "PRENOTAZIONE ANTICIPATA [MATTINA]" :
                        fascia_scelta == Pomeriggio ? "PRENOTAZIONE ANTICIPATA [POMERIGGIO]" :
                        "PRENOTAZIONE ANTICIPATA [SERA]"), ora_attuale);
        return;
    }

    /* Prenotazione per la fascia corrente */
    {
        int posti_liberi = 0;
        for (i = 0; i < MAX_POSTI; i++)
            if (turnoaAula->posti[i].stato == Libero) posti_liberi++;

        if (posti_liberi > 0)
        {
            /* Assegna il primo posto libero trovato */
            for (i = 0; i < MAX_POSTI; i++)
            {
                if (turnoaAula->posti[i].stato == Libero)
                {
                    turnoaAula->posti[i].stato = Prenotato;
                    strncpy(turnoaAula->posti[i].matricola, matricola, MAX_MATRICOLA - 1);
                    turnoaAula->posti[i].matricola[MAX_MATRICOLA - 1] = '\0';
                    turnoaAula->posti[i].ora_prenotazione = ora_attuale;
                    turnoaAula->posti_occupati++;
                    turnoaAula->prenotazioni++;

                    printf("[SUCCESSO] Posto %d prenotato alle %02d:%02d:%02d.\n",
                           i + 1, ora_attuale.ora, ora_attuale.minuti, ora_attuale.secondi);
                    Scrivi_storico(ListaStudenti, matricola,
                                   (fascia_scelta == Mattina ? "PRENOTAZIONE [MATTINA]" :
                                    fascia_scelta == Pomeriggio ? "PRENOTAZIONE [POMERIGGIO]" :
                                    "PRENOTAZIONE [SERA]"), ora_attuale);
                    return;
                }
            }
        }
        else
        {
            /* Nessun posto libero: lo studente viene inserito in coda */
            printf("[AULA PIENA] Ti inserisco in coda...\n");
            aggiungi_in_coda(coda, ListaStudenti, fascia_scelta, turnoaAula->data);
            turnoaAula->prenotazioni++;
            Scrivi_storico(ListaStudenti, matricola,
                           (fascia_scelta == Mattina ? "PRENOTAZIONE IN CODA [MATTINA]" :
                            fascia_scelta == Pomeriggio ? "PRENOTAZIONE IN CODA [POMERIGGIO]" :
                            "PRENOTAZIONE IN CODA [SERA]"), ora_attuale);
        }
    }
}

/* =========================
   CHECK-IN
   ========================= */

/*
 * Funzione: check_in_studenti
 * ----------------------------
 * Gestisce l'ingresso fisico di uno studente in aula.
 * I casi gestiti, nell'ordine di priorita', sono:
 *   1. Studente gia' seduto (Occupato)   -> segnalazione e uscita.
 *   2. Studente con prenotazione (Prenotato) -> stato aggiornato a Occupato.
 *   3. Studente in coda per la fascia corrente -> estrazione dalla coda
 *      e assegnazione del primo posto libero; se l'aula e' piena
 *      viene reinserito in coda.
 *   4. Studente in coda per una fascia futura  -> messaggio di attesa.
 *   5. Ingresso diretto senza prenotazione (solo se nessuno e' in coda
 *      e ci sono posti liberi) -> assegnazione immediata.
 *   6. Tutti gli altri casi -> inserimento in coda.
 *
 * Parametri:
 *   turnoAula    : stato corrente del turno
 *   ListaStudenti: lista degli studenti registrati
 *   coda         : coda d'attesa
 *   matricola    : matricola dello studente che effettua il check-in
 *   ora_attuale  : timestamp virtuale per lo storico
 */
void check_in_studenti(Turno *turnoAula, NodoStudente *ListaStudenti,
                       CodaAttesa *coda, char *matricola, OrarioVirtuale ora_attuale)
{
    int i;

    /* --- Caso 1 & 2: studente con prenotazione su posto fisico --- */
    for (i = 0; i < MAX_POSTI; i++)
    {
        if (turnoAula->posti[i].stato != Libero &&
            strcmp(turnoAula->posti[i].matricola, matricola) == 0)
        {
            if (turnoAula->posti[i].stato == Occupato)
            {
                printf("Lo studente %s risulta gia' seduto al posto %d.\n", matricola, i + 1);
                return;
            }

            if (turnoAula->posti[i].stato == Prenotato)
            {
                turnoAula->posti[i].stato = Occupato;
                turnoAula->presenti++;
                turnoAula->accessi_effettivi[(int)turnoAula->orario]++;

                printf("Check-in effettuato per %s, posto %d.\n", matricola, i + 1);
                Scrivi_storico(ListaStudenti, matricola,
                               (turnoAula->orario == Mattina ? "CHECK-IN CON PRENOTAZIONE [MATTINA]" :
                                turnoAula->orario == Pomeriggio ? "CHECK-IN CON PRENOTAZIONE [POMERIGGIO]" :
                                "CHECK-IN CON PRENOTAZIONE [SERA]"), ora_attuale);
                return;
            }
        }
    }

    /* --- Caso 3: studente in coda per la fascia corrente --- */
    {
        CodaAttesa *curr = coda->head;
        CodaAttesa *prec = NULL;

        while (curr != NULL)
        {
            if (strcmp(curr->studente.Matricola, matricola) == 0 &&
                curr->orario == turnoAula->orario)
            {
                /* Rimozione del nodo dalla coda mantenendo la coerenza dei puntatori */
                if (prec == NULL)
                    coda->head = curr->next;
                else
                    prec->next = curr->next;
                if (coda->tail == curr)
                    coda->tail = prec;
                coda->dimensione--;

                /* Assegnazione del primo posto libero disponibile */
                for (i = 0; i < MAX_POSTI; i++)
                {
                    if (turnoAula->posti[i].stato == Libero)
                    {
                        turnoAula->posti[i].stato = Occupato;
                        strncpy(turnoAula->posti[i].matricola, matricola, MAX_MATRICOLA - 1);
                        turnoAula->posti[i].matricola[MAX_MATRICOLA - 1] = '\0';
                        turnoAula->posti[i].ora_prenotazione = ora_attuale;
                        turnoAula->posti_occupati++;
                        turnoAula->presenti++;
                        turnoAula->accessi_effettivi[(int)turnoAula->orario]++;
                        printf("[CHECK-IN] Posto %d assegnato da coda a %s.\n", i + 1, matricola);
                        Scrivi_storico(ListaStudenti, matricola,
                                       (turnoAula->orario == Mattina ? "CHECK-IN DA PRENOTAZIONE ANTICIPATA [MATTINA]" :
                                        turnoAula->orario == Pomeriggio ? "CHECK-IN DA PRENOTAZIONE ANTICIPATA [POMERIGGIO]" :
                                        "CHECK-IN DA PRENOTAZIONE ANTICIPATA [SERA]"), ora_attuale);
                        free(curr);
                        return;
                    }
                }

                /* Nessun posto libero: lo studente torna in coda */
                aggiungi_in_coda(coda, ListaStudenti, turnoAula->orario, turnoAula->data);
                printf("L'aula e' piena. Rimesso in coda.\n");
                free(curr);
                return;
            }
            prec = curr;
            curr = curr->next;
        }
    }

    /* --- Caso 4: studente in attesa per una fascia futura --- */
    {
        CodaAttesa *temp_curr = coda->head;
        while (temp_curr != NULL)
        {
            if (strcmp(temp_curr->studente.Matricola, matricola) == 0 &&
                temp_curr->orario > turnoAula->orario)
            {
                printf("[Sei in attesa per una fascia futura. Attendi il cambio turno.]\n");
                return;
            }
            temp_curr = temp_curr->next;
        }
    }

    /* --- Caso 5 & 6: ingresso senza prenotazione --- */
    {
        int posti_liberi = 0;
        for (i = 0; i < MAX_POSTI; i++)
            if (turnoAula->posti[i].stato == Libero) posti_liberi++;

        /*
         * L'ingresso diretto e' consentito solo se non ci sono persone
         * in coda; altrimenti sarebbe ingiusto saltare la fila.
         */
        if (posti_liberi > 0 && coda->dimensione == 0)
        {
            for (i = 0; i < MAX_POSTI; i++)
            {
                if (turnoAula->posti[i].stato == Libero)
                {
                    turnoAula->posti[i].stato = Occupato;
                    strncpy(turnoAula->posti[i].matricola, matricola, MAX_MATRICOLA - 1);
                    turnoAula->posti[i].matricola[MAX_MATRICOLA - 1] = '\0';
                    turnoAula->posti_occupati++;
                    turnoAula->presenti++;
                    turnoAula->accessi_effettivi[(int)turnoAula->orario]++;

                    printf("[SUCCESSO] Posto libero %d assegnato direttamente.\n", i + 1);
                    Scrivi_storico(ListaStudenti, matricola,
                                   (turnoAula->orario == Mattina ? "CHECK-IN SENZA PRENOTAZIONE [MATTINA]" :
                                    turnoAula->orario == Pomeriggio ? "CHECK-IN SENZA PRENOTAZIONE [POMERIGGIO]" :
                                    "CHECK-IN SENZA PRENOTAZIONE [SERA]"), ora_attuale);
                    return;
                }
            }
        }
        else
        {
            if (coda->dimensione > 0 && turnoAula->posti_occupati < MAX_POSTI)
                printf("[INFO] Ci sono persone in attesa prima di te. Ti aggiungo alla coda.\n");
            else
                printf("[AULA PIENA] Nessun posto disponibile.\n");

            aggiungi_in_coda(coda, ListaStudenti, turnoAula->orario, turnoAula->data);
            Scrivi_storico(ListaStudenti, matricola,
                           (turnoAula->orario == Mattina ? "INSERIMENTO IN LISTA ATTESA [MATTINA]" :
                            turnoAula->orario == Pomeriggio ? "INSERIMENTO IN LISTA ATTESA [POMERIGGIO]" :
                            "INSERIMENTO IN LISTA ATTESA [SERA]"), ora_attuale);
        }
    }
}

/* =========================
   ANNULLA PRENOTAZIONE
   ========================= */

/*
 * Funzione: annulla_prenotazione
 * -------------------------------
 * Cancella la prenotazione di uno studente, gestendo due scenari:
 *   A) Posto fisico in stato Prenotato: il posto viene liberato e,
 *      se esiste qualcuno in coda per la stessa fascia, subentra
 *      automaticamente (primo in coda per la stessa OrarioAula).
 *   B) Prenotazione anticipata in coda: il nodo viene estratto
 *      e liberato senza subentro, poiche' non occupa ancora un posto.
 *
 * Parametri:
 *   matricola    : matricola dello studente che annulla
 *   turnoAula    : stato corrente del turno
 *   coda         : coda d'attesa
 *   ListaStudenti: lista degli studenti per aggiornare lo storico
 *   ora_attuale  : timestamp virtuale per lo storico
 */
void annulla_prenotazione(char *matricola, Turno *turnoAula, CodaAttesa *coda,
                          NodoStudente *ListaStudenti, OrarioVirtuale ora_attuale)
{
    int i;
    if (turnoAula == NULL || coda == NULL || matricola == NULL) return;

    /* --- Scenario A: prenotazione su posto fisico --- */
    for (i = 0; i < MAX_POSTI; i++)
    {
        if (strcmp(turnoAula->posti[i].matricola, matricola) == 0 &&
            turnoAula->posti[i].stato == Prenotato)
        {
            printf("[SISTEMA] Annullamento prenotazione per %s...\n", matricola);
            Scrivi_storico(ListaStudenti, matricola, "ANNULLAMENTO PRENOTAZIONE", ora_attuale);

            {
                CodaAttesa *curr = coda->head;
                CodaAttesa *prec = NULL;
                CodaAttesa *subentrante = NULL;

                /* Cerca il primo in coda per la stessa fascia oraria */
                while (curr != NULL)
                {
                    if (curr->orario == turnoAula->orario)
                    {
                        subentrante = curr;
                        if (prec == NULL) 
                        {
                            coda->head = curr->next;
                        }
                        else 
                        {
                            prec->next = curr->next;
                        }
                        if (coda->tail == curr)
                        {
                            coda->tail = prec; 
                        }
                        coda->dimensione--;
                        break;
                    }
                    prec = curr;
                    curr = curr->next;
                }

                if (subentrante != NULL)
                {
                    /* Assegna il posto liberato al primo in coda */
                    strncpy(turnoAula->posti[i].matricola, subentrante->studente.Matricola, MAX_MATRICOLA - 1);
                    turnoAula->posti[i].matricola[MAX_MATRICOLA - 1] = '\0';
                    turnoAula->posti[i].stato = Prenotato;
                    turnoAula->posti[i].ora_prenotazione = ora_attuale;

                    printf("[SUBENTRO] Posto %d assegnato a %s.\n", i + 1, subentrante->studente.Matricola);
                    Scrivi_storico(ListaStudenti, subentrante->studente.Matricola,
                                   "SUBENTRO DA CODA (DOPO ANNULLAMENTO)", ora_attuale);
                    free(subentrante);
                }
                else
                {
                    /* Nessun subentrante: il posto torna libero */
                    turnoAula->posti[i].stato = Libero;
                    strcpy(turnoAula->posti[i].matricola, "");
                    if (turnoAula->posti_occupati > 0) turnoAula->posti_occupati--;
                    printf("[SISTEMA] Posto %d liberato.\n", i + 1);
                }
            }
            return;
        }
    }

    /* --- Scenario B: prenotazione anticipata ancora in coda --- */
    {
        CodaAttesa *curr = coda->head;
        CodaAttesa *prec = NULL;

        while (curr != NULL)
        {
            if (strcmp(curr->studente.Matricola, matricola) == 0)
            {
                if (prec == NULL) coda->head = curr->next;
                else              prec->next  = curr->next;
                if (coda->tail == curr) coda->tail = prec;
                coda->dimensione--;

                printf("[SISTEMA] Prenotazione anticipata per fascia %s annullata per %s.\n",
                       (curr->orario == Mattina ? "MATTINA" :
                        curr->orario == Pomeriggio ? "POMERIGGIO" : "SERA"), matricola);
                Scrivi_storico(ListaStudenti, matricola,
                               "ANNULLAMENTO PRENOTAZIONE ANTICIPATA", ora_attuale);
                free(curr);
                if (turnoAula->prenotazioni > 0) turnoAula->prenotazioni--;
                return;
            }
            prec = curr;
            curr = curr->next;
        }
    }

    fprintf(stderr,"[ERRORE] Nessuna prenotazione trovata per %s.\n", matricola);
}

/* =========================
   VISUALIZZAZIONE
   ========================= */

/*
 * Funzione: visualizza_studenti_per_stato
 * ----------------------------------------
 * Stampa due sezioni distinte:
 *   1. Studenti presenti in aula (Occupato) o con prenotazione
 *      attiva (Prenotato), con dettagli anagrafici se disponibili.
 *   2. Studenti in lista d'attesa, con fascia oraria di riferimento.
 * Usata per il pannello di controllo del gestore dell'aula.
 *
 * Parametri:
 *   ListaStudenti : lista degli studenti registrati (per l'anagrafica)
 *   TurnoAula     : stato corrente dei posti
 *   coda          : coda d'attesa corrente
 */
void visualizza_studenti_per_stato(NodoStudente *ListaStudenti, Turno *TurnoAula, CodaAttesa *coda)
{
    int i, trovati;
    Studente *s;

    printf("\n+==========================================+\n");
    printf("|       ELENCO STUDENTI PER STATO         |\n");
    printf("+==========================================+\n");

    printf("\n[Presenti e Prenotati]:\n");
    trovati = 0;
    for (i = 0; i < MAX_POSTI; i++)
    {
        if (TurnoAula->posti[i].stato == Occupato || TurnoAula->posti[i].stato == Prenotato)
        {
            trovati++;
            s = cerca_Studente(ListaStudenti, TurnoAula->posti[i].matricola);
            if (s != NULL)
                printf("  [Posto %3d] %-12s | %-30s | %s\n",
                       i + 1, TurnoAula->posti[i].matricola, s->Nome, s->CorsoLaurea);
            else
                printf("  [Posto %3d] %-12s | (anagrafica non disponibile)\n",
                       i + 1, TurnoAula->posti[i].matricola);
        }
    }
    if (trovati == 0) printf("  Nessuno presente.\n");

    printf("\nNella Lista d'Attesa (%d in coda):\n", coda->dimensione);
    {
        CodaAttesa *curr = coda->head;
        int pos = 1;
        if (curr == NULL) printf("  Nessuno in coda.\n");
        while (curr != NULL)
        {
            s = cerca_Studente(ListaStudenti, curr->studente.Matricola);
            if (s != NULL)
                printf("  [#%d] %-12s | %-30s | %s  (Fascia: %s)\n",
                       pos, curr->studente.Matricola, s->Nome, s->CorsoLaurea,
                       (curr->orario == Mattina ? "MATTINA" :
                        curr->orario == Pomeriggio ? "POMERIGGIO" : "SERA"));
            else
                printf("  [#%d] %-12s | (anagrafica non disponibile)\n",
                       pos, curr->studente.Matricola);
            curr = curr->next;
            pos++;
        }
    }
}

/*
 * Funzione: visualizza_situazione_corrente
 * -----------------------------------------
 * Snapshot rapido dello stato dell'aula: mostra i posti occupati
 * e prenotati con la rispettiva matricola, poi elenca gli studenti
 * in coda d'attesa. Piu' sintetica di visualizza_studenti_per_stato,
 * non cerca l'anagrafica completa.
 *
 * Parametri:
 *   turnoAula : stato corrente del turno
 *   coda      : coda d'attesa
 */
void visualizza_situazione_corrente(Turno *turnoAula, CodaAttesa *coda)
{
    int i, cont = 0;
    printf("\n--- STUDENTI ATTUALMENTE IN AULA ---\n");
    for (i = 0; i < MAX_POSTI; i++)
    {
        if (turnoAula->posti[i].stato == Occupato)
        {
            printf("Posto %d: %s [PRESENTE]\n", i + 1, turnoAula->posti[i].matricola);
            cont++;
        }
        else if (turnoAula->posti[i].stato == Prenotato)
        {
            printf("Posto %d: %s [PRENOTATO]\n", i + 1, turnoAula->posti[i].matricola);
            cont++;
        }
    }
    if (cont == 0) printf("L'aula e' attualmente vuota.\n");

    printf("\n--- STUDENTI IN CODA D'ATTESA ---\n");
    if (coda->head == NULL)
    {
        printf("Nessuno in attesa.\n");
    }
    else
    {
        CodaAttesa *temp = coda->head;
        while (temp != NULL)
        {
            printf("- Studente: %s\n", temp->studente.Matricola);
            temp = temp->next;
        }
    }
}

/* =========================
   CHECK-OUT
   ========================= */

/*
 * Funzione: check_out_studenti
 * -----------------------------
 * Registra l'uscita di uno studente dall'aula.
 * Dopo aver liberato il posto, controlla se c'e' qualcuno in coda
 * per la fascia corrente: in tal caso assegna automaticamente il posto
 * liberato allo studente in testa alla coda (subentro).
 * Se la coda e' vuota, il posto torna semplicemente libero.
 *
 * Parametri:
 *   ListaStudenti    : lista degli studenti (per lo storico)
 *   coda             : coda d'attesa
 *   turnoAula        : stato corrente del turno
 *   orario_virtuale  : timestamp virtuale per lo storico
 *   matricola        : matricola dello studente che esce
 */
void check_out_studenti(NodoStudente *ListaStudenti, CodaAttesa *coda,
                        Turno *turnoAula, OrarioVirtuale orario_virtuale, char *matricola)
{
    int i, trovato = 0;
    if (turnoAula == NULL || coda == NULL || matricola == NULL) return;

    for (i = 0; i < MAX_POSTI; i++)
    {
        if (turnoAula->posti[i].stato != Libero &&
            strcmp(turnoAula->posti[i].matricola, matricola) == 0)
        {
            printf("\n[CHECK-OUT] Studente %s uscito dal posto %d.\n", matricola, i + 1);
            Scrivi_storico(ListaStudenti, matricola, "CHECK-OUT (USCITA)", orario_virtuale);
            turnoAula->checkout++;
            trovato = 1;

            {
                CodaAttesa *curr = coda->head;
                CodaAttesa *prev = NULL;
                CodaAttesa *subentrante = NULL;

                /* Cerca il primo in coda per la fascia corrente */
                while (curr != NULL)
                {
                    if (curr->orario == turnoAula->orario)
                    {
                        subentrante = curr;
                        if (prev == NULL) coda->head = curr->next;
                        else              prev->next = curr->next;
                        if (coda->tail == curr) coda->tail = prev;
                        coda->dimensione--;
                        break;
                    }
                    prev = curr;
                    curr = curr->next;
                }

                if (subentrante != NULL)
                {
                    /* Prenota il posto appena liberato per il subentrante */
                    strncpy(turnoAula->posti[i].matricola, subentrante->studente.Matricola, MAX_MATRICOLA - 1);
                    turnoAula->posti[i].matricola[MAX_MATRICOLA - 1] = '\0';
                    turnoAula->posti[i].stato = Prenotato;
                    turnoAula->posti[i].ora_prenotazione = orario_virtuale;

                    printf("[SUBENTRO] Posto %d riservato per %s.\n",
                           i + 1, subentrante->studente.Matricola);
                    Scrivi_storico(ListaStudenti, subentrante->studente.Matricola,
                                   "SUBENTRO DALLA CODA (PRENOTATO)", orario_virtuale);
                    free(subentrante);
                }
                else
                {
                    /* Nessuno in coda: il posto torna libero */
                    turnoAula->posti[i].stato = Libero;
                    strcpy(turnoAula->posti[i].matricola, "");
                    if (turnoAula->posti_occupati > 0) turnoAula->posti_occupati--;
                    printf("[INFO] Posto %d ora libero.\n", i + 1);
                }
            }
            break;
        }
    }

    if (!trovato)
        printf("\n[ERRORE] La matricola %s non risulta presente in aula.\n", matricola);
}

/* =========================
   REPORT
   ========================= */

/*
 * Funzione: report
 * ----------------
 * Genera e stampa un riepilogo completo del turno corrente, articolato in:
 *   - Dati identificativi (data e fascia)
 *   - Contatori cumulativi del turno (prenotazioni, check-in, check-out,
 *     assenti, espulsi dalla coda)
 *   - Snapshot istantaneo (posti occupati, prenotati, liberi, coda)
 *   - Accessi effettivi suddivisi per fascia oraria
 *   - Percentuale di saturazione dell'aula
 * Al termine richiama leggi_storico() per mostrare lo storico testuale.
 *
 * Parametri:
 *   turnoAula : stato corrente del turno
 *   coda      : coda d'attesa
 */
void report(Turno *turnoAula, CodaAttesa *coda)
{
    int i;
    if (turnoAula == NULL || coda == NULL) return;

    int presenti_ora = 0, prenotati_ora = 0;
    for (i = 0; i < MAX_POSTI; i++)
    {
        if      (turnoAula->posti[i].stato == Occupato)  presenti_ora++;
        else if (turnoAula->posti[i].stato == Prenotato) prenotati_ora++;
    }

    /* Percentuale di occupazione rispetto alla capienza massima */
    float perc = (MAX_POSTI > 0) ?
        ((float)(presenti_ora + prenotati_ora) / MAX_POSTI) * 100.0f : 0.0f;

    printf("  Data   : %-10s\n", turnoAula->data);
    printf("  Fascia : %s\n",
           (turnoAula->orario == Mattina ? "MATTINA (09:00-13:00)" :
            turnoAula->orario == Pomeriggio ? "POMERIGGIO (14:00-18:00)" : "SERA (18:00-22:00)"));

    printf("\n--- RIEPILOGO TURNO CORRENTE --------------\n");
    printf("  Totale prenotazioni effettuate : %d\n",  turnoAula->prenotazioni);
    printf("  Accessi effettivi (check-in)   : %d\n",  turnoAula->presenti);
    printf("  Uscite registrate (check-out)  : %d\n",  turnoAula->checkout);
    printf("  Assenti (prenotati non arrivati): %d\n", turnoAula->assenti);
    printf("  Rimasti in attesa (espulsi)    : %d\n",  turnoAula->esplusi_dalla_coda);

    printf("\n--- SNAPSHOT ISTANTANEO -------------------\n");
    printf("  Presenti in aula ora           : %d\n",   presenti_ora);
    printf("  Prenotati non ancora arrivati  : %d\n",   prenotati_ora);
    printf("  Posti liberi                   : %d / %d\n",
           MAX_POSTI - presenti_ora - prenotati_ora, MAX_POSTI);
    printf("  In lista d'attesa ora          : %d\n",   coda->dimensione);

    printf("\n--- OCCUPAZIONE PER FASCIA ----------------\n");
    printf("  MATTINA    : %d accessi\n", turnoAula->accessi_effettivi[0]);
    printf("  POMERIGGIO : %d accessi\n", turnoAula->accessi_effettivi[1]);
    printf("  SERA       : %d accessi\n", turnoAula->accessi_effettivi[2]);

    printf("\n--- SATURAZIONE AULA ----------------------\n");
    printf("  %.1f%% occupata\n", perc);

    leggi_storico();
}

/* =========================
   STORICO
   ========================= */

/*
 * Funzione: leggi_storico
 * ------------------------
 * Apre il file "storico_accessi.txt" in modalita' lettura e stampa
 * riga per riga il suo contenuto su stdout. Il file viene creato e
 * aggiornato da Scrivi_storico() ad ogni operazione rilevante.
 * Se il file non esiste (prima esecuzione), stampa un messaggio informativo.
 */
void leggi_storico(void)
{
    FILE *file = fopen("storico_accessi.txt", "r");
    if (file == NULL)
    {
        printf("[INFO] Nessuno storico disponibile.\n");
        return;
    }
    char riga[256];
    printf("\n--- STORICO ACCESSI ---\n");
    while (fgets(riga, sizeof(riga), file) != NULL)
        printf("%s", riga);
    fclose(file);
}

/*
 * Funzione: Scrivi_storico
 * -------------------------
 * Aggiunge una riga di log al file "storico_accessi.txt" in modalita'
 * append. Ogni riga include: timestamp virtuale, matricola, nome, corso
 * e tipo di operazione. Se lo studente non e' trovato in lista, i campi
 * nome e corso vengono sostituiti con "N/D".
 * Chiamata da tutte le funzioni che modificano lo stato dell'aula.
 *
 * Parametri:
 *   ListaStudenti : lista per recuperare i dati anagrafici
 *   matricola     : matricola dello studente coinvolto
 *   operazione    : stringa descrittiva dell'azione (es. "CHECK-IN")
 *   ora           : timestamp virtuale al momento dell'evento
 */
void Scrivi_storico(NodoStudente *ListaStudenti, char *matricola,
                    char *operazione, OrarioVirtuale ora)
{
    FILE *fp = fopen("storico_accessi.txt", "a");
    if (fp == NULL)
    {
        fprintf(stderr, "[ERRORE] Impossibile aprire storico_accessi.txt.\n");
        return;
    }

    Studente *s = (ListaStudenti != NULL) ? cerca_Studente(ListaStudenti, matricola) : NULL;

    if (s != NULL)
        fprintf(fp, "[%02d:%02d:%02d] Matricola: %-12s | Nome: %-30s | Corso: %-30s | Azione: %s\n",
                ora.ora, ora.minuti, ora.secondi,
                matricola, s->Nome, s->CorsoLaurea, operazione);
    else
        fprintf(fp, "[%02d:%02d:%02d] Matricola: %-12s | Nome: N/D | Corso: N/D | Azione: %s\n",
                ora.ora, ora.minuti, ora.secondi, matricola, operazione);

    fclose(fp);
}

/* =========================
   ORARIO VIRTUALE
   ========================= */

/*
 * Funzione: orario_in_secondi
 * ----------------------------
 * Converte una struttura OrarioVirtuale (ore, minuti, secondi) in
 * un valore scalare in secondi dall'inizio della giornata.
 * Usata internamente da is_orario_valido per confronti rapidi.
 *
 * Parametri:
 *   o : struttura OrarioVirtuale da convertire
 *
 * Ritorna:
 *   Numero di secondi corrispondenti all'orario (long).
 */
long orario_in_secondi(OrarioVirtuale o)
{
    return (long)o.ora * 3600 + o.minuti * 60 + o.secondi;
}

/*
 * Funzione: is_orario_valido
 * ---------------------------
 * Verifica se l'orario virtuale corrente rientra nella finestra
 * temporale della fascia specificata:
 *   Mattina    : 09:00 - 13:00 (32400 - 46800 s)
 *   Pomeriggio : 14:00 - 18:00 (50400 - 64800 s)
 *   Sera       : 18:00 - 22:00 (64800 - 79200 s)
 *
 * Parametri:
 *   adesso  : orario virtuale attuale
 *   orario  : fascia da verificare
 *
 * Ritorna:
 *   1 se l'orario rientra nella fascia, 0 altrimenti.
 */
int is_orario_valido(OrarioVirtuale adesso, OrarioAula orario)
{
    long s = orario_in_secondi(adesso);
    if (orario == Mattina)    return (s >= 32400 && s <= 46800);
    if (orario == Pomeriggio) return (s >= 50400 && s <= 64800);
    if (orario == Sera)       return (s >= 64800 && s <= 79200);
    return 0;
}

/*
 * Funzione: cambio_fascia_automatica
 * ------------------------------------
 * Esegue la transizione da una fascia oraria alla successiva.
 * Le operazioni, nell'ordine, sono:
 *   1. Conteggio degli assenti (Prenotato non arrivati) e reset posti.
 *   2. Espulsione dalla coda degli studenti della fascia appena terminata.
 *   3. Reset di tutti i contatori del turno per la nuova fascia.
 *   4. Promozione automatica degli studenti in coda per la nuova fascia,
 *      assegnando loro un posto Prenotato fino a esaurimento dei posti.
 *
 * Parametri:
 *   Turnoaula   : stato del turno da aggiornare
 *   s           : lista degli studenti (per lo storico)
 *   coda        : coda d'attesa
 *   nuova_fascia: fascia oraria verso cui si transita
 */
void cambio_fascia_automatica(Turno *Turnoaula, NodoStudente *s,
                              CodaAttesa *coda, OrarioAula nuova_fascia)
{
    int i;
    OrarioVirtuale ora_zero = {0, 0, 0};

    printf("\n[SISTEMA] Cambio fascia: %s -> %s\n",
           (Turnoaula->orario == Mattina ? "MATTINA" :
            Turnoaula->orario == Pomeriggio ? "POMERIGGIO" : "SERA"),
           (nuova_fascia == Pomeriggio ? "POMERIGGIO" : "SERA"));

    /* 1. Conteggio assenti e reset di tutti i posti */
    for (i = 0; i < MAX_POSTI; i++)
    {
        if (Turnoaula->posti[i].stato == Prenotato)
        {
            Turnoaula->assenti++;
            Scrivi_storico(s, Turnoaula->posti[i].matricola, "ASSENTE (FINE TURNO)", ora_zero);
        }
        else if (Turnoaula->posti[i].stato == Occupato)
            Scrivi_storico(s, Turnoaula->posti[i].matricola, "CHECKOUT AUTOMATICO (FINE TURNO)", ora_zero);

        Turnoaula->posti[i].stato = Libero;
        strcpy(Turnoaula->posti[i].matricola, "");
    }

    /* 2. Rimozione selettiva dalla coda degli studenti della fascia scaduta */
    {
        int n_espulsi = 0, n_rimasti = 0;
        CodaAttesa *temp_curr = coda->head;
        CodaAttesa *temp_prec = NULL;

        while (temp_curr != NULL)
        {
            CodaAttesa *next = temp_curr->next;
            if (temp_curr->orario == Turnoaula->orario)
            {
                /* Studente in coda per la fascia terminata: va espulso */
                Turnoaula->esplusi_dalla_coda++;
                n_espulsi++;
                Scrivi_storico(s, temp_curr->studente.Matricola, "ESPULSO DA CODA (FINE TURNO)", ora_zero);
                if (temp_prec == NULL) coda->head = next;
                else                   temp_prec->next = next;
                if (coda->tail == temp_curr) coda->tail = temp_prec;
                coda->dimensione--;
                free(temp_curr);
            }
            else
            {
                /* Studente in coda per una fascia futura: rimane */
                n_rimasti++;
                temp_prec = temp_curr;
            }
            temp_curr = next;
        }
        printf("[CODA] %d rimossi, %d mantenuti.\n", n_espulsi, n_rimasti);
    }

    /* 3. Reset contatori per il nuovo turno */
    Turnoaula->posti_occupati        = 0;
    Turnoaula->prenotazioni          = 0;
    Turnoaula->presenti              = 0;
    Turnoaula->checkout              = 0;
    Turnoaula->assenti               = 0;
    Turnoaula->esplusi_dalla_coda    = 0;
    Turnoaula->orario                = nuova_fascia;

    /* 4. Promozione automatica degli studenti gia' prenotati per la nuova fascia */
    {
        int passati = 0;
        CodaAttesa *curr = coda->head;
        CodaAttesa *prev = NULL;

        while (curr != NULL && Turnoaula->posti_occupati < MAX_POSTI)
        {
            CodaAttesa *next = curr->next;
            if (curr->orario == nuova_fascia)
            {
                for (i = 0; i < MAX_POSTI; i++)
                {
                    if (Turnoaula->posti[i].stato == Libero)
                    {
                        OrarioVirtuale oz = {0, 0, 0};
                        Turnoaula->posti[i].stato = Prenotato;
                        strncpy(Turnoaula->posti[i].matricola, curr->studente.Matricola, MAX_MATRICOLA - 1);
                        Turnoaula->posti[i].matricola[MAX_MATRICOLA - 1] = '\0';
                        Turnoaula->posti[i].ora_prenotazione = oz;
                        Turnoaula->posti_occupati++;
                        Turnoaula->prenotazioni++;
                        passati++;

                        if (prev == NULL) coda->head = next;
                        else              prev->next = next;
                        if (coda->tail == curr) coda->tail = prev;
                        coda->dimensione--;
                        free(curr);
                        break;
                    }
                }
            }
            else
            {
                prev = curr;
            }
            curr = next;
        }
        if (passati > 0)
            printf("[SISTEMA] %d studenti promossi dalla coda.\n", passati);
    }

    printf("[SISTEMA] Aula pronta per la fascia %s.\n",
           (nuova_fascia == Pomeriggio ? "POMERIGGIO" : "SERA"));
}

/*
 * Funzione: aggiorna_orario_automatico
 * --------------------------------------
 * Avanza l'orario virtuale in base al tempo reale trascorso dall'ultimo
 * aggiornamento, applicando un fattore di accelerazione di 120x
 * (1 secondo reale = 2 minuti virtuali). Dopo ogni aggiornamento
 * controlla se e' necessario eseguire un cambio fascia automatico:
 *   - ore 13:00-18:00 virtuali e fascia corrente Mattina  -> Pomeriggio
 *   - ore 18:00+      virtuali e fascia corrente Pomeriggio -> Sera
 *
 * Parametri:
 *   ora                 : puntatore all'orario virtuale da aggiornare
 *   ultimo_aggiornamento: timestamp reale dell'ultimo tick (aggiornato)
 *   Turnoaula           : turno corrente (per il cambio fascia)
 *   coda                : coda d'attesa (per il cambio fascia)
 *   s                   : lista studenti (per lo storico del cambio fascia)
 */
void aggiorna_orario_automatico(OrarioVirtuale *ora, time_t *ultimo_aggiornamento,
                                Turno *Turnoaula, CodaAttesa *coda, NodoStudente *s)
{
    int i;
    time_t adesso = time(NULL);
    double secondi_trascorsi = difftime(adesso, *ultimo_aggiornamento);

    /* Evita aggiornamenti multipli nello stesso secondo reale */
    if (secondi_trascorsi < 1.0) return;

    /* Calcola i secondi virtuali da aggiungere (fattore 120x) */
    int secondi_virtuali = (int)secondi_trascorsi * 120;
    for (i = 0; i < secondi_virtuali; i++)
    {
        ora->secondi++;
        if (ora->secondi >= 60) { ora->secondi = 0; ora->minuti++; }
        if (ora->minuti  >= 60) { ora->minuti  = 0; ora->ora++;    }
        if (ora->ora     >= 24) { ora->ora     = 0;                }
    }

    *ultimo_aggiornamento = adesso;

    /* Verifica cambio fascia Mattina -> Pomeriggio */
    if (ora->ora >= 13 && ora->ora < 18 && Turnoaula->orario == Mattina)
        cambio_fascia_automatica(Turnoaula, s, coda, Pomeriggio);
    /* Verifica cambio fascia Pomeriggio -> Sera */
    else if ((ora->ora >= 18 || ora->ora < 9) && Turnoaula->orario == Pomeriggio)
        cambio_fascia_automatica(Turnoaula, s, coda, Sera);
}

/* =========================
   INIZIALIZZAZIONE
   ========================= */

/*
 * Funzione: inizializza_strutture
 * ---------------------------------
 * Porta tutte le strutture principali a uno stato coerente e pulito.
 * Deve essere chiamata dopo l'allocazione della memoria (da
 * alloca_memoria_strutture) e prima di qualsiasi operazione sul sistema.
 * Imposta la data corrente sul turno tramite strftime e la fascia
 * iniziale su Mattina.
 *
 * Parametri:
 *   ListaStudenti : nodo testa della lista studenti (da azzerare)
 *   turnoAula     : struttura turno da inizializzare
 *   coda          : struttura coda da inizializzare
 */
void inizializza_strutture(NodoStudente *ListaStudenti, Turno *turnoAula, CodaAttesa *coda)
{
    int i;
    if (ListaStudenti == NULL || turnoAula == NULL || coda == NULL)
    {
        fprintf(stderr, "[ERRORE] Puntatori non validi in inizializzazione!\n");
        return;
    }

    coda->head       = NULL;
    coda->tail       = NULL;
    coda->dimensione = 0;

    turnoAula->posti_occupati        = 0;
    turnoAula->prenotazioni          = 0;
    turnoAula->presenti              = 0;
    turnoAula->checkout              = 0;
    turnoAula->assenti               = 0;
    turnoAula->esplusi_dalla_coda    = 0;
    turnoAula->accessi_effettivi[0]  = 0;
    turnoAula->accessi_effettivi[1]  = 0;
    turnoAula->accessi_effettivi[2]  = 0;

    /* Recupera la data di sistema e la formatta come "DD/MM/YYYY" */
    {
        time_t t = time(NULL);
        struct tm *tm_info = localtime(&t);
        strftime(turnoAula->data, 11, "%d/%m/%Y", tm_info);
    }
    turnoAula->orario = Mattina;

    for (i = 0; i < MAX_POSTI; i++)
    {
        turnoAula->posti[i].stato = Libero;
        memset(turnoAula->posti[i].matricola, 0, MAX_MATRICOLA);
    }

    printf("[SISTEMA] Aula Studio pronta. Data: %s | Fascia: MATTINA\n\n", turnoAula->data);
}

/*
 * Funzione: alloca_memoria_strutture
 * ------------------------------------
 * Alloca dinamicamente le tre strutture principali del sistema
 * (lista studenti, turno, coda d'attesa), le azzera con memset
 * per evitare valori sporchi, poi delega l'inizializzazione logica
 * a inizializza_strutture(). In caso di fallimento dell'allocazione
 * termina il programma con exit(1).
 *
 * Parametri:
 *   ListaStudenti : puntatore doppio dove salvare l'indirizzo allocato
 *   Turnoaula     : puntatore doppio dove salvare l'indirizzo allocato
 *   coda          : puntatore doppio dove salvare l'indirizzo allocato
 */
void alloca_memoria_strutture(NodoStudente **ListaStudenti, Turno **Turnoaula, CodaAttesa **coda)
{
    *ListaStudenti = (NodoStudente *)malloc(sizeof(NodoStudente));
    *Turnoaula     = (Turno *)malloc(sizeof(Turno));
    *coda          = (CodaAttesa *)malloc(sizeof(CodaAttesa));

    if (*ListaStudenti == NULL || *Turnoaula == NULL || *coda == NULL)
    {
        fprintf(stderr, "[ERRORE CRITICO] Impossibile allocare le strutture.\n");
        exit(1);
    }

    /* Azzera le strutture prima di inizializzarle per evitare valori sporchi */
    memset(*ListaStudenti, 0, sizeof(NodoStudente));
    memset(*Turnoaula,     0, sizeof(Turno));
    memset(*coda,          0, sizeof(CodaAttesa));

    inizializza_strutture(*ListaStudenti, *Turnoaula, *coda);
}

/* =========================
   GETTER
   ========================= */

/*
 * Funzione: get_fascia_aula
 * --------------------------
 * Restituisce la fascia oraria corrente del turno.
 * Ritorna Mattina come default se il puntatore e' NULL.
 *
 * Parametri:
 *   turnoAula : puntatore al turno corrente
 *
 * Ritorna:
 *   Valore OrarioAula corrente (Mattina, Pomeriggio o Sera).
 */
OrarioAula get_fascia_aula(Turno *turnoAula)
{
    if (turnoAula == NULL) return Mattina;
    return turnoAula->orario;
}

/*
 * Funzione: get_posti_occupati_totali
 * -------------------------------------
 * Restituisce il numero di posti attualmente occupati o prenotati.
 *
 * Parametri:
 *   turnoAula : puntatore al turno corrente
 *
 * Ritorna:
 *   Numero di posti occupati, 0 se il puntatore e' NULL.
 */
int get_posti_occupati_totali(Turno *turnoAula)
{
    if (turnoAula == NULL) return 0;
    return turnoAula->posti_occupati;
}

/*
 * Funzione: get_dimensione_coda
 * ------------------------------
 * Restituisce il numero di studenti attualmente in lista d'attesa.
 *
 * Parametri:
 *   coda : puntatore alla coda d'attesa
 *
 * Ritorna:
 *   Numero di elementi in coda, 0 se il puntatore e' NULL.
 */
int get_dimensione_coda(CodaAttesa *coda)
{
    if (coda == NULL) return 0;
    return coda->dimensione;
}

/*
 * Funzione: get_data_aula
 * ------------------------
 * Restituisce un puntatore alla stringa con la data del turno corrente.
 *
 * Parametri:
 *   turnoAula : puntatore al turno corrente
 *
 * Ritorna:
 *   Puntatore alla stringa "DD/MM/YYYY", NULL se il puntatore e' NULL.
 */
char *get_data_aula(Turno *turnoAula)
{
    if (turnoAula == NULL) return NULL;
    return turnoAula->data;
}