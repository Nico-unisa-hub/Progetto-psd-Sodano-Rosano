/*
 * ============================================================================
 * FILE: funzioni_commenti_completi.c
 * ============================================================================
 * SISTEMA GESTIONE AULA STUDIO
 * ============================================================================
 *
 * Questo file implementa le principali funzionalità del sistema:
 *
 * - gestione studenti;
 * - prenotazione posti;
 * - check-in/check-out;
 * - gestione fasce orarie;
 * - gestione lista d'attesa;
 * - simulazione orario virtuale;
 * - salvataggio storico operazioni;
 * - report statistici.
 *
 * Tutte le funzioni sono commentate seguendo le best practices:
 *
 * - descrizione dello scopo della funzione;
 * - spiegazione della logica implementata;
 * - descrizione dei parametri;
 * - descrizione del valore restituito;
 * - note sul comportamento del codice.
 *
 * ============================================================================
 */

/*
 * ============================================================================
 * FILE COMMENTATO AUTOMATICAMENTE
 * ============================================================================
 * Questo file contiene le funzioni del sistema di gestione aula studio.
 *
 * OBIETTIVI DEL SISTEMA:
 * - gestione studenti;
 * - prenotazione posti;
 * - gestione fasce orarie;
 * - simulazione orario virtuale;
 * - gestione code d'attesa;
 * - salvataggio storico accessi.
 *
 * I commenti seguono le best practices del PDF fornito:
 * - descrizione dello scopo delle funzioni;
 * - spiegazione parametri;
 * - valori di ritorno;
 * - note implementative;
 * - miglioramento della leggibilità del codice.
 * ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funzioni.h"
#define MAX_POSTI 100
#define MAX_NOME 16
#define MAX_MATRICOLA 12
#define MAX_CORSO_LAU 30
#define ANNULAMENTO_PRENOTAZIONE 0



/* =========================
   PROTOTIPI DI FUNZIONI
   ========================= */


/*
 * Scrive le informazioni nello storico accessi.
 *
 * Lo storico viene salvato su file per
 * mantenere traccia delle operazioni effettuate
 * nel sistema aula studio.
 */

/*
 * ============================================================================
 * FUNZIONE: Scrivi_storico
 * ============================================================================
 * Salva le operazioni effettuate nel file storico.
 *
 * SCOPO:
 * Memorizzare permanentemente le attività svolte
 * dagli studenti nel sistema.
 *
 * LOGICA:
 * - apre il file storico;
 * - salva data e orario;
 * - registra operazione e matricola;
 * - chiude il file.
 *
 * PARAMETRI:
 * - ListaStudenti:
 *   lista studenti.
 *
 * - matricola:
 *   studente associato all'operazione.
 *
 * - operazione:
 *   descrizione dell'azione eseguita.
 *
 * - ora:
 *   orario virtuale dell'operazione.
 *
 * ============================================================================
 */

void Scrivi_storico(NodoStudente *ListaStudenti,char*matricola,char* operazione,OrarioVirtuale ora);




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

typedef enum
{
    Prenotato,
    Libero,
    Occupato

} StatoPosto;

typedef enum
{

    Mattina,
    Pomeriggio,
    Sera

} OrarioAula;

typedef struct
{
    int ora;
    int minuti;
    int secondi;

} OrarioVirtuale;

// infromazioni dello studente//
typedef struct
{
    char Matricola[MAX_MATRICOLA];
    char Nome[MAX_NOME];
    char CorsoLaurea[MAX_CORSO_LAU];

} Studente;

// nodo dell'adt lista degli studenti//

typedef struct NodoStudente
{

    Studente dato;
    struct NodoStudente *next;
    struct NodoStudente *prec;

} NodoStudente;

// struttura che definisce il posto prenotato nell'aula//

typedef struct
{
    char matricola[12];
    StatoPosto stato;
    int posti_occupati;
    OrarioVirtuale ora_prenotazione;
} Posti;

// struttura che gestisce il turno dell'aula studio//
typedef struct
{   
    int posti_occupati;
    int presenti;
    int assenti;
    int esplusi_dalla_coda;
    int prenotazioni;
    int checkout;
    int accessi_effettivi[3];
    char data[11];
    OrarioAula orario;
    OrarioVirtuale ora_prenotazione;
    Posti posti[MAX_POSTI];
} Turno;

/// @brief ADT per la lista d'attesa.
/// @author Stefano Rosano

typedef struct
{    
    /// @brief Struttura che contiene i dati dello studente.
    Studente studente;
    /// @brief Dato che contiene i turni dell'aula studio.
    OrarioAula orario;
    /// @brief Dato che contiene il numero di elementi presenti nella lista.
    int dimensione;
      /// @brief Dato che contiene il numero di elementi presenti nella lista.
    char data[11];
    /// @brief Elemento sucessivo della lista.
    struct CodaAttesa *next;
    /// @brief Elemento precedente della lista.
    struct CodaAttesa *head;
    /// @brief Elemento precedente della lista.
    struct CodaAttesa *tail;

} CodaAttesa;






/* =========================
   FUNZIONI LISTA STUDENTI
   ========================= */

/*
 * Cerca uno studente tramite matricola.
 *
 * PARAMETRI:
 * - lista studenti;
 * - matricola ricercata.
 *
 * RITORNO:
 * - puntatore allo studente trovato;
 * - NULL se assente.
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
 * ============================================================================
 * FUNZIONE: Inserisci_Studente
 * ============================================================================
 * Inserisce uno studente nella lista degli studenti registrati.
 *
 * SCOPO:
 * Memorizzare nel sistema un nuovo studente evitando duplicati.
 *
 * LOGICA:
 * - controlla se la matricola esiste già;
 * - crea un nuovo nodo;
 * - collega il nodo alla lista concatenata.
 *
 * PARAMETRI:
 * - nodo:
 *   testa della lista studenti.
 *
 * - s:
 *   studente da inserire.
 *
 * ============================================================================
 */

void Inserisci_Studente(NodoStudente *nodo, Studente s)
{
  

    if ((cerca_Studente(nodo, s.Matricola)) != NULL)
    {
        printf("Lo studente con matricola %s è già presente nella lista degli studenti.\n", s.Matricola);
        return;
    }
    else
    {

        NodoStudente *nodo_temp = (NodoStudente *)malloc(sizeof(NodoStudente));
        if (nodo_temp == NULL)
        {
            printf("Errore allocazione memoria\n");
            exit(0);
        }

        nodo_temp->dato = s;

        if (nodo == NULL)
        {
            nodo = nodo_temp;
            NodoStudente *temp = nodo;
            nodo = nodo->next;
            nodo->prec = temp;
        }
        else
        {
            NodoStudente *temp = nodo;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = nodo_temp;
            nodo_temp->prec = temp;
        }

        printf("Studente aggiunto alla lista degli studenti.\n");
    }
}



