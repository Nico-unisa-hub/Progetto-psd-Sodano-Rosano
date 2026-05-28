#include "strutture.h"
#include <time.h>>


Studente *cerca_Studente(NodoStudente *, char *);
void Inserisci_Studente(NodoStudente *nodo, Studente s);
Studente crea_studente(char* , char *, char* );
void Registra_Studente(NodoStudente* ,char* ,char* , char* );
void effettua_prenotazione(NodoStudente* , Turno* , CodaAttesa* , char* , OrarioAula , OrarioVirtuale );
void  aggiungi_in_coda(CodaAttesa* , NodoStudente* , OrarioAula ,char* );
void check_in_studenti( Turno *, NodoStudente *,CodaAttesa *,char *,OrarioVirtuale );
void annulla_prenotazione(char *, OrarioAula *, Turno *, CodaAttesa *, NodoStudente *, OrarioVirtuale );
void visualizza_studenti_per_stato(NodoStudente* , Turno*, CodaAttesa* );
void visualizza_situazione_corrente(Turno* , CodaAttesa* );
void check_out_studenti(NodoStudente *,CodaAttesa *,Turno *, OrarioVirtuale ,char *);
void report(Turno *, CodaAttesa *);
void leggi_storico();
void Scrivi_storico(NodoStudente *,char*,char* ,OrarioVirtuale );
void cambio_fascia_automatica(Turno *,NodoStudente *, CodaAttesa *, OrarioAula );
long orario_in_secondi(OrarioVirtuale );
int is_orario_valido(OrarioVirtuale , OrarioAula );
void aggiorna_orario_automatico(OrarioVirtuale *, time_t *, Turno *, CodaAttesa *, NodoStudente *);
void inizializza_strutture(NodoStudente *,Turno* , CodaAttesa* ); 
void alloca_memoria_strutture(NodoStudente** , Turno** , CodaAttesa** );
int get_dimensione_coda(CodaAttesa* );
char* get_data_aula(Turno* );
int get_posti_occupati_totali(Turno* );
OrarioAula get_fascia_aula(Turno* );


