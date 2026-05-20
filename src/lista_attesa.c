#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 16
#define ANNULAMENTO_PRENOTAZIONE 0
#define FINE_TURNO 1

/// @brief ADT per la lista d'attesa.
typedef struct lista
{
    /// @brief Matricola con cui si indentifica lo studente.
    int matricola;
    char nome[MAX_NOME];
    /// @brief Turno nel quale lo studente vuole prenotare l'aula studio.
    int turno;
    /// @brief Elemento successivo della lista.
    struct lista *successivo;
    /// @brief Elemento precedente della lista.
    struct lista *precedente;
} lista;

/// @brief Cancella un elemento nella lista mantendeo i collegamenti.
/// @param Lista Indirizzo dell'elemento da cancellare.
/// @return Restituisce l'elemento successivo a quello che è stato eliminato se esiste, altrimenti l'elemento precedente.
lista *cancella(lista *elemento)
{
    if (elemento->precedente == NULL && elemento->successivo == NULL)
    {
        free(elemento);
        return NULL;
    }
    else if (elemento->successivo == NULL)
    {
        elemento = elemento->precedente;
        free(elemento->successivo);
        elemento->successivo = NULL;
        return elemento;
    }
    else if (elemento->precedente == NULL)
    {
        elemento = elemento->successivo;
        free(elemento->precedente);
        elemento->precedente = NULL;
        return elemento;
    }
    else
    {
        lista *temp;
        temp = elemento->successivo;
        elemento = elemento->precedente;
        free(elemento->successivo);
        elemento->successivo = temp;
        temp = elemento;
        elemento = elemento->successivo;
        elemento->precedente = temp;
        return elemento;
    }
}

/// @brief Permetti di tornare indetro al primo elemento della lista.
/// @param Lista Consiste nella lista da poter navigare.
/// @return L'indirizzo del primo elemento nella lista.
lista *cima_lista(lista *elemento)
{
    while (elemento->precedente != NULL)
    {
        if (elemento->precedente != NULL)
        {
            elemento = elemento->precedente;
        }
    }
    return elemento;
}

/// @brief Questa funzione si preoccupa di cercare gli elementi da eliminare.
/// @param Lista Lista in cui cercare gli elemnti da eliminare.
/// @param Motivo Indica il criterio che deve seguire per la eliminazione.
/// @param Matricola Matricola di cui annullare la prenotazione.
/// @param Turno Turno di cui cancellare le persone rimaste in lista d'attesa.
/// @return Restituisce la lista modificata.
lista *rimozione_elementi(lista *parte, int motivo, int matricola, int turno)
{
    switch (motivo)
    {
    case ANNULAMENTO_PRENOTAZIONE:
    {
        int termine = 0;
        parte = cima_lista(parte);
        while (termine == 0)
        {
            if (parte->matricola == matricola)
            {
                cancella(parte);
                termine = 1;
            }
            if (parte->successivo == NULL && termine != 1)
            {
                termine = 2;
            }
            parte = parte->successivo;
        }
        switch (termine)
        {
        case 1:
            printf("La sua prenotazione e' stata rimossa con successo!\n");
            break;
        case 2:
            printf("La sua prenotazione non esiste.\n");
            break;
        default:
            // Toricamente questo caso non è possibile, poiche sono dei valori gestiti internamente.
            break;
        }
        return parte;
    }
    break;
    case FINE_TURNO:
    {
        lista *tmp;
        int fine = 0;
        parte = cima_lista(parte);
        do
        {
            tmp = parte->successivo;
            if (parte->turno == turno)
            {
                parte = cancella(parte);
            }
            if (parte == NULL || parte->successivo == NULL && tmp == NULL)
            {
                fine = 1;
            }
            else if (parte->successivo != NULL)
            {
                parte = parte->successivo;
            }
        } while (fine == 0);
        return parte;
    }
    break;
    default:
        // Non gestito perché essendo una funzione gestita internamente, non può avere valori non previsti.
        return NULL;
        break;
    }
}

/// @brief Permette di annullare una prenotazione.
/// @param Lista Lista da cui annullare la prenotazione.
/// @param Matricola Matricola di cui annullare la prenotazione.
/// @return Inizio della lista d'attesa con le eliminazioni effettuate.
lista *elim_preontazione(lista *parte, int matricola)
{
    if (parte == NULL)
    {
        return NULL;
    }
    parte = rimozione_elementi(parte, ANNULAMENTO_PRENOTAZIONE, matricola, 0);
    if (parte == NULL)
    {
        return NULL;
    }
    return cima_lista(parte);
}