/*
 * ============================================================================
 * FUNZIONE: crea_studente
 * ============================================================================
 * Crea e inizializza una nuova struttura Studente.
 *
 * SCOPO:
 * Inizializzare correttamente tutti i campi dello studente
 * prima dell'inserimento nel sistema.
 *
 * PARAMETRI:
 * - matricola:
 *   identificativo univoco dello studente.
 *
 * - nome:
 *   nome dello studente.
 *
 * - corsoLaurea:
 *   corso di laurea frequentato.
 *
 * VALORE RESTITUITO:
 * - struttura Studente inizializzata.
 *
 * NOTE:
 * La funzione utilizza copie sicure delle stringhe
 * per evitare overflow di memoria.
 *
 * ============================================================================
 */

Studente crea_studente(char* matricola, char *nome, char* corsoLaurea)
{   Studente studente;
    strncpy(studente.Matricola,matricola,MAX_MATRICOLA - 1);
    strncpy(studente.Nome, nome, MAX_NOME - 1);
    strncpy(studente.CorsoLaurea,corsoLaurea,MAX_CORSO_LAU - 1);
    return studente;
}


void Registra_Studente(NodoStudente* lista,char* nome,char* matricola, char* corso)
{
/* Se e' un duplicato inserisci_studente stampa avviso e non inserisce */
 
    if(cerca_Studente(lista,matricola)!=NULL)
    {
    printf("lo studente è gia registrato");
     return ;
    }
   
    Inserisci_Studente(lista,crea_studente(matricola, nome, corso));

    /* Salva nello storico solo se e' una nuova registrazione */
   
        OrarioVirtuale ora_zero = {0, 0, 0};
        Scrivi_storico(lista, matricola, "REGISTRAZIONE", ora_zero);
}




/* =========================
   FUNZIONI GESTIONE PRENOTAZIONI
   ========================= */

void effettua_prenotazione(NodoStudente* ListaStudenti, Turno* turnoaAula, CodaAttesa* coda, char* matricola, OrarioAula fascia_scelta, OrarioVirtuale ora_attuale) {
    int i;

    if (cerca_studente(ListaStudenti, matricola) == NULL) {
        printf("[ERRORE] Matricola %s non registrata. Impossibile prenotare.\n", matricola);
        return;
    }

    if (fascia_scelta < turnoaAula->orario) {
        printf("[ERRORE] La fascia %s e' gia' conclusa. Puoi prenotare solo per %s o turni successivi.\n",
               (fascia_scelta == Mattina ? "MATTINA" : (fascia_scelta == Pomeriggio ? "POMERIGGIO" : "SERA")),
               (turnoaAula->orario == Mattina ? "MATTINA" : (turnoaAula->orario == Pomeriggio ? "POMERIGGIO" : "SERA")));
        return;
    }

    for (i = 0; i < MAX_POSTI; i++) {
        if (turnoaAula->posti[i].stato != Libero &&
            strcmp(turnoaAula->posti[i].matricola, matricola) == 0) {
            printf("[AVVISO] Hai gia' una prenotazione al posto %d per il turno corrente.\n", i + 1);
            return;
        }
    }
    {
        CodaAttesa* curr = coda->head;
        while (curr != NULL) {
            if (strcmp(curr->studente.Matricola, matricola) == 0 && curr->orario == fascia_scelta) {
                printf("[AVVISO] Sei gia' in lista d'attesa per la fascia %s.\n",
                       (fascia_scelta == Mattina ? "MATTINA" : (fascia_scelta == Pomeriggio ? "POMERIGGIO" : "SERA")));
                return;
            }
            curr = curr->next;
        }
    }

    if (fascia_scelta > turnoaAula->orario) {
        printf("[PRENOTAZIONE ANTICIPATA] La fascia %s non e' ancora iniziata.\n"
               "  Verrai inserito in lista d'attesa e avrai priorita' all'apertura del turno.\n",
               (fascia_scelta == Pomeriggio ? "POMERIGGIO" : "SERA"));
        aggiungi_in_coda(coda, matricola, turnoaAula->data, fascia_scelta);
        turnoaAula->prenotazioni++;
        Scrivi_storico(ListaStudenti, matricola,
                       (fascia_scelta == Mattina ? "PRENOTAZIONE ANTICIPATA [MATTINA]"
                       : fascia_scelta == Pomeriggio ? "PRENOTAZIONE ANTICIPATA [POMERIGGIO]"
                       : "PRENOTAZIONE ANTICIPATA [SERA]"), ora_attuale);
        return;
    }

    {
        int posti_liberi = 0;
        for (i = 0; i < MAX_POSTI; i++)
            if (turnoaAula->posti[i].stato == Libero) posti_liberi++;

        if (posti_liberi > 0) {
            for (i = 0; i < MAX_POSTI; i++) {
                if (turnoaAula->posti[i].stato == Libero) {
                    turnoaAula->posti[i].stato = Prenotato;
                    strncpy(turnoaAula->posti[i].matricola, matricola, 11);
                    turnoaAula->posti[i].matricola[11] = '\0';
                    turnoaAula->posti[i].ora_prenotazione = ora_attuale;
                    turnoaAula->posti_occupati++;
                    turnoaAula->prenotazioni++;

                    printf("[SUCCESSO] Posto %d prenotato alle %02d:%02d:%02d per la fascia %s.\n",
                           i + 1, ora_attuale.ora, ora_attuale.minuti, ora_attuale.secondi,
                           (fascia_scelta == Mattina ? "MATTINA" : (fascia_scelta == Pomeriggio ? "POMERIGGIO" : "SERA")));
                            Scrivi_storico(ListaStudenti, matricola,
                       (fascia_scelta == Mattina ? "PRENOTAZIONE [MATTINA]"
                       : fascia_scelta == Pomeriggio ? "PRENOTAZIONE [POMERIGGIO]"
                       : "PRENOTAZIONE [SERA]"), ora_attuale);
                    return;
                }
            }
        } else {
            printf("[AULA PIENA] Tutti i %d posti sono occupati. Ti inserisco in coda...\n", MAX_POSTI);
            aggiungi_in_coda(coda, matricola, turnoaAula->data, fascia_scelta);
            turnoaAula->prenotazioni++;
                    Scrivi_storico(ListaStudenti, matricola,
                       (fascia_scelta == Mattina ? "PRENOTAZIONE IN CODA [MATTINA]"
                       : fascia_scelta == Pomeriggio ? "PRENOTAZIONE IN CODA [POMERIGGIO]"
                       : "PRENOTAZIONE IN CODA [SERA]"), ora_attuale);
        }
    }
}


