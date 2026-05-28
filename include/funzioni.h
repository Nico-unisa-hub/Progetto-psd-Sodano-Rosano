enum StatoPosto;
enum OrarioAula;
enum StatoStudente;
typedef struct OrarioVirtuale;
typedef struct Studente;
typedef struct NodoStudente;
typedef struct Posti;
typedef struct Turno;
typedef struct CodaAttesa;


void Inserisci_Studente(NodoStudente *, Studente );
Studente crea_studente(char* , char *, char* );
void Registra_Studente(NodoStudente* ,char* ,char* , char* );
void Effettua_Prenotazione(char *, char *, OrarioAula *, Turno *,NodoStudente* , CodaAttesa *);
void  aggiungi_in_coda(CodaAttesa* , NodoStudente* , OrarioAula ,char* );
void check_in_studenti( Turno *, NodoStudente *,CodaAttesa *,char *,OrarioVirtuale );
void annulla_prenotazione(char *, OrarioAula *, Turno *, CodaAttesa *, NodoStudente *, OrarioVirtuale );
void visualizza_studenti_per_stato(NodoStudente* , Turno* , CodaAttesa* );
void visualizza_situazione_corrente(Turno* , CodaAttesa* );
void check_out_studenti(NodoStudente *,CodaAttesa *,Turno *, OrarioVirtuale ,char *);
void report(Turno *, CodaAttesa *);
void leggi_storico();
void Scrivi_storico(NodoStudente *,char*,char* ,OrarioVirtuale );
void cambio_fascia_automatica(Turno *,NodoStudente *, CodaAttesa *, OrarioAula );
long orario_in_secondi(OrarioVirtuale );
int is_orario_valido(OrarioVirtuale , OrarioAula );
void aggiorna_orario_automatico(OrarioVirtuale *, time_t *, Turno *, CodaAttesa *, NodoStudente *);
void inizializza_sistema(NodoStudente *,Turno* , CodaAttesa* ); 
void inizializza_sistema_dinamico(NodoStudente** , Turno** , CodaAttesa** );



