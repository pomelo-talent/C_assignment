# include "general.h"
# include "specific.h"

Node* init_varchain()
{
    int size;
    size = MAXNUMVARS;

    Node* varchain = (struct Node*)calloc(size, sizeof(struct Node));
    varchain->node_size = size;
    
    /*initialize the value of char in the Node* chain */
    int k;
    for (k=0; k<varchain->node_size; k++){
       varchain[k].data.var_word[0]='\0';
       //varchain[k].data.var_type[0]='\0';
       varchain[k].data.int_exist = false;
       varchain[k].data.arr_exist = false;
    }
   

    return varchain;
}

dict search_varchain(Node* x, const char* s)
{
    dict temp;
    temp.int_exist = false;
    temp.arr_exist = false;
    temp.var_int = 0;
    if (x==NULL || s==NULL){
        //temp.int_exist = false;
        return temp;
    }
    
    int i;
    for (i=0; i<x->node_size; i++){
        if (strcmp(x[i].data.var_word, s)==0){
            if (x[i].data.int_exist){
                temp.int_exist = true;
                temp.var_int = x[i].data.var_int;
                temp.index = x[i].data.index;
                return temp;
            } 
            else if (x[i].data.arr_exist){
                temp.arr_exist = true;
                for (int j=0; j<x[i].data.arr_row; j++){
                    for (int k=0; k<x[i].data.arr_col; k++){
                        temp.var_arr[j][k] = x[i].data.var_arr[j][k];

                    }
                }
                temp.arr_row = x[i].data.arr_row;
                temp.arr_col = x[i].data.arr_col;
                temp.index = x[i].data.index;
                return temp;
            }
        }
    }
    return temp;
}



/* dict for Integer Variables */
bool insert_varintchain(Node* x, const char* s, int value)
{
    if (x==NULL || s==NULL){
        return false;
    }

    else if (search_varchain(x, s).int_exist){
        x[search_varchain(x, s).index].data.var_int = value;
        x[search_varchain(x, s).index].data.int_exist = true;
        //printf("%s 'sindex %d:  \n", s, search_varchain(x, s).index);
        return true;
    } else {
        int i;
        for (i=0; i<x->node_size; i++){
            if (x[i].data.var_word[0]=='\0'){
                strcpy(x[i].data.var_word, s);
                x[i].data.var_int = value;
                x[i].data.int_exist = true;
                //strcpy(x[i].data.var_type, "int");
                x[i].data.index = i;
                //printf("%s's i is %d \n", s, i);
                x[i].next = NULL;
                return true;
            }
        }
    }
    return true;

}


/*dict for 2D-Arrary Variables*/
bool insert_vararrchain(Node* x, const char* s, int row, int col, int arr[MAXTOKENSIZE][MAXTOKENSIZE])
{
    if (x==NULL || s==NULL){
        return false;
    }

    else if (search_varchain(x, s).arr_exist){ // alter existing elements
        for (int j =0; j<search_varchain(x, s).arr_row; j++){
            for (int k=0; k<search_varchain(x, s).arr_col; k++){
                x[search_varchain(x, s).index].data.var_arr[j][k] = arr[j][k];
            }
        }
        return true;
        

    } else { //insert new elements
        int i;
        for (i=0; i<x->node_size; i++){
            if (x[i].data.var_word[0]=='\0'){
                strcpy(x[i].data.var_word, s);
                //printf("%s", x[i].data.var_word);
                //printf("The copied array: \n");
                for (int j=0; j<row; j++){
                    for (int k=0; k<col; k++){
                        x[i].data.var_arr[j][k] = arr[j][k];
                        //printf("%d ", arr[j][k]);
                    }
                    //printf("\n");
                }
                x[i].data.arr_exist = true;
                //strcpy(x[i].data.var_type, "arr");
                x[i].data.arr_row = row;
                //printf("the row is %d", row);
                x[i].data.arr_col = col;
                x[i].data.index = i;
                x[i].next = NULL;
                return true;
                
            }
        }
    }
    return true;

}




/* Print & Check the Node */
void print_varchain(Node* x)
{
    int i;
    for (i=0; i<x->node_size; i++){
        if (x[i].data.var_word[0]!='\0'){
            printf("The varname is %s \n", x[i].data.var_word);
            if (x[i].data.int_exist){
                printf("The value is %d \n", x[i].data.var_int);}
            else if (x[i].data.arr_exist){
                printf("%s", "The array is \n");
                for (int j=0; j<x[i].data.arr_row; j++){
                    for (int k=0; k<x[i].data.arr_col; k++){
                        printf("%d ", x[i].data.var_arr[j][k]);
                    }
                    printf("\n");
                }

            } 
            //assert(x[i].data.int_exist);
        }
    }
    
    return;
}
