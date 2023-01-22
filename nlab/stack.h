
#define MAX_LEN 100
#define MORE_LEN 100


typedef struct dict_Listelement{

    char type[3]; 
    // 4 types: VI-Varname(store int); VA-Varname(store array); IN-Constant Integer; IA-Const Array;
    char word[MAX_LEN]; // for VI & VA e.g. $A's word: A
    int int_value; // for storing VI/IN's value;
    int arr_value[MAX_LEN][MAX_LEN]; // for storing VA/IA's value;
    int arr_row; // for storing VA/IA's row;
    int arr_col; // for storing VA/IA's row;

} dict_Listelement;


typedef struct Stack{

    int len; //current size
    dict_Listelement* top; // pointer to the top
    dict_Listelement* base; // pointer to the base
    int stacksize;

} stack;




void CreateStack(stack* s);
void push(stack *s, dict_Listelement e);
void pop(stack *s, dict_Listelement* e);
void FreeStack(stack* s);
void ClearStack(stack *s);

