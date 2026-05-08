#include <stdlib.h>
#include <string.h>

#define MAX_NOME 16
#define ANNULAMENTO_PRENOTAZIONE 0
#define FINE_TURNO 1

/// @brief ADT per la lista d'attesa.
typedef struct 
{
    /// @brief Matricola con cui si indentifica lo studente.
    int matricola;
    char nome[MAX_NOME];
    /// @brief Turno nel quale lo studente vuole prenotare l'aula studio.
    int turno;
    /// @brief Elemento sucessivo della lista.
    lista *sucessivo;
    /// @brief Elemento precedente della lista.
    lista *precedente;
} lista;

/// @brief Cancella un elemento nella lista mantendeo i collegamenti.
/// @param Lista Indirizzo dell'elemento da cancellare.
/// @return Restituisce l'elemento sucessivo a quello che è stato eliminato se esiste, altrimenti l'elemento precedente.
lista *cancella(lista *elemento)
{
    lista *temp;
    if (elemento->precedente == NULL)
    {
        // Per gestire il cambio dell'indirizzo del primo elemento della lista si potrebbe restituire sempre il primo elemento.
        temp = elemento->sucessivo;
        free(elemento);
        elemento = temp;
        elemento->precedente = NULL;
        return elemento;
    }
    else if (elemento->sucessivo == NULL)
    {
        elemento = elemento->precedente;
        free(elemento->sucessivo);
        elemento->sucessivo == NULL;
        return elemento;
    }
    else if (elemento->precedente == NULL && elemento->sucessivo == NULL)
    {
        free(elemento);
        return NULL;
    }
    else
    {
        temp = elemento->sucessivo;
        elemento = elemento->precedente;
        free(elemento->sucessivo);
        elemento->sucessivo = temp;
        temp = elemento;
        elemento = elemento->sucessivo;
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

/// @brief Permette di annullare una prenotazione.
/// @param Lista Lista da cui annullare la prenotazione.
/// @param Matricola Matricola di cui annullare la prenotazione.
/// @return Inizio della lista d'attesa con le eliminazioni effettuate.
lista *elim_preontazione(lista *parte, int matricola)
{
    parte = rimozione_elementi(parte, ANNULAMENTO_PRENOTAZIONE, matricola, NULL);
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
    parte = rimozione_elementi(parte, FINE_TURNO, NULL, turno);
    if (parte == NULL)
    {
        return NULL;
    }
    return cima_lista(parte);
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
        int termine = 0;
        parte = cima_lista(parte);
        while (termine == 0)
        {
            if (parte->matricola == matricola)
            {
                cancella(parte);
                termine = 1;
            }
            if (parte->sucessivo == NULL && termine != 1)
            {
                termine = 2;
            }
            parte = parte->sucessivo;
        }
        switch (termine)
        {
        case 1:
            // Capire come gestire il caso di eliminazione riuscita.
            break;
        case 2:
            // Capire come gestire il caso in cui la eliminazione non è riuscita.
            break;
        default:
            // Toricamente questo caso non è possibile, poiche sono dei valori gestiti internamente.
            break;
        }
        break;
    case FINE_TURNO:
        parte = cima_lista(parte);
        do
        {
            if (parte->turno == turno)
            {
                parte = cancella(parte);
            }
            parte = parte->sucessivo;
        } while (parte->sucessivo == NULL);
        break;
    default:
        // Non gestito perché essendo una funzione gestita internamente, non può avere valori non previsti.
        break;
    }
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
    parte->matricola = NULL;
    parte->precedente = NULL;
    parte->sucessivo = NULL;
    parte->turno = NULL;
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
    while (parte->sucessivo != NULL)
    {
        parte = parte->sucessivo;
    }
    lista *tmp = parte;
    parte->sucessivo = (lista *)malloc(sizeof(lista));
    parte = parte->sucessivo;
    parte->precedente = tmp;
    parte->matricola = NULL;
    parte->turno = NULL;
    return parte;
}

/// @brief Permette di aggiungere una nuova prenotazione alla coda
/// @param Lista Lista in cui aggiungere il nuovo elemento
/// @param Nome Nome dello studente che sta effettuando la prenotazione
/// @param Turno Turno nel quale si sta prenotando lo studente
/// @param Matricola Matricola dello studente
/// @return Restituisce l'indirizzo del nuovo elemento salvato nella lista d'attesa
lista *nuova_aggiunta(lista *parte, char *nome, int turno, int matricola){
    parte=nuovo_elemento(parte);
    strcpy_s(parte->nome, MAX_NOME, nome);
    parte->turno=turno;
    parte->matricola=matricola;
    return parte;
}