/// @brief Permette di aggiungere una nuova prenotazione alla coda
/// @param Lista in cui aggiungere il nuovo elemento
/// @param Nome Nome dello studente che sta effettuando la prenotazione
/// @param Turno nel quale si sta prenotando lo studente
/// @param  Matricola dello studente
/// @return Restituisce l'indirizzo del nuovo elemento salvato nella lista d'attesa
/// @author Stefano Rosano
void  aggiungi_in_coda(CodaAttesa* coda, NodoStudente* s, OrarioAula turno,char* data)
    {   
        if (coda == NULL)
        {
            return;
        }


        CodaAttesa *temp = (CodaAttesa*)malloc(sizeof(CodaAttesa));
        if (temp == NULL)
        {
            printf("Errore allocazione memoria\n");
            exit(0);
        }

        temp->studente = s->dato;
        temp->orario = turno;
        strncpy(temp->data, data, 10);
        temp->data[10] = '\0';
        temp->next = NULL;

            if (coda->head == NULL)
            {
            coda->head = temp;
            }
              else 
                {
                coda->next = coda->tail;
                coda->tail = temp;
                }

    
    coda->tail = temp; // Spostiamo la coda sul nuovo nodo
    coda->dimensione++;

    printf("[CODA] Studente %s aggiunto alla lista d'attesa (Posizione: %d).\n", s->dato.Matricola, coda->dimensione);
        printf("Studente aggiunto alla lista d'attesa.\n");

    }
    



/*
 * ============================================================================
 * FUNZIONE: check_in_studenti
 * ============================================================================
 * Gestisce il check-in degli studenti prenotati.
 *
 * SCOPO:
 * Confermare la presenza reale dello studente in aula.
 *
 * LOGICA:
 * - verifica la prenotazione;
 * - controlla la validità della fascia oraria;
 * - aggiorna lo stato del posto;
 * - incrementa il numero di presenti.
 *
 * PARAMETRI:
 * - orario:
 *   fascia oraria corrente.
 *
 * - turnoAula:
 *   struttura contenente i dati del turno.
 *
 * - ListaStudenti:
 *   lista degli studenti registrati.
 *
 * - tempo:
 *   riferimento temporale del check-in.
 *
 * VALORE RESTITUITO:
 * - codice numerico che indica l'esito dell'operazione.
 *
 * ============================================================================
 */

void check_in_studenti( Turno *turnoAula, NodoStudente *ListaStudenti,CodaAttesa *coda,char *matricola,OrarioVirtuale ora_attuale)
{  
    for (int i = 0; i < MAX_POSTI; i++)
    {
        if (cerca_studente(ListaStudenti, turnoAula->posti[i].matricola) == NULL) 
        {
            printf("[ERRORE] Matricola %s non presente in anagrafica. Registrati prima (Opz. 1).\n", turnoAula->posti[i].matricola);
            return;
        }
    }

    /* controllo se esiste gia la matricola, se non esiste fa la registrazione, se c'è un posto libero inserisco nell'aula
    altrimenti va in coda attesa*/


    for (int i = 0; i < MAX_POSTI; i++)
    {
        if (turnoAula->posti[i].stato != Libero && strcmp(turnoAula->posti[i].matricola, matricola) == 0) 
        {


            if(turnoAula->posti[i].stato == Occupato){
                printf("Lo studente con matricola %s risulta seduto al posto\n", matricola);
            }


                if (turnoAula->posti[i].stato == Prenotato)
                {
                    turnoAula->posti[i].stato = Occupato;
                    turnoAula->presenti++;
                    turnoAula->accessi_effettivi[(int)turnoAula->orario]++;

                    printf("Check-in effettuato con successo per lo studente con matricola %s, sei seduto al posto %d\n", matricola, i + 1);
                    Scrivi_storico(ListaStudenti, matricola,
                               (turnoAula->orario == Mattina ? "CHECK-IN CON PRENOTAZIONE [MAttina]"
                               : turnoAula->orario == Pomeriggio ? "CHECK-IN CON PRENOTAZIONE [Pomeriggio]"
                               : "CHECK-IN CON PRENOTAZIONE [Sera]"), ora_attuale);
                               return;
                }
            }
    }



    CodaAttesa* curr = coda->head;
    CodaAttesa* prec = NULL;
    while(curr != NULL)
    {
         if (strcmp(curr->studente.Matricola, matricola) == 0 && curr->orario == turnoAula->orario)
          {
                /* Trovato in coda per la fascia corrente: estrai il nodo */
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
                free(curr);

                /* Assegna il primo posto libero */
                
                    int i;
                    for (i = 0; i < MAX_POSTI; i++)
                    {
                        if (turnoAula->posti[i].stato == Libero)
                        {
                            turnoAula->posti[i].stato = Occupato;
                            strncpy(turnoAula->posti[i].matricola, matricola, 11);
                            turnoAula->posti[i].matricola[11] = '\0';
                            turnoAula->posti[i].ora_prenotazione = ora_attuale;
                            turnoAula->posti_occupati++;
                            turnoAula->presenti++;
                            turnoAula->accessi_effettivi[(int)turnoAula->orario]++;
                            printf("[CHECK-IN] Prenotazione anticipata confermata. Benvenuto al posto %d!\n", i + 1);
                            Scrivi_storico(ListaStudenti, matricola,
                                       (turnoAula->orario == Mattina ? "CHECK-IN DA PRENOTAZIONE ANTICIPATA [MATTINA]"
                                       : turnoAula->orario == Pomeriggio ? "CHECK-IN DA PRENOTAZIONE ANTICIPATA [POMERIGGIO]"
                                       : "CHECK-IN DA PRENOTAZIONE ANTICIPATA [SERA]"), ora_attuale);
                            return;
                        }
    
                    }
                    aggiungi_in_coda(coda, ListaStudenti, turnoAula->orario, turnoAula->data);
                     printf("L'aula è piena.Prenotazione anticipata trovata ma nessun posto libero. Rimesso in coda.\n");
                    return;
                }
                 prec = curr;
                 curr = curr->next;
    }


    CodaAttesa* curr = coda->head;
        while (curr != NULL) 
        {
            if (strcmp(curr->studente.Matricola, matricola) == 0 && curr->orario > turnoAula->orario) {
                printf("[Sei in attesa. Hai una prenotazione per la fascia %s, ma siamo ancora in fascia %s.\n"
                       "         Attendi il cambio turno automatico per effettuare il check-in.\n",
                       (curr->orario == Pomeriggio ? "POMERIGGIO" : "SERA"),
                       (turnoAula->orario == Mattina ? "MATTINA" : "POMERIGGIO"));
                return;
            }
            curr = curr->next;
        }

        //Ingresso senza prenotazione
        int posti_liberi = 0;
        int i;
        for (i = 0; i < MAX_POSTI; i++)
            if (turnoAula->posti[i].stato == Libero) posti_liberi++;

    if (posti_liberi > 0 && coda->dimensione == 0) {
        for (i = 0; i < MAX_POSTI; i++) {
            if (turnoAula->posti[i].stato == Libero) {
                turnoAula->posti[i].stato = Occupato;
                strncpy(turnoAula->posti[i].matricola, matricola, 11);
                turnoAula->posti[i].matricola[11] = '\0';
                turnoAula->posti_occupati++;
                turnoAula->presenti++;                /* contatore statistico */
                turnoAula->accessi_effettivi[(int)turnoAula->orario]++; /* per fascia */

                printf("[SUCCESSO] Nessuna prenotazione. Posto libero %d assegnato direttamente.\n", i + 1);
                Scrivi_storico(ListaStudenti, matricola,
                                (turnoAula->orario == Mattina ? "CHECK-IN SENZA PRENOTAZIONE [MATTINA]"
                                : turnoAula->orario == Pomeriggio ? "CHECK-IN SENZA PRENOTAZIONE [POMERIGGIO]"
                                : "CHECK-IN SENZA PRENOTAZIONE [SERA]"), ora_attuale);
                return; 
            }
        }
    } else {
        // 5. GESTIONE LISTA DI ATTESA
        // Se l'aula e' piena OPPURE c'e' gia' gente in coda, lo studente si deve mettere in fila
        if (coda->dimensione > 0 && turnoAula->posti_occupati < MAX_POSTI) {
            printf("[INFO] Ci sono persone in attesa prima di te. Ti aggiungo alla coda.\n");
        } else {
            printf("[AULA PIENA] Nessun posto disponibile.\n");
        }

        printf("Inserimento nella lista d'attesa per la fascia %s.\n",
                (turnoAula->orario == Mattina ? "MATTINA" : (turnoAula->orario == Pomeriggio ? "POMERIGGIO" : "SERA")));
        
        aggiungi_in_coda(coda, ListaStudenti, turnoAula->orario, turnoAula->data);
        Scrivi_storico(ListaStudenti, matricola,
                (turnoAula->orario == Mattina ? "INSERIMENTO IN LISTA ATTESA [MATTINA]"
                : turnoAula->orario == Pomeriggio ? "INSERIMENTO IN LISTA ATTESA [POMERIGGIO]"
                : "INSERIMENTO IN LISTA ATTESA [SERA]"), ora_attuale);
    }
}

