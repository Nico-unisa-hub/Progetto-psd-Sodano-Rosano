
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Funzioni.h"
/*#include "lista.h"
#include "lista.c"*/  
#define MAX_POSTI 100

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
    char Matricola[12];
    char Nome[16];
    char CorsoLaurea;

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
    OrarioAula orario;
    Posti posti[MAX_POSTI];
} Turno;


typedef struct 
{
    /// @brief Struttura che contiene i dati dello studente.
    Studente studente;
    OrarioAula orario;
    /// @brief Elemento sucessivo della lista.
    struct  CodaAttesa *head;
    /// @brief Elemento precedente della lista.
    struct CodaAttesa *tail;
} CodaAttesa;





void cambio_fascia_automatica(Turno* Turnoaula, CodaAttesa* coda, OrarioAula nuova_fascia) {
    //questa funzione fa anche da cambio turno, commentare//
    OrarioVirtuale ora_zero = {0, 0, 0};

    printf("\n[SISTEMA] Cambio fascia rilevato! Chiusura turno: %s -> %s\n",
           (Turnoaula->orario == Mattina ? "MATTINA" : (Turnoaula->orario == Pomeriggio ? "POMERIGGIO" : "SERA")),
           (nuova_fascia == Pomeriggio ? "POMERIGGIO" : "SERA"));

    // --- 1. Conteggio assenti: prenotati che non si sono mai presentati ---
    for (int i = 0; i < MAX_POSTI; i++) {
        if (Turnoaula->posti[i].stato == Prenotato) {
            Turnoaula->assenti++;
            salva_storico_accesso(NULL, Turnoaula->posti[i].matricola, "NO-SHOW (FINE TURNO)", ora_zero);
        } else if (Turnoaula->posti[i].stato == Occupato) {
            // Chi era ancora dentro a fine turno: checkout forzato
            salva_storico_accesso(NULL, Turnoaula->posti[i].matricola, "CHECKOUT AUTOMATICO (FINE TURNO)", ora_zero);
        }
        Turnoaula->posti[i].stato = Libero;
        strcpy(Turnoaula->posti[i].matricola, "");
    }

    // --- 2. Espulsione selettiva dalla coda:
    //         Rimuove SOLO i nodi con fascia == fascia corrente (turno appena concluso).
    //         Chi ha prenotato per un turno futuro rimane in coda e sara' servito
    //         all'apertura del turno corrispondente.
    {
        const char* fascia_conclusa = (Turnoaula->orario == Mattina ? "MATTINA"
                                     : Turnoaula->orario == Pomeriggio ? "POMERIGGIO" : "SERA");
        const char* fascia_nuova    = (nuova_fascia == Mattina ? "MATTINA"
                                     : nuova_fascia == Pomeriggio ? "POMERIGGIO" : "SERA");
        int n_espulsi = 0;
        int n_rimasti = 0;

        CodaAttesa* temp_curr = coda->head;
        CodaAttesa* temp_prec = NULL;
        while (temp_curr != NULL) {
            CodaAttesa* next = temp_curr->next;
            if (temp_curr->orario == Turnoaula->orario) {
                /* Prenotato per il turno appena concluso: viene rimosso */
                Turnoaula->esplusi_dalla_coda++;
                n_espulsi++;
                salva_storico_accesso(NULL, temp_curr->matricola,
                                      "ESPULSO DA CODA (FINE TURNO)", ora_zero);
                printf("  [RIMOSSO] %s era in coda per %s (turno concluso).\n",
                       curr->matricola, fascia_conclusa);
                if (prev == NULL) coda->head = next;
                else              prev->next  = next;
                if (coda->tail == curr) coda->tail = prev;
                coda->dimensione--;
                free(curr);
            } else {
                /* Prenotato per un turno futuro: rimane in coda */
                n_rimasti++;
                printf("  [MANTENUTO] %s rimane in coda per la fascia %s.\n",
                       temp_curr->matricola,
                       (curr->fascia == Mattina ? "MATTINA"
                       : curr->fascia == Pomeriggio ? "POMERIGGIO" : "SERA"));
                temp_prev = temp_curr;
            }
            temp_curr = next;
        }

        printf("[CODA] Pulizia completata: %d rimossi (fascia %s), %d mantenuti per %s.\n",
               n_espulsi, fascia_conclusa, n_rimasti, fascia_nuova);
    }

    // --- 3. Reset contatori e posti per il nuovo turno ---
    {
        int i;
        for (i = 0; i < MAX_POSTI; i++) {
            Turnoaula->posti[i].stato = Libero;
            memset(Turnoaula->posti[i].matricola, 0, 12);
        }
    }
    Turnoaula->posti_occupati      = 0;
    Turnoaula->prenotazioni = 0;
    Turnoaula->presenti      = 0;
    Turnoaula->checkout     = 0;
    Turnoaula->assenti      = 0;
    Turnoaula->esplusi_dalla_coda = 0;

    // Aggiorna la fascia
    aula->fascia = nuova_fascia;

    // --- 4. Promozione automatica dalla coda:
    //         Chi aveva prenotato in anticipo per la nuova fascia ottiene
    //         subito un posto fisico (PRENOTATO), senza dover fare check-in manuale.
    //         Vengono promossi in ordine FIFO fino a riempire i posti disponibili.
    {
        int promossi = 0;
        NodoAttesa* curr = coda->head;
        NodoAttesa* prev = NULL;

        while (curr != NULL && aula->posti_occupati < MAX_POSTI) {
            NodoAttesa* next = curr->next;
            if (curr->fascia == nuova_fascia) {
                /* Trova il primo posto libero */
                int i;
                for (i = 0; i < MAX_POSTI; i++) {
                    if (aula->posti[i].stato == LIBERO) {
                        OrarioVirtuale ora_zero = {0, 0, 0};
                        aula->posti[i].stato = PRENOTATO;
                        strncpy(aula->posti[i].matricola_studente, curr->matricola, 11);
                        aula->posti[i].matricola_studente[11] = '\0';
                        aula->posti[i].ora_prenotazione = ora_zero;
                        aula->posti_occupati++;
                        aula->totale_prenotazioni++;
                        promossi++;

                        printf("  [PROMOSSO] %s: dalla coda al posto %d per fascia %s.\n",
                               curr->matricola, i + 1,
                               (nuova_fascia == Pomeriggio ? "POMERIGGIO" : "SERA"));

                        /* Rimuovi il nodo dalla coda */
                        if (prev == NULL) coda->head = next;
                        else              prev->next  = next;
                        if (coda->tail == curr) coda->tail = prev;
                        coda->dimensione--;
                        free(curr);
                        break;
                    }
                }
            } else {
                prev = curr;
            }
            curr = next;
        }

        if (promossi > 0)
            printf("[SISTEMA] %d studenti promossi dalla coda ai posti per fascia %s.\n",
                   promossi, (nuova_fascia == Pomeriggio ? "POMERIGGIO" : "SERA"));
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

long orario_in_secondi(OrarioVirtuale o) {
    return (long)o.ora * 3600 + o.minuti * 60 + o.secondi;
}

/* is_orario_valido:
 * Verifica se l'orario virtuale rientra nei limiti della fascia indicata.
 * Pre:  fascia è MATTINA, POMERIGGIO o SERA.
 * Post: restituisce 1 se l'ora è nella fascia, 0 altrimenti.
 */
int is_orario_valido(OrarioVirtuale adesso, OrarioAula orario) {
    long s = orario_in_secondi(adesso);
    if      (orario == Mattina)    return (s >= 32400 && s <= 46800); /* 09:00-13:00 */
    else if (orario == Pomeriggio) return (s >= 50400 && s <= 64800); /* 14:00-18:00 */
    else if (orario == Sera)       return (s >= 64800 && s <= 79200); /* 18:00-22:00 */
    return 0;
}

/* aggiorna_orario_automatico:
 * Avanza l'orario virtuale in base al tempo reale trascorso (120x).
 * Controlla se è necessario un cambio fascia automatico e lo esegue.
 * Pre:  ora, ultimo_aggiornamento, aula, coda non NULL.
 * Post: ora viene aggiornata; se la fascia è cambiata viene chiamata
 *       cambio_fascia_automatica con la nuova fascia corretta.
 */
void aggiorna_orario_automatico(OrarioVirtuale* ora, time_t* ultimo_aggiornamento, Turno* Turnoaula) {
    time_t  adesso = time(NULL);
    double  secondi_trascorsi = difftime(adesso, *ultimo_aggiornamento);

    if (secondi_trascorsi < 1.0) return;

    /* Avanza l'orologio virtuale secondo per secondo (120x) */
    int secondi_virtuali = (int)secondi_trascorsi * 120;
    int i;
    for (i = 0; i < secondi_virtuali; i++) {
        ora->secondi++;
        if (ora->secondi >= 60) { ora->secondi = 0; ora->minuti++;  }
        if (ora->minuti  >= 60) { ora->minuti  = 0; ora->ora++;     }
        if (ora->ora     >= 24) { ora->ora     = 0;                 }
    }

    *ultimo_aggiornamento = adesso;

    /* Cambio fascia automatico in base all'ora virtuale raggiunta */
    if (ora->ora >= 13 && ora->ora < 18 && Turnoaula->orario == Mattina)
        cambio_fascia_automatica(Turnoaula, Pomeriggio);
    else if ((ora->ora >= 18 || ora->ora < 9) && Turnoaula->orario == Pomeriggio)
        cambio_fascia_automatica(Turnoaula, Sera);
}


/* =========================
   FUNZIONI LISTA STUDENTI
   ========================= */


   
char *get_Nome(Studente studenti)
{
    return studenti.Nome;
}

char *get_matricola(Studente studenti)
{
    char *s_matricola[12];
    strcpy(s_matricola, studenti.Matricola);

    return s_matricola;
}



void Inserisci_Studente(NodoStudente *nodo, Studente s)
{

    // controllo se lo studente è già presente//

    if (cerca_Studente(nodo, s.Matricola) != NULL)
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



Studente *cerca_Studente(NodoStudente *testa, char *matricola)
{

    while (testa != NULL)
    {
        if (strcmp(testa->dato.Matricola, matricola) == 0)
            return testa;

        testa = testa->next;
    }
    return NULL;
}




void crea_studente(Studente *s, char *matricola, char *nome, char corsoLaurea)
{
    strcpy(s->Matricola, matricola);
    strcpy(s->Nome, nome);
    s->CorsoLaurea = corsoLaurea;
}



/* =========================
   FUNZIONI GESTIONE PRENOTAZIONI 
   ========================= */


void Effettua_Prenotazione(char *matricola, OrarioAula* orario, Turno *turnoAula, CodaAttesa *coda)
{
    /* controllo se la matricola esiste già, caso negativo deve fare la registrazione e procede alla prenotazione*/


    if (turnoAula->posti_occupati >= MAX_POSTI)
    {
        printf("Non ci sono posti disponibili per questo turno.Verrai spostato nellal ista d'attesa\n");
        //funzione che aggiunge in lista d'attesa lo studnete//

        return;
    }
    else
    {
        for (int i = 0; i < MAX_POSTI; i++)
        {
            if (turnoAula->posti[i].stato == Libero)
            {

                turnoAula->posti[i].stato = Prenotato;

                turnoAula->orario = *orario;

                strcpy(turnoAula->posti[i].matricola, matricola);

                turnoAula->posti_occupati++;
                printf("Prenotazione effettuata con successo per lo studente con matricola %s\n", matricola);
            }

     
            
        }



    }
}




int check_in_studenti(OrarioAula* orario, Turno* turnoAula, NodoStudente *ListaStudenti,time_t* tempo)
{
   /* controllo se esiste gia la matricola, se non esiste fa la registrazione, se c'è un posto libero lo butto dentro
   altrimenti va in coda attesa*/
    for (int i = 0; i < MAX_POSTI; i++)
    {
        if (turnoAula->orario == *orario && turnoAula->posti[i].stato == Prenotato)
        {

            Studente *studente = cerca_Studente(ListaStudenti, turnoAula->posti[i].matricola);

            if (studente != NULL)
            {
                printf("Check-in effettuato per lo studente con matricola %s\n", studente->Matricola);
            }
            else
            {
                printf("Lo studente con matricola %s non è presente nella lista delle prenotazioni.\n", turnoAula->posti[i].matricola);
            }
        }
        else
        {

            printf("lo studente con matricola %s non è presente.\n", turnoAula->posti[i].matricola);
        }
    }
}


// gestione della stampa della lsta studenti da fare//

void annulla_prenotazione(char *matricola, char *data, OrarioAula* orario, Turno *turnoAula)
{
    for (int i = 0; i < MAX_POSTI; i++)
    {
        if (strcmp(turnoAula->posti[i].matricola, matricola) == 0 && turnoAula->orario == *orario)
        {
            turnoAula->posti[i].stato = Libero;
            turnoAula->posti_occupati--;
            printf("Prenotazione annullata con successo per lo studente con matricola %s\n", matricola);
            return;
        }
    }
    printf("Non è stata trovata alcuna prenotazione per lo studente con matricola %s nella data %s e nell'orario %d.\n", matricola, data, orario);
}

void disponibilita_posti(OrarioAula* orario, Turno *turnoAula)
{

    if (turnoAula->orario == *orario)
    {
        int posti_disponibili = MAX_POSTI - turnoAula->posti_occupati;
        printf("Ci sono %d posti disponibili l'orario %d.\n", posti_disponibili, *orario);
    }
    else
    {
        printf("Non ci sono prenotazioni per e l'orario %d.\n", *orario);
    }

}



void check_out_studenti(char *matricola, OrarioAula orario, OrarioVirtuale* orario_virtuale,time_t* ultimo_aggiornamento,Turno *turnoAula, CodaAttesa *coda)
{

    /*ricerca dello studente da togliere, scrivi nello storico l'uscita(aggiungendo l'ora di uscita),
    se c'è qualcuno nella coda di attesa buttacelo dentro*/
    for(int i=0;i<MAX_POSTI;i++){


    }

}




void report(Turno *turnoAula, NodoStudente *ListaStudenti, CodaAttesa *coda,OrarioAula orario) {
  int ingressi = 0;
  int prenotazioni = 0;
  int perc_occupazione= 0;
  int assenti = 0;
  int in_attesa= 0;
  CodaAttesa *temp_coda = coda;


  for (int i = 0; i < MAX_POSTI; i++) {
   
   if(turnoAula->posti[i].stato==Prenotato && turnoAula->orario==orario){
    prenotazioni++;
    if(check_in_studenti(&orario, turnoAula, ListaStudenti)){
        ingressi++;
    } else {
        assenti++;
    }

   }
        
        
    }
    
  

  while(temp_coda != NULL) {
   in_attesa++;
    temp_coda = temp_coda->sucessivo;
  } 
  
  perc_occupazione= (prenotazioni * 100) / MAX_POSTI;

  printf("report:\n");
  printf("il numero di studenti prenotati è:%d\n",prenotazioni);
  printf("il numero di studenti in attesa è: %d\n", in_attesa);
  printf("il humero di ingressi in questo turno è:%d ",ingressi);
  printf("il numero di studenti non presenti è:%d\n", assenti);
  printf("la percentuale di occupazione in questa fascia oraria è:% %d",perc_occupazione);


}


void leggi_storico()
{
   FILE* file = fopen("storico.txt","r");
    char* riga;
    size_t len=0;
    while (getLine(&riga,&len,file)!= -1)
   {
    printf("%s",riga);
   }    

}



void Scrivi_storico(Turno* turnoaula,NodoStudente* ListaStudenti,CodaAttesa* coda, OrarioAula orario,time_t* adesso)
{

// da fare domani //6

    FILE*  file = fopen("storico.txt","a");
    fprintf("Nome:%s \n",ListaStudenti->dato.Nome);
    fprintf("Matricola:%s\n", ListaStudenti->dato.Matricola);    
    fprintf("Corso di laurea:%s\n",ListaStudenti->dato.CorsoLaurea);
    fprintf("");
    fprintf("/-------------------------------/");

    
    printf("storico aggiornato");
    

    

}


