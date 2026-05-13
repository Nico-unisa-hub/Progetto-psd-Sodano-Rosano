
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_POSTI 100

typedef enum
{
    Prenotato,
    Libero

} StatoPosto;



typedef enum
{

    Mattina,
    Pomeriggio,
    Sera

} OrarioAula;

typedef enum
{

    presente,
    assente,
    attesa

} StatoStudente;
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
    char *data;
    OrarioAula orario;
    Posti posti[MAX_POSTI];
} Turno;


/* =========================
   FUNZIONI LISTA
   ========================= */

/* stampa lista */
void stampaLista(NodoStudente *testa)
{
    NodoStudente *corrente = testa;

    while (corrente != NULL)
    {

        printf("Nome: %s\n", corrente->dato.Nome);
        printf("Matricola: %s\n", corrente->dato.Matricola);
        printf("------------------\n");

        corrente = corrente->next;
    }
}

/* libera memoria */
void liberaLista(NodoStudente *testa)
{
    NodoStudente *tmp;

    while (testa != NULL)
    {
        tmp = testa;
        testa = testa->next;
        free(tmp);
    }
}

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

void Inserisci_Studente(NodoStudente *nodo, Studente studenti)
{

    //cerca dello studente//
   NodoStudente* nodo_temp = (NodoStudente*) malloc(sizeof(NodoStudente));
    if(nodo_temp == NULL)
    {
        printf("Errore allocazione memoria\n");
        exit(0);
    }

    nodo_temp->dato = studenti;

    if(nodo == NULL)
    {
        nodo = nodo_temp;
    }
    else
    {
        NodoStudente* temp = nodo;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = nodo_temp;
        nodo_temp->prec = temp;
    }

   

}

Studente* cerca_Studente(NodoStudente *testa, char *matricola)
{

   while(testa != NULL)
    {
        if(strcmp(testa->dato.Matricola, matricola)==0)
            return testa;

        testa = testa->next;
    }
    return NULL;
}

void Effettua_Prenotazione(char *matricola, char *data, OrarioAula orario, Turno *turnoAula)
{
    if (turnoAula->posti_occupati >= MAX_POSTI)
    {

        printf("Non ci sono posti disponibili per questo turno.Verrai spostato nellal ista d'attesa\n");
        return;
    }
    else
    {
        for (int i = 0; i < MAX_POSTI; i++)
        {
            if (turnoAula->posti[i].stato == Libero)
            {

                turnoAula->posti[i].stato = Prenotato;

                turnoAula->data = data;

                turnoAula->orario = orario;

                strcpy(turnoAula->posti[i].matricola, matricola);

                turnoAula->posti_occupati++;
                printf("Prenotazione effettuata con successo per lo studente con matricola %s\n", matricola);
            }
        }
    }
}

void stampa_turno(char *data, OrarioAula orario, Turno turnoAula)
{

    printf("Elenco del turno per la data %s e l'orario %d:\n", turnoAula.data, orario);
    if (strcmp(turnoAula.data, data) == 0 && turnoAula.orario == orario)
    {
        for (int i = 0; i < MAX_POSTI; i++)
        {
            if (turnoAula.posti[i].stato == Prenotato)
            {
                printf("Posto %d: Matricola %s\n", i + 1, turnoAula.posti[i].matricola);
            }
        }
    }
    else
    {
        printf("Non ci sono prenotazioni per la data %s e l'orario %d.\n", data, orario);
    }
}

void check_in_studenti(char *data,Turno turnoAula,NodoStudente* ListaStudenti)
{
  for(int i=0;i<MAX_POSTI;i++)
  
  
  


}