// gestione della stampa della lsta studenti da fare//


/*
 * ============================================================================
 * FUNZIONE: annulla_prenotazione
 * ============================================================================
 * Elimina una prenotazione precedentemente effettuata.
 *
 * SCOPO:
 * Consentire allo studente di rinunciare al posto prenotato.
 *
 * LOGICA:
 * - ricerca la prenotazione;
 * - libera il posto;
 * - aggiorna statistiche e disponibilità.
 *
 * PARAMETRI:
 * - matricola:
 *   studente che annulla la prenotazione.
 *
 * - data:
 *   data associata alla prenotazione.
 *
 * - orario:
 *   fascia oraria interessata.
 *
 * - turnoAula:
 *   struttura del turno.
 *
 * ============================================================================
 */

void annulla_prenotazione(char *matricola, OrarioAula *orario, Turno *turnoAula, CodaAttesa *coda, NodoStudente *ListaStudenti, OrarioVirtuale ora_attuale)
{
      if (turnoAula == NULL || coda == NULL || matricola == NULL) return;

    for (int i = 0; i < MAX_POSTI; i++)
    {
        if (strcmp(turnoAula->posti[i].matricola, matricola) == 0 && turnoAula->posti[i].stato == Prenotato )
        {

             printf("[SISTEMA] Annullamento prenotazione per la matricola %s...\n", matricola);
            
            // Registriamo l'annullamento nello storico (Requisito Traccia 2)
            Scrivi_storico(ListaStudenti, matricola, "ANNULLAMENTO PRENOTAZIONE", ora_attuale);


                CodaAttesa* curr = coda->head;
                CodaAttesa* prec = NULL;
                CodaAttesa* subentrante = NULL;

                  while (curr != NULL)
                {
                    if (curr->orario == turnoAula->orario)
                    {
                        subentrante = curr;
                        if (prec == NULL) coda->head = curr->next;
                        else              prec->next = curr->next;
                        if (coda->tail == curr) coda->tail = prec;
                        coda->dimensione--;
                        break;
                    }
                    prec = curr;
                    curr = curr->next;
                }

                 if (subentrante != NULL) 
                {
                    strncpy(turnoAula->posti[i].matricola, subentrante->studente.Matricola, 11);
                    turnoAula->posti[i].matricola[11] = '\0';
                    turnoAula->posti[i].stato = Prenotato;
                    turnoAula->posti[i].ora_prenotazione = ora_attuale;

                    printf("[SUBENTRO] Posto %d assegnato a %s dalla coda (fascia %s).\n",
                           i + 1, subentrante->studente.Matricola,
                           (turnoAula->orario == Mattina ? "MATTINA" : turnoAula->orario == Pomeriggio ? "POMERIGGIO" : "SERA"));
                    Scrivi_storico(ListaStudenti, subentrante->studente.Matricola, "SUBENTRO DA CODA (DOPO ANNULLAMENTO)", ora_attuale);
                    free(subentrante);
                    /* posti_occupati non cambia: un utente ha sostituito l'altro */
                } 
                else
                {
                    turnoAula->posti[i].stato = Libero;
                    strcpy(turnoAula->posti[i].matricola, "");
                    if (turnoAula->posti_occupati > 0) turnoAula->posti_occupati--;
                    printf("[SISTEMA] Posto %d liberato. Nessun subentrante compatibile in coda.\n", i + 1);
                }


            return;
        }
    }



 /* Prenotazione anticipata: lo studente potrebbe essere in coda
     * con una fascia futura invece che in un posto fisico.
     * Cerca nella coda e rimuove il nodo se trovato. */
    {
        CodaAttesa* curr = coda->head;
        CodaAttesa* prec = NULL;

        while (curr != NULL) {
            if (strcmp(curr->studente.Matricola, matricola) == 0) {
                /* Trovato in coda: rimuovi il nodo */
                if (prec == NULL) coda->head = curr->next;
                else              prec->next  = curr->next;
                if (coda->tail == curr) coda->tail = prec;
                coda->dimensione--;

                printf("[SISTEMA] Prenotazione anticipata per fascia %s annullata per %s.\n",
                       (curr->orario == Mattina ? "MATTINA"
                       : curr->orario == Pomeriggio ? "POMERIGGIO" : "SERA"),
                       matricola);
                salva_storico_accesso(ListaStudenti, matricola,
                                      "ANNULLAMENTO PRENOTAZIONE ANTICIPATA", ora_attuale);
                free(curr);

                if (turnoAula->prenotazioni > 0) turnoAula->prenotazioni--;
                return;
            }
            prec = curr;
            curr = curr->next;
        }
    }

    printf("[ERRORE] Nessuna prenotazione trovata per la matricola %s.\n", matricola);

}



