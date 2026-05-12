
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
    char *Matricola[12];
    char *Nome[16];
    char *CorsoLaurea;

} Studente;
// nodo dell'adt lista degli studenti//
typedef struct
{

    Studente dato;
    struct NodoStudente *next;
    struct NodoStudente *prec;

} NodoStudente;
// struttura che definisce il posto prenotato nell'aula//
typedef struct
{
    char *matricola[12];
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

/* crea un nuovo nodo */
NodoStudente *creaNodo(Studente s)
{
    NodoStudente *nuovo = malloc(sizeof(NodoStudente));

    if (nuovo == NULL)
    {
        printf("Errore allocazione memoria\n");
        return NULL;
    }

    nuovo->dato = s;
    nuovo->next = NULL;

    return nuovo;
}

/* inserimento in testa */
NodoStudente *inserisciTesta(NodoStudente *testa, Studente s)
{
    NodoStudente *nuovo = creaNodo(s);

    if (nuovo == NULL)
        return testa;

    nuovo->next = testa;
    nuovo->prec=NULL;
    return nuovo;
}

/* stampa lista */
void stampaLista(NodoStudente *testa)
{
    NodoStudente *corrente = testa;

    while (corrente != NULL)
    {

        printf("Nome: %s\n", corrente->dato.Nome);
        printf("Matricola: %d\n", corrente->dato.Matricola);
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
    char *s_nome[16];
    strcpy_s(s_nome, 16, studenti.Nome);

    return s_nome;
}

char *get_matricola(Studente studenti)
{
    char *s_matricola[12];
    strcpy(s_matricola, studenti.Matricola);

    return s_matricola;
}

void Inserisci_Studente(Studente studenti)
{

    scanf("%s", studenti.Nome);
    scanf("%s", studenti.Matricola);
    scanf("%s", studenti.CorsoLaurea);
}

Studente *cerca_Studente(Studente *studenti, char *matricola)
{

    if (strcmp(studenti->Matricola, matricola) == 0)
    {
        return studenti;
    }
    else
    {
        return NULL;
    }
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
  
  {
  if(strcmp(turnoAula->posti[i].matricola,ListaStudenti->dato[i].matricola)==0 )

  }


}