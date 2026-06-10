#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "funzioni.h"

int main(void)
{
    NodoStudente *ListaStudenti = NULL;
    Turno        *turnoAula     = NULL;
    CodaAttesa   *coda          = NULL;
    OrarioVirtuale ora_attuale  = {9, 0, 0};
    time_t ultimo_controllo     = time(NULL);

    /* Allocazione e inizializzazione delle strutture */
    alloca_memoria_strutture(&ListaStudenti, &turnoAula, &coda);

    int  scelta = -1;   /* CORREZIONE: variabile inizializzata prima del do-while */
    char mat[MAX_MATRICOLA], nome[MAX_NOME], corso[MAX_CORSO_LAU];

    do
    {
        aggiorna_orario_automatico(&ora_attuale, &ultimo_controllo, turnoAula, coda, ListaStudenti);

        printf("\n--- STATO SISTEMA ---\n");
        printf("Ora: %02d:%02d:%02d | Turno: %s\n",
               ora_attuale.ora, ora_attuale.minuti, ora_attuale.secondi,
               (get_fascia_aula(turnoAula) == Mattina    ? "MATTINA" :
                get_fascia_aula(turnoAula) == Pomeriggio ? "POMERIGGIO" : "SERA"));

        printf("\nDigita 1 per effettuare la registrazione\n");
        printf("Digita 2 per effettuare la prenotazione\n");
        printf("Digita 3 per effettuare il check-in\n");
        printf("Digita 4 per effettuare il check-out\n");
        printf("Digita 5 per visualizzare lo stato della classe\n");
        printf("Digita 6 per annullare la prenotazione\n");
        printf("Digita 7 per vedere il report dell'aula e lo storico\n");
        printf("Digita 0 per uscire\n");
        printf("> ");

        /* CORREZIONE: scanf aggiunto per leggere la scelta */
        scanf("%d", &scelta);

        switch (scelta)
        {
        case 1: /* REGISTRAZIONE */
        {
            printf("Matricola: "); scanf("%11s", mat);
            getchar();
            printf("Nome: ");     fgets(nome,  sizeof(nome),  stdin); nome[strcspn(nome,  "\n")] = 0;
            printf("Corso: ");    fgets(corso, sizeof(corso), stdin); corso[strcspn(corso, "\n")] = 0;

            /* CORREZIONE: ordine parametri allineato alla firma (lista, nome, matricola, corso) */
            Registra_Studente(ListaStudenti, nome, mat, corso);
            break;
        }

        case 2: /* PRENOTAZIONE */
        {
            int scelta_f;
            printf("Matricola per prenotazione: ");
            scanf("%11s", mat);

            /* CORREZIONE: nome funzione unificato a cerca_Studente (con S maiuscola) */
            if (cerca_Studente(ListaStudenti, mat) == NULL)
            {
                char risp;
                printf("[!] Matricola %s non registrata. Vuoi registrarti ora? (s/n): ", mat);
                scanf(" %c", &risp);
                getchar(); 

                if (risp == 's' || risp == 'S')
                {
                    char nome_preno[MAX_NOME], corso_preno[MAX_CORSO_LAU];
                    printf("Nome e Cognome: ");
                    fgets(nome_preno, sizeof(nome_preno), stdin); nome_preno[strcspn(nome_preno, "\n")] = 0;
                    printf("Corso di Studi: ");
                    fgets(corso_preno, sizeof(corso_preno), stdin); corso_preno[strcspn(corso_preno, "\n")] = 0;

                    Registra_Studente(ListaStudenti, nome_preno, mat, corso_preno);
                    printf("[OK] Registrazione completata!\n");
                }
                else
                {
                    printf("Operazione annullata.\n");
                    break;
                }
            }

            printf("Seleziona Fascia (0: Mattina, 1: Pomeriggio, 2: Sera): ");
            scanf("%d", &scelta_f);

            OrarioAula f_scelta;
            if      (scelta_f == 0) f_scelta = Mattina;
            else if (scelta_f == 1) f_scelta = Pomeriggio;
            else                    f_scelta = Sera;

            effettua_prenotazione(ListaStudenti, turnoAula, coda, mat, f_scelta, ora_attuale);
            break;
        }

        case 3: /* CHECK-IN */
        {
            printf("Matricola per check-in: ");
            scanf("%11s", mat);

            if (cerca_Studente(ListaStudenti, mat) == NULL)
            {
                char risp;
                printf("[!] Studente non trovato. Vuoi registrarlo ora? (s/n): ");
                scanf(" %c", &risp);
                getchar();

                if (risp == 's' || risp == 'S')
                {
                    char nome_nuovo[MAX_NOME], corso_nuovo[MAX_CORSO_LAU];
                    printf("Nome e Cognome: ");
                    fgets(nome_nuovo, sizeof(nome_nuovo), stdin); nome_nuovo[strcspn(nome_nuovo, "\n")] = 0;
                    printf("Corso di Studi: ");
                    fgets(corso_nuovo, sizeof(corso_nuovo), stdin); corso_nuovo[strcspn(corso_nuovo, "\n")] = 0;

                    /* CORREZIONE: nome funzione unificato a Registra_Studente */
                    Registra_Studente(ListaStudenti, nome_nuovo, mat, corso_nuovo);
                    printf("[OK] Registrazione completata!\n");
                }
                else
                {
                    printf("Operazione annullata.\n");
                    break;
                }
            }
            check_in_studenti(turnoAula, ListaStudenti, coda, mat, ora_attuale);
            break;
        }

        case 4: /* CHECK-OUT */
            printf("Matricola per check-out: "); scanf("%11s", mat);
            check_out_studenti(ListaStudenti, coda, turnoAula, ora_attuale, mat);
            break;

        case 5: /* VISUALIZZA STUDENTI PER STATO */
            visualizza_studenti_per_stato(ListaStudenti, turnoAula, coda);
            break;

        case 6: /* ANNULLA PRENOTAZIONE */
            printf("Matricola per annullamento: "); scanf("%11s", mat);
            /* CORREZIONE: ordine parametri allineato alla firma */
            annulla_prenotazione(mat, turnoAula, coda, ListaStudenti, ora_attuale);
            break;

        case 7: /* REPORT */
            report(turnoAula, coda);
            break;

        case 0:
            printf("Chiusura sistema...\n");
            break;

        default:
            printf("Scelta non valida.\n");
            break;
        }
    }
    while (scelta != 0);

    return 0;
}