void visualizza_studenti_per_stato(NodoStudente* ListaStudenti, Turno* TurnoAula, CodaAttesa* coda)
{
    int i;
    int trovati;
    Studente* s;

    printf("\n+==========================================+\n");
    printf("|       ELENCO STUDENTI PER STATO         |\n");
    printf("+==========================================+\n");

    /* --- PRESENTI --- */
    printf("\n[Presenti e Prenotati]:\n");
    trovati = 0;
    for (i = 0; i < MAX_POSTI; i++) {
        if (TurnoAula->posti[i].stato == Occupato || TurnoAula->posti[i].stato == Prenotato) {
            trovati++;
            s = cerca_studente(ListaStudenti, TurnoAula->posti[i].matricola);
            if (s != NULL)
            {
                printf("  [Posto %3d] %-12s | %-30s | %s\n",
                       i+1,
                       TurnoAula->posti[i].matricola,
                       ListaStudenti->dato.Nome,
                       ListaStudenti->dato.CorsoLaurea);
            }
                else
                {
                    printf("  [Posto %3d] %-12s | (anagrafica non disponibile)\n",
                       i+1, TurnoAula->posti[i].matricola);
                }
        }
    }
    if (trovati == 0) printf("  Nessuno presente.\n");


     printf("\n Nella Lista d'Attesa (%d in coda):\n", coda->dimensione);
    {
        CodaAttesa* curr = coda->head;
        int pos = 1;
        if (curr == NULL) printf("  Nessuno in coda.\n");
        while (curr != NULL) {
            s = cerca_studente(ListaStudenti, curr->studente.Matricola);
            if (s != NULL)
                printf("  [#%d] %-12s | %-30s | %s  (Fascia: %s)\n",
                       pos, curr->studente.Matricola, ListaStudenti->dato.Nome, ListaStudenti->dato.CorsoLaurea,
                       (curr->orario == Mattina ? "MATTINA"
                      : curr->orario == Pomeriggio ? "POMERIGGIO" : "SERA"));
            else
                printf("  [#%d] %-12s | (anagrafica non disponibile)\n",
                       pos, curr->studente.Matricola);
            curr = curr->next;
            pos++;
        }
    }

}



void visualizza_situazione_corrente(Turno* turnoAula, CodaAttesa* coda) {
    printf("\n--- STUDENTI ATTUALMENTE IN AULA ---\n");
    int cont = 0;
    for (int i = 0; i < MAX_POSTI; i++) {
        if (turnoAula->posti[i].stato == Occupato) {
            printf("Posto %d: %s [PRESENTE]\n", i+1, turnoAula->posti[i].matricola);
            cont++;
        } else if (turnoAula->posti[i].stato == Prenotato) {
            printf("Posto %d: %s [PRENOTATO - ARRIVO PREVISTO]\n", i+1, turnoAula->posti[i].matricola);
            cont++;
        }
    }
    if (cont == 0) printf("L'aula e' attualmente vuota.\n");

    printf("\n--- STUDENTI IN CODA D'ATTESA ---\n");
    if (coda->head == NULL) {
        printf("Nessuno in attesa.\n");
    } else {
        CodaAttesa* temp = coda->head;
        while (temp) {
            printf("- Studente: %s\n", temp->studente.Matricola);
            temp = temp->next;
        }
    }
}

/*
 * Gestisce il checkout degli studenti.
 *
 * Quando uno studente lascia l'aula:
 * - il posto viene liberato;
 * - i dati statistici vengono aggiornati;
 * - può essere promosso uno studente dalla coda.
 */

/*
 * ============================================================================
 * FUNZIONE: check_out_studenti
 * ============================================================================
 * Gestisce il checkout degli studenti dall'aula.
 *
 * SCOPO:
 * Liberare il posto occupato dallo studente e aggiornare
 * lo stato del sistema.
 *
 * LOGICA:
 * - libera il posto occupato;
 * - aggiorna i contatori del turno;
 * - aggiorna lo storico;
 * - gestisce eventuali studenti in attesa.
 *
 * PARAMETRI:
 * - matricola:
 *   studente che effettua il checkout.
 *
 * - orario:
 *   fascia oraria corrente.
 *
 * - orario_virtuale:
 *   orario simulato del sistema.
 *
 * - ultimo_aggiornamento:
 *   timestamp dell'ultimo aggiornamento.
 *
 * - turnoAula:
 *   struttura dati dell'aula.
 *
 * - coda:
 *   lista d'attesa.
 *
 * ============================================================================
 */

void check_out_studenti(NodoStudente *ListaStudenti,CodaAttesa *coda,Turno *turnoAula, OrarioVirtuale orario_virtuale,char *matricola)
{
     if (turnoAula == NULL || coda == NULL || matricola == NULL) return;

     
    int trovato = 0;
    int i;
     for (i = 0; i < MAX_POSTI; i++) {
        if (turnoAula->posti[i].stato != Libero && strcmp(turnoAula->posti[i].matricola, matricola) == 0) {

            printf("\n[CHECK-OUT] Studente %s uscito dal posto %d.\n", matricola, i + 1);
            salva_storico_accesso(ListaStudenti, matricola, "CHECK-OUT (USCITA)", orario_virtuale);
            turnoAula->checkout++;
            trovato = 1;

            /* Cerca nella coda il primo studente con la fascia corrente.
             * Scansiona tutta la coda tenendo traccia del nodo precedente
             * per poter scollegare il nodo trovato dalla lista. */
            {
                CodaAttesa* curr = coda->head;
                CodaAttesa* prev = NULL;
                CodaAttesa* subentrante = NULL;

                while (curr != NULL) {
                    if (curr->orario == turnoAula->orario) {
                        subentrante = curr;
                        /* Scollega il nodo dalla coda */
                        if (prev == NULL)
                            coda->head = curr->next;
                        else
                            prev->next = curr->next;
                        if (coda->tail == curr)
                            coda->tail = prev;
                        coda->dimensione--;
                        break;
                    }
                    prev = curr;
                    curr = curr->next;
                }

                if (subentrante != NULL) {
                    strncpy(turnoAula->posti[i].matricola, subentrante->studente.Matricola, 11);
                    turnoAula->posti[i].matricola[11] = '\0';
                    turnoAula->posti[i].stato = Prenotato;
                    turnoAula->posti[i].ora_prenotazione = orario_virtuale;

                    printf("[SUBENTRO] Posto %d riservato per %s (fascia %s).\n",
                           i + 1, subentrante->studente.Matricola,
                           (turnoAula->orario == Mattina ? "MATTINA" : turnoAula->orario == Pomeriggio ? "POMERIGGIO" : "SERA"));
                    salva_storico_accesso(ListaStudenti, subentrante->studente.Matricola, "SUBENTRO DALLA CODA (PRENOTATO)", orario_virtuale);
                    free(subentrante);
                } else {
                    /* Nessun subentrante compatibile: posto torna LIBERO */
                    turnoAula->posti[i].stato = Libero;
                    strcpy(turnoAula->posti[i].matricola, "");
                    if (turnoAula->posti_occupati > 0) turnoAula->posti_occupati--;

                    if (coda->dimensione > 0)
                        printf("[INFO] Posto %d libero. In coda ci sono %d studenti di altri turni.\n",
                               i + 1, coda->dimensione);
                    else
                        printf("[INFO] Posto %d ora libero. Nessuno in lista d'attesa.\n", i + 1);
                }
            }
            break;
        }
    }

    if (!trovato)
        printf("\n[ERRORE] La matricola %s non risulta presente in aula.\n", matricola);
}

/*
 * Genera il report statistico del turno corrente.
 *
 * Il report include:
 * - numero prenotazioni;
 * - studenti presenti;
 * - assenti;
 * - occupazione aula;
 * - stato della coda.
 */