/// @brief Permette di cancellare le prenotazioni del turno appena concluso.
/// @param Lista Lista da cui annullare la prenotazione.
/// @param Turno Indica il turno di cui bisogna cancellare le prenotazioni.
/// @return Inizio della lista d'attesa con le eliminazioni effettuate.
lista *chiudi_turno(lista *parte, int turno)
{
    if (parte == NULL)
    {
        return NULL;
    }
    parte = rimozione_elementi(parte, FINE_TURNO, 0, turno);
    if (parte == NULL)
    {
        return NULL;
    }
    return cima_lista(parte);
}

/// @brief Cancella completamente la lista
/// @param Lista Lista da eliminare complletamente
void cancella_lista(lista *parte)
{
    while (parte != NULL)
    {
        parte = cancella(parte);
    }
}

/// @brief Permentte di inizializzare una lista
/// @return Restituisce il primo elemento di una lista
lista *inizializza_lista()
{
    lista *parte = (lista *)malloc(sizeof(lista));
    parte->matricola = 0;
    parte->precedente = NULL;
    parte->successivo = NULL;
    parte->turno = 0;
    return parte;
}

/// @brief Genera un nuovo elemento vergine della lista
/// @param Lista Lista in cui bisogna aggiungere un pezzo
/// @return Restituisce l'inidirizzo dell'elemento nuovo
lista *nuovo_elemento(lista *parte)
{
    if (parte == NULL)
    {
        return inizializza_lista();
    }
    while (parte->successivo != NULL)
    {
        parte = parte->successivo;
    }
    lista *tmp = parte;
    parte->successivo = (lista *)malloc(sizeof(lista));
    parte = parte->successivo;
    parte->successivo = NULL;
    parte->precedente = tmp;
    parte->matricola = 0;
    parte->turno = 0;
    return parte;
}

/// @brief Permette di aggiungere una nuova prenotazione alla coda
/// @param Lista Lista in cui aggiungere il nuovo elemento
/// @param Nome Nome dello studente che sta effettuando la prenotazione
/// @param Turno Turno nel quale si sta prenotando lo studente
/// @param Matricola Matricola dello studente
/// @return Restituisce l'indirizzo del nuovo elemento salvato nella lista d'attesa
lista *nuova_aggiunta(lista *parte, char *nome, int turno, int matricola)
{
    if (parte != NULL)
    {
        if (parte->nome != NULL && parte->matricola != 0 && parte->turno != 0)
        {
            parte = nuovo_elemento(parte);
        }
    }
    else
    {
        parte = nuovo_elemento(parte);
    }
    strcpy_s(parte->nome, MAX_NOME, nome);
    parte->turno = turno;
    parte->matricola = matricola;
    return parte;
}

/// @brief Permette di vedere tutti gli studenti in coda
/// @param Lista Lista dalla quale stampare gli studenti
void visualizza_lista(lista *elemento)
{
    int termina = 0;
    elemento = cima_lista(elemento);
    printf("------\n");
    do
    {
        printf("Nome: %s\n", elemento->nome);
        printf("Matricola: %d\n", elemento->matricola);
        printf("Turno: %d\n", elemento->turno);
        printf("------\n");
        if (elemento->successivo != NULL)
        {
            elemento = elemento->successivo;
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
lista *cerca(lista *elemento, int matricola)
{
    int presente = 0;
    elemento = cima_lista(elemento);
    do
    {
        if (elemento->matricola == matricola)
        {
            presente = 1;
        }
        else
        {
            elemento = elemento->successivo;
        }
    } while (elemento->successivo != NULL && presente == 0);
    if (presente == 1)
    {
        return elemento;
    }
    printf("Non è stato trovato lo studente!\n");
    return cima_lista(elemento);
}

/// @brief Calcola il numero di studneti in attesa
/// @param Lista Lista id cui calcolare gli studenti in attesa
/// @return Restituisce un intero che indica il numero di studenti in attesa
int qta_in_attesa(lista *elemento)
{
    int qta = 0;
    do
    {
        qta++;
        if (elemento->successivo != NULL)
        {
            elemento = elemento->successivo;
        }
    } while (elemento->successivo != NULL);
    return qta;
}

/// @brief Permette di vedere la matricola dello studente nel punto dato della lista
/// @param Lista Elemento di cui vedere la matricola
/// @return Matricola dello studente in data posizione della lista
int ricevi_matricola(lista *elemento)
{
    return elemento->matricola;
}

int ricevi_turno(lista *elemento)
{
    return elemento->turno;
}