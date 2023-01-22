# include "general.h"
# include "stack.h"

void CreateStack(stack* s)

{
    // Assign space for stack
    s->base = (dict_Listelement*)malloc(MAX_LEN*sizeof(dict_Listelement));
    if(!s->base)// Check whether CreateStack succeeds
    {
        printf("ERROR in CreateStack! \n");
        exit(0);
    }

    s->top = s->base;
    s->len = 0;
    s->stacksize = MAX_LEN;

}

void push(stack* s, dict_Listelement e)
{
    if(s->top-s->base >= MAX_LEN) // The space cannot hold more elements-> realloc
    {
        s->base = (dict_Listelement *)realloc(s->base,(s->stacksize+MORE_LEN)*sizeof(dict_Listelement));
            if(!s->base) {
                printf("ERROR in push! \n");
                exit(0);
            }
        s->stacksize += MORE_LEN;
    }
    *s->top = e;
    s->top++;

}

void pop(stack* s, dict_Listelement* e)
{
    if(s->base == s->top){
        printf("ERROR in pop! \n");
    } else{
        s->top --;
        *e = *s->top;
        s->len--;
    }

}



void FreeStack(stack *s) // Free the Stack
{
    free(s->base);
}


void ClearStack(stack *s) // Clear the Stack
{
    s->top = s->base;
    s->len = 0;
    s->stacksize = MAX_LEN;

}