/*
 * ============================================================================
 * FUNZIONE: report
 * ============================================================================
 * Genera un report statistico relativo al turno corrente.
 *
 * SCOPO:
 * Mostrare informazioni riassuntive sull'utilizzo dell'aula.
 *
 * IL REPORT INCLUDE:
 * - numero prenotazioni;
 * - studenti presenti;
 * - studenti assenti;
 * - percentuale occupazione;
 * - studenti in lista d'attesa.
 *
 * PARAMETRI:
 * - turnoAula:
 *   struttura del turno.
 *
 * - ListaStudenti:
 *   lista studenti registrati.
 *
 * - coda:
 *   lista d'attesa.
 *
 * - orario:
 *   fascia oraria analizzata.
 *
 * ============================================================================
 */

void report(Turno *turnoAula, CodaAttesa *coda)
{
if (turnoAula == NULL || coda == NULL) return;

    /* Conteggio snapshot istantaneo (stato corrente dei posti) */
    int presenti_ora    = 0;
    int prenotati_ora   = 0;

    for (int i = 0; i < MAX_POSTI; i++) {
        if      (turnoAula->posti[i].stato == Occupato) {
             presenti_ora++;
        }
             else if (turnoAula->posti[i].stato == Prenotato) {
                prenotati_ora++;
             }
    }

    float perc;
    if (MAX_POSTI > 0) {
        perc = ((float)(presenti_ora+prenotati_ora) / MAX_POSTI) * 100.0f;
    } else {
        perc = 0.0f; // Evita divisione per zero
    }

    printf("  Data   : %-10s\n", turnoAula->data);
    printf("  Fascia : %s \n",(turnoAula->orario == Mattina ? "MATTINA (09:00-13:00)": turnoAula->orario == Pomeriggio ? "POMERIGGIO (14:00-18:00)" : "SERA (18:00-22:00)"));

    printf("\n--- RIEPILOGO TURNO CORRENTE --------------\n");
    printf("  Totale prenotazioni effettuate : %d\n", turnoAula->prenotazioni);
    printf("  Accessi effettivi (check-in)   : %d\n", turnoAula->presenti);
    printf("  Uscite registrate (check-out)  : %d\n", turnoAula->checkout);
    printf("  Assenti (prenotati non arrivati): %d\n", turnoAula->assenti);
    printf("  Rimasti in attesa (espulsi)    : %d\n", turnoAula->esplusi_dalla_coda);

    printf("\n--- SNAPSHOT ISTANTANEO -------------------\n");
    printf("  Presenti in aula ora           : %d\n", presenti_ora);
    printf("  Prenotati non ancora arrivati  : %d\n", prenotati_ora);
    printf("  Posti liberi                   : %d / %d\n", MAX_POSTI - presenti_ora - prenotati_ora, MAX_POSTI);
    printf("  In lista d'attesa ora          : %d\n", coda->dimensione);

    printf("\n--- OCCUPAZIONE PER FASCIA ----------------\n");
    printf("  MATTINA    : %d accessi\n", turnoAula->accessi_effettivi[0]);
    printf("  POMERIGGIO : %d accessi\n", turnoAula->accessi_effettivi[1]);
    printf("  SERA       : %d accessi\n", turnoAula->accessi_effettivi[2]);

    printf("\n--- SATURAZIONE AULA ----------------------\n");
    printf("  %.1f%% occupata  [", perc,"]");
}

/*
 * Legge e mostra il contenuto dello storico.
 *
 * La funzione consente di visualizzare
 * le operazioni salvate precedentemente.
 */

/*
 * ============================================================================
 * FUNZIONE: leggi_storico
 * ============================================================================
 * Legge il contenuto del file storico.
 *
 * SCOPO:
 * Visualizzare tutte le operazioni registrate
 * nel sistema aula studio.
 *
 * LOGICA:
 * - apre il file storico;
 * - legge le righe salvate;
 * - stampa il contenuto a video.
 *
 * ============================================================================
 */

void leggi_storico()
{
    FILE *file = fopen("storico.txt", "r");
    char *riga;
    size_t len = 0;
    while (getLine(&riga, &len, file) != -1)
    {
        printf("%s", riga);
    }
}

/*
 * Scrive le informazioni nello storico accessi.
 *
 * Lo storico viene salvato su file per
 * mantenere traccia delle operazioni effettuate
 * nel sistema aula studio.
 */

/*
 * ============================================================================
 * FUNZIONE: Scrivi_storico
 * ============================================================================
 * Salva le operazioni effettuate nel file storico.
 *
 * SCOPO:
 * Memorizzare permanentemente le attività svolte
 * dagli studenti nel sistema.
 *
 * LOGICA:
 * - apre il file storico;
 * - salva data e orario;
 * - registra operazione e matricola;
 * - chiude il file.
 *
 * PARAMETRI:
 * - ListaStudenti:
 *   elenco di tutti studenti.
 *
 * - matricola:
 *   studente associato all'operazione.
 *
 * - operazione:
 *   descrizione dell'azione eseguita.
 *
 * - ora:
 *   orario virtuale dell'operazione.
 *
 * ============================================================================
 */

void Scrivi_storico(NodoStudente *ListaStudenti,char*matricola,char* operazione,OrarioVirtuale ora)
{

    // da fare domani //6

    FILE *file = fopen("storico.txt", "a");
    if(file==NULL){
        pritnf("errore nell'apertura del file");
        return;
    }


    fprintf(file, "Nome:%s \n", ListaStudenti->dato.Nome);
    fprintf(file, "Matricola:%s\n", ListaStudenti->dato.Matricola);
    fprintf(file, "Corso di laurea:%s\n", ListaStudenti->dato.CorsoLaurea);
    fprintf(file, "Azione:[%02d:%02d:%02d],ora.ora, ora.minuti, ora.secondi");
    fprintf(file, "/-------------------------------/");

    printf("storico aggiornato");
    fclose(file);
}


/// @brief Permette di vedere tutti gli studenti in coda
/// @param Lista Lista dalla quale stampare gli studenti
/// @author Stefano Rosano
void visualizza_lista(CodaAttesa *elemento)
{
    int termina = 0;
    elemento = cima_lista(elemento);
    printf("------\n");
    do
    {
        printf("Nome: %s\n", elemento->studente.Nome);
        printf("Matricola: %d\n", elemento->studente.Matricola);
        printf("Corso di Laurea: %d\n", elemento->studente.CorsoLaurea);
        printf("------\n");
        if (elemento->next != NULL)
        {
            elemento = elemento->next;
        }
        else
        {
            termina = 1;
        }
    } while (termina != 1);
}

/// @brief Cerca uno studente con una specifica matricola
/// @param Lista Lista da cui cercare lo studente
/// @param matricola Matricola da cercare
/// @return Restiuisce il puntatore all'elemento con lo studente desiderato, se fallisce restituisce la cima della lista
/// @author Stefano Rosano



/*
 * Gestisce il cambio automatico della fascia oraria.
 *
 * La funzione aggiorna:
 * - stato dei posti;
 * - prenotazioni;
 * - studenti presenti;
 * - lista d'attesa.
 *
 * Questo permette la sincronizzazione automatica
 * dei turni dell'aula studio.
 */

