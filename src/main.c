#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "funzioni.h"

int main()
{
    /*
     * Le strutture sono dichiarate come PUNTATORI perché sono tipi opachi
     * (Information Hiding): il main non conosce la loro dimensione interna.
     * La memoria viene allocata dentro inizializza_sistema_dinamico.
     */
    NodoStudente* ListaStudenti  = NULL;
    Turno* turnoAula = NULL;
    CodaAttesa* coda = NULL;
    OrarioVirtuale ora_attuale = {9, 0, 0}; // Orario virtuale iniziale (default 09:00:00)
    time_t ultimo_controllo = time(NULL);

    // Inizializzazione dinamica: alloca le strutture e le azzera
    alloca_memoria_strutture(&ListaStudenti, &turnoAula, &coda);
    
    int scelta;
    char mat[20], nome[60], corso[60];

    do
    {
        aggiorna_orario_automatico(&ora_attuale, &ultimo_controllo, aula, coda);
        printf("\n--- STATO SISTEMA ---");
        printf("\nOra: %02d:%02d:%02d | Turno: %s", 
                ora_attuale.ora, ora_attuale.minuti, ora_attuale.secondi,
                (get_fascia_aula(aula) == MATTINA ? "MATTINA" : 
                (get_fascia_aula(aula) == POMERIGGIO ? "POMERIGGIO" : "SERA")));
        
        mostra_menu();
        if (scanf("%d", &scelta) != 1) {
            printf("Inserire un numero valido.\n");
            while(getchar() != '\n'); // Svuota buffer in caso di errore
            continue;
        }
        getchar(); // pulizia buffer

        switch(scelta)
        {
        case 1: /* REGISTRAZIONE */
        {
            printf("Matricola: "); scanf("%s", mat);
            getchar(); /* pulizia buffer dopo scanf */
            printf("Nome: "); fgets(nome, 60, stdin); nome[strcspn(nome, "\n")] = 0;
            printf("Corso: "); fgets(corso, 60, stdin); corso[strcspn(corso, "\n")] = 0; 
            
            /* Usiamo registra_studente perché Studente è un tipo opaco:
               il main non può dichiarare variabili Studente direttamente. */
            registra_studente(anagrafica, mat, nome, corso);
            break;
        }
        
        case 2: /* PRENOTAZIONE */
        {
            int scelta_f;
            printf("Matricola per prenotazione: ");
            scanf("%s", mat);

            /* Controllo anagrafica: se lo studente non è registrato,
               offriamo la registrazione immediata e poi continuiamo con la prenotazione. */
            if (cerca_studente(anagrafica, mat) == NULL) {
                char risp;
                printf("[!] Matricola %s non registrata. Vuoi registrarti ora? (s/n): ", mat);
                scanf(" %c", &risp);
                getchar(); /* pulizia buffer */

                if (risp == 's' || risp == 'S') {
                    char nome_preno[60], corso_preno[60];
                    printf("Nome e Cognome: ");
                    fgets(nome_preno, 60, stdin); nome_preno[strcspn(nome_preno, "\n")] = 0;
                    printf("Corso di Studi: ");
                    fgets(corso_preno, 60, stdin); corso_preno[strcspn(corso_preno, "\n")] = 0;

                    registra_studente(anagrafica, mat, nome_preno, corso_preno);
                    printf("[OK] Registrazione completata! Procedo con la prenotazione...\n");
                } else {
                    printf("Operazione annullata.\n");
                    break;
                }
            }

            /* Seleziona fascia oraria */
            printf("Seleziona Fascia (0: Mattina, 1: Pomeriggio, 2: Sera): ");
            scanf("%d", &scelta_f);

            /* Aggiorna la logica di assegnazione */
            FasciaOraria f_scelta;
            if (scelta_f == 0) f_scelta = MATTINA;
            else if (scelta_f == 1) f_scelta = POMERIGGIO;
            else f_scelta = SERA;

            effettua_prenotazione(anagrafica, aula, coda, mat, f_scelta, ora_attuale);
            break;
        }
        
        case 3: /* CHECK-IN */
        {
            printf("Matricola per check-in: ");
            scanf("%s", mat);
            
            if (cerca_studente(anagrafica, mat) == NULL) {
                char risp;
                printf("[!] Studente non trovato. Vuoi registrarlo ora? (s/n): ");
                scanf(" %c", &risp); 
                getchar(); /* pulizia buffer */

                if (risp == 's' || risp == 'S') {
                    char nome_nuovo[60], corso_nuovo[60];
                    printf("Nome e Cognome: ");
                    fgets(nome_nuovo, 60, stdin); nome_nuovo[strcspn(nome_nuovo, "\n")] = 0;
                    printf("Corso di Studi: ");
                    fgets(corso_nuovo, 60, stdin); corso_nuovo[strcspn(corso_nuovo, "\n")] = 0;
                    
                    /* Usiamo registra_studente perché Studente è un tipo opaco */
                    registra_studente(anagrafica, mat, nome_nuovo, corso_nuovo);
                    printf("[OK] Registrazione completata! Procedo col check-in...\n");
                    effettua_checkin(anagrafica, aula, coda, mat, ora_attuale);
                } else {
                    printf("Operazione annullata.\n");
                }
            } else {
                effettua_checkin(anagrafica, aula, coda, mat, ora_attuale);
            }
            break;
        }

        case 4: /* CHECK-OUT */
            printf("Matricola per check-out: "); scanf("%s", mat);
            effettua_checkout(anagrafica, aula, coda, mat, ora_attuale);
            break;

        case 5: /* VISUALIZZA STUDENTI PER STATO */
            visualizza_studenti_per_stato(anagrafica, aula, coda);
            break;

        case 6: /* ANNULLA PRENOTAZIONE */
            printf("Matricola per annullamento prenotazione: "); scanf("%s", mat);
            annulla_prenotazione(anagrafica, aula, coda, mat, ora_attuale);
            break;
 
        case 7: /* REPORT STATO AULA E STORICO */
            genera_report_aula(aula, coda);
            break;
        
        case 8: /* TEST AUTOMATICO */
            esegui_test_completo(anagrafica, aula, coda);
            break;

        case 0: 
            printf("Chiusura sistema...\n");
            libera_risorse(anagrafica, aula, coda);
            break;

        default: 
            printf("Scelta non valida.\n");
            break;
        }
    } while(scelta != 0);

    return 0;
}