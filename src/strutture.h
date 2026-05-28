
#define MAX_POSTI 100
#define MAX_NOME 16
#define MAX_MATRICOLA 12
#define MAX_CORSO_LAU 30



typedef enum{ Prenotato,Libero,Occupato } StatoPosto;

typedef enum{ Mattina,Pomeriggio,Sera } OrarioAula;

typedef struct
{
    int ora;
    int minuti;
    int secondi;

} OrarioVirtuale;


typedef struct Studente;


typedef struct  NodoStudente;


typedef struct  Posti;

typedef struct  Turno;



typedef struct  CodaAttesa;