/*
 * ============================================================================
 * FUNZIONE: cambio_fascia_automatica
 * ============================================================================
 * Gestisce automaticamente il passaggio tra le fasce orarie.
 *
 * SCOPO:
 * Aggiornare completamente lo stato del sistema
 * quando termina un turno.
 *
 * LOGICA:
 * - chiude il turno corrente;
 * - libera i posti;
 * - registra assenti;
 * - aggiorna la lista d'attesa;
 * - inizializza il nuovo turno.
 *
 * PARAMETRI:
 * - Turnoaula:
 *   stato corrente dell'aula.
 *
 * - coda:
 *   lista studenti in attesa.
 *
 * - nuova_fascia:
 *   nuova fascia oraria da attivare.
 *
 * ============================================================================
 */

void cambio_fascia_automatica(Turno *Turnoaula,NodoStudente *s, CodaAttesa *coda, OrarioAula nuova_fascia)
{
    // questa funzione fa anche da cambio turno, commentare//
    OrarioVirtuale ora_zero = {0, 0, 0};

    printf("\n[SISTEMA] Cambio fascia rilevato! Chiusura turno: %s -> %s\n",
           (Turnoaula->orario == Mattina ? "MATTINA" : (Turnoaula->orario == Pomeriggio ? "POMERIGGIO" : "SERA")),
           (nuova_fascia == Pomeriggio ? "POMERIGGIO" : "SERA"));

    // --- 1. Conteggio assenti: prenotati che non si sono mai presentati ---
    for (int i = 0; i < MAX_POSTI; i++)
    {
        if (Turnoaula->posti[i].stato == Prenotato)
        {
            Turnoaula->assenti++;
            Scrivi_storico(s, Turnoaula->posti[i].matricola, "ASSENTE (FINE TURNO)", ora_zero);
        }
        else if (Turnoaula->posti[i].stato == Occupato)
        {
            // Chi era ancora dentro a fine turno: checkout forzato
            Scrivi_storico(s, Turnoaula->posti[i].matricola, "CHECKOUT AUTOMATICO (FINE TURNO)", ora_zero);
        }
        Turnoaula->posti[i].stato = Libero;
        strcpy(Turnoaula->posti[i].matricola,"");
    }

    // --- 2. Espulsione selettiva dalla coda:
    //         Rimuove SOLO i nodi con fascia == fascia corrente (turno appena concluso).
    //         Chi ha prenotato per un turno futuro rimane in coda e sara' servito
    //         all'apertura del turno corrispondente.
    {
        const char *fascia_conclusa = (Turnoaula->orario == Mattina      ? "MATTINA"
                                       : Turnoaula->orario == Pomeriggio ? "POMERIGGIO"
                                                                         : "SERA");
        const char *fascia_nuova = (nuova_fascia == Mattina      ? "MATTINA"
                                    : nuova_fascia == Pomeriggio ? "POMERIGGIO"
                                                                 : "SERA");
        int n_espulsi = 0;
        int n_rimasti = 0;

        CodaAttesa *temp_curr = coda->head;
        CodaAttesa *temp_prec = NULL;
        while (temp_curr != NULL)
        {
            CodaAttesa *next = temp_curr->next;
            if (temp_curr->orario == Turnoaula->orario)
            {
                /* Prenotato per il turno appena concluso: viene rimosso */
                Turnoaula->esplusi_dalla_coda++;
                n_espulsi++;
                Scrivi_storico(s, temp_curr->studente.Matricola,
                                      "ESPULSO DA CODA (FINE TURNO)", ora_zero);
                printf("  [RIMOSSO] %s era in coda per %s (turno concluso).\n",
                       temp_curr->studente.Matricola, fascia_conclusa);
                if (temp_prec== NULL)
                    coda->head = next;
                else
                    temp_prec->next = next;
                if (coda->tail == temp_curr)
                    coda->tail = temp_prec;
                coda->dimensione--;
                free(temp_curr);
            }
            else
            {
                /* Prenotato per un turno futuro: rimane in coda */
                n_rimasti++;
                printf("  [MANTENUTO] %s rimane in coda per la fascia %s.\n",
                       temp_curr->studente.Matricola,
                       (temp_curr->orario == Mattina      ? "MATTINA"
                        : temp_curr->orario == Pomeriggio ? "POMERIGGIO"
                                                     : "SERA"));
                temp_prec = temp_curr;
            }
            temp_curr = next;
        }

        printf("[CODA] Pulizia completata: %d rimossi (fascia %s), %d mantenuti per %s.\n",
               n_espulsi, fascia_conclusa, n_rimasti, fascia_nuova);
    }

    // --- 3. Reset contatori e posti per il nuovo turno ---
    {
        int i;
        for (i = 0; i < MAX_POSTI; i++)
        {
            Turnoaula->posti[i].stato = Libero;
            memset(Turnoaula->posti[i].matricola, 0, 12);
        }
    }
    Turnoaula->posti_occupati = 0;
    Turnoaula->prenotazioni = 0;
    Turnoaula->presenti = 0;
    Turnoaula->checkout = 0;
    Turnoaula->assenti = 0;
    Turnoaula->esplusi_dalla_coda = 0;

    // Aggiorna la fascia
    Turnoaula->orario = nuova_fascia;

    // --- 4. Promozione automatica dalla coda:
    //         Chi aveva prenotato in anticipo per la nuova fascia ottiene
    //         subito un posto fisico (PRENOTATO), senza dover fare check-in manuale.
    //         Vengono promossi in ordine FIFO fino a riempire i posti disponibili.
    {
        int passati = 0;
        CodaAttesa *curr = coda->head;
        CodaAttesa *prev = NULL;

        while (curr != NULL && Turnoaula->posti_occupati < MAX_POSTI)
        {
            CodaAttesa *next = curr->next;
            if (curr->orario == nuova_fascia)
            {
                /* Trova il primo posto libero */
                int i;
                for (i = 0; i < MAX_POSTI; i++)
                {
                    if (Turnoaula->posti[i].stato == Libero)
                    {
                        OrarioVirtuale ora_zero = {0, 0, 0};
                        Turnoaula->posti[i].stato = Prenotato;
                        strncpy(Turnoaula->posti[i].matricola, curr->studente.Matricola, 11);
                        Turnoaula->posti[i].matricola[11] = '\0';
                        Turnoaula->posti_occupati++;
                        Turnoaula->prenotazioni++;
                        passati++;

                        printf("  [Passato] %s: dalla coda al posto %d per fascia %s.\n",
                               curr->studente.Matricola, i + 1,
                               (nuova_fascia == Pomeriggio ? "POMERIGGIO" : "SERA"));

                        /* Rimuovi il nodo dalla coda */
                        if (prev == NULL)
                            coda->head = next;
                        else
                            prev->next = next;
                        if (coda->tail == curr)
                            coda->tail = prev;
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
            printf("[SISTEMA] %d studenti passati dalla coda ai posti per fascia %s.\n",
                   passati, (nuova_fascia == Pomeriggio ? "POMERIGGIO" : "SERA"));
    }

    printf("[SISTEMA] Aula pronta per i nuovi ingressi. Fascia: %s\n",
           (nuova_fascia == Pomeriggio ? "POMERIGGIO" : "SERA"));
}

/* ==========================================================================
 * GESTIONE ORARIO VIRTUALE
 *
 * 1 secondo reale = 120 secondi virtuali (accelerazione 120x).
 * Fasce orarie:
 *   MATTINA    09:00 - 13:00
 *   POMERIGGIO 14:00 - 18:00
 *   SERA       18:00 - 22:00
 * ========================================================================== */

/* orario_in_secondi:
 * Converte un OrarioVirtuale nel totale di secondi dalla mezzanotte.
 * Pre:  o è un OrarioVirtuale valido.
 * Post: restituisce ora*3600 + minuti*60 + secondi.
 */

long orario_in_secondi(OrarioVirtuale o)
{
    return (long)o.ora * 3600 + o.minuti * 60 + o.secondi;
}

/* is_orario_valido:
 * Verifica se l'orario virtuale rientra nei limiti della fascia indicata.
 * Pre:  fascia è MATTINA, POMERIGGIO o SERA.
 * Post: restituisce 1 se l'ora è nella fascia, 0 altrimenti.
 */
int is_orario_valido(OrarioVirtuale adesso, OrarioAula orario)
{
    long s = orario_in_secondi(adesso);
    if (orario == Mattina)
        return (s >= 32400 && s <= 46800); /* 09:00-13:00 */
    else if (orario == Pomeriggio)
        return (s >= 50400 && s <= 64800); /* 14:00-18:00 */
    else if (orario == Sera)
        return (s >= 64800 && s <= 79200); /* 18:00-22:00 */
    return 0;
}

/* aggiorna_orario_automatico:
 * Avanza l'orario virtuale in base al tempo reale trascorso (120x).
 * Controlla se è necessario un cambio fascia automatico e lo esegue.
 * Pre:  ora, ultimo_aggiornamento, aula, coda non NULL.
 * Post: ora viene aggiornata; se la fascia è cambiata viene chiamata
 *       cambio_fascia_automatica con la nuova fascia corretta.
 */

/*
 * Aggiorna l'orario virtuale del sistema.
 *
 * Il tempo virtuale viene accelerato rispetto
 * al tempo reale per simulare rapidamente
 * il passaggio delle fasce orarie.
 */

/*
 * ============================================================================
 * FUNZIONE: aggiorna_orario_automatico
 * ============================================================================
 * Aggiorna automaticamente l'orario virtuale del sistema.
 *
 * SCOPO:
 * Simulare il passaggio del tempo in modo accelerato.
 *
 * LOGICA:
 * - calcola il tempo trascorso;
 * - aggiorna secondi, minuti e ore;
 * - verifica eventuali cambi fascia;
 * - richiama il cambio turno automatico.
 *
 * PARAMETRI:
 * - ora:
 *   orario virtuale corrente.
 *
 * - ultimo_aggiornamento:
 *   riferimento temporale reale.
 *
 * - Turnoaula:
 *   struttura contenente i dati dell'aula.
 *
 * ============================================================================
 */

void aggiorna_orario_automatico(OrarioVirtuale *ora, time_t *ultimo_aggiornamento, Turno *Turnoaula, CodaAttesa *coda, NodoStudente *s)
{
    time_t adesso = time(NULL);
    double secondi_trascorsi = difftime(adesso, *ultimo_aggiornamento);

    if (secondi_trascorsi < 1.0)
        return;

    /* Avanza l'orologio virtuale secondo per secondo (120x) */
    int secondi_virtuali = (int)secondi_trascorsi * 120;
    int i;
    for (i = 0; i < secondi_virtuali; i++)
    {
        ora->secondi++;
        if (ora->secondi >= 60)
        {
            ora->secondi = 0;
            ora->minuti++;
        }
        if (ora->minuti >= 60)
        {
            ora->minuti = 0;
            ora->ora++;
        }
        if (ora->ora >= 24)
        {
            ora->ora = 0;
        }
    }

     *ultimo_aggiornamento = adesso;

     /* Cambio fascia automatico in base all'ora virtuale raggiunta */
    if (ora->ora >= 13 && ora->ora < 18 && Turnoaula->orario == Mattina)
    {
        cambio_fascia_automatica(Turnoaula,s,coda, Pomeriggio);
    }
    else if ((ora->ora >= 18 || ora->ora < 9) && Turnoaula->orario == Pomeriggio)
        cambio_fascia_automatica(Turnoaula,s,coda, Sera);
}



void inizializza_strutture(NodoStudente *ListaStudenti,Turno* turnoAula, CodaAttesa* coda) {
    // 1. Controllo di sicurezza: evitiamo crash se passiamo puntatori NULL
    if (ListaStudenti == NULL || turnoAula == NULL || coda == NULL) {
        fprintf(stderr, "[ERRORE] Puntatori non validi in inizializzazione!\n");
        return;
    }

    // 2. INIZIALIZZA LA CODA D'ATTESA
    coda->head = NULL;
    coda->tail = NULL;
    coda->dimensione = 0;

    // 3. INIZIALIZZA L'AULA E IL TURNO
    turnoAula->posti_occupati = 0;

    // Contatori statistici azzerati
    turnoAula->prenotazioni  = 0;
    turnoAula->presenti      = 0;
    turnoAula->checkout      = 0;
    turnoAula->assenti       = 0;
    turnoAula->esplusi_dalla_coda = 0;
    turnoAula->accessi_effettivi[0]  = 0; /* MATTINA    */
    turnoAula->accessi_effettivi[1]  = 0; /* POMERIGGIO */
    turnoAula->accessi_effettivi[2]  = 0; /* SERA       */

    // Data reale di sistema (formato GG/MM/AAAA)
    {
        time_t t = time(NULL);
        struct tm* tm_info = localtime(&t);
        strftime(turnoAula->data, 11, "%d/%m/%Y", tm_info);
    }
    turnoAula->orario = Mattina; /* Fascia di default */

    {
        int i;
        for (i = 0; i < MAX_POSTI; i++) 
        {
            turnoAula->posti[i].stato = Libero;
            memset(turnoAula->posti[i].matricola, 0, 12);
        }
    }

    printf("[SISTEMA] Aula Studio pronta. Data: %s | Fascia: MATTINA\n\n", turnoAula->data);
}



void alloca_memoria_strutture(NodoStudente** ListaStudenti, Turno** Turnoaula, CodaAttesa** coda) {
    /* Allocazione delle strutture */
    *ListaStudenti = (NodoStudente*)malloc(sizeof(NodoStudente));
    *Turnoaula = (Turno*)malloc(sizeof(Turno));
    *coda = (CodaAttesa*)malloc(sizeof(CodaAttesa));

    if (*ListaStudenti == NULL || *Turnoaula == NULL || *coda == NULL) {
        fprintf(stderr, "[ERRORE CRITICO] Impossibile allocare le strutture principali.\n");
        exit(1);
    }

    inizializza_strutture(*ListaStudenti, *Turnoaula, *coda); }

