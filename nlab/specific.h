#include "general.h"
#include "stack.h"

#define MAXNUMTOKENS 100
#define MAXTOKENSIZE 20
#define strsame(A,B) (strcmp(A, B)==0)
#define ERROR(PHRASE) { fprintf(stderr, \
          "Fatal Error %s occurred in %s, line %d\n", PHRASE, \
          __FILE__, __LINE__); \
          exit(EXIT_FAILURE); }

#define MAXNUMVARS 100
#define MAXVARSIZE MAXTOKENSIZE-1
#define One 1

          

struct prog{
   char wds[MAXNUMTOKENS][MAXTOKENSIZE];
   int cw; // Current word
   int len; // Full length of prog
};
typedef struct prog Program;


struct dict{
    int index; // must be unique
    char var_word[MAXVARSIZE]; // must be unique
    bool int_exist; // this dict has two types: the 1st one- Varname(store int); 
    int var_int;
    bool arr_exist; //  the 2nd one- Varname(store array);
    int var_arr[MAXTOKENSIZE][MAXTOKENSIZE];
    int arr_row;
    int arr_col;
};
typedef struct dict dict;

struct Node{
    struct dict data;
    struct Node* next;
    int node_size;
};
typedef struct Node Node;


void Prog(Program* p, Node* x);
void Instrclist(Program* p, Node* x);
void Instrc(Program* p, Node* x);
void Print(Program* p, Node* x);
void Set(Program* p, Node* x);
void Create(Program* p, Node* x);
void Loop(Program* p, Node* x);

#ifdef INTERP
void Polishlist(Program* p, Node* x, dict_Listelement* e);
#else
void Polishlistcheck(Program* p, Node* x);
#endif
bool isUNARYOP(Program* p);
bool isBINARYOP(Program* p);
char* Pushdown(Program* p);
char* Varname(Program* p);
char* String(Program* p);
char* Integer(Program* p);


Node* init_varchain();
bool insert_varintchain(Node* x, const char* s, int value);
bool insert_vararrchain(Node* x, const char* s, int row, int col, int arr[MAXTOKENSIZE][MAXTOKENSIZE]);
dict search_varchain(Node* x, const char* s);
void print_varchain(Node* x);


