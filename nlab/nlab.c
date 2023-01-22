# include "specific.h"

void Prog(Program* p, Node* x)
{   
    //p->cw = 0;
    if (strsame(p->wds[p->cw], "BEGIN")) {
        p->cw = p->cw + 1;

        if (!strsame(p->wds[p->cw], "{")){
            ERROR("No LBRACE IN BEGIN? \n");
        }
        p->cw = p->cw + 1;
        Instrclist(p, x);
    }

    else if (!strsame(p->wds[p->cw], "BEGIN") && !strsame(p->wds[p->cw+1], "")){
        p->cw = p->cw + 1;
        Prog(p, x);
    }

    else if (!strsame(p->wds[p->cw], "BEGIN") && strsame(p->wds[p->cw+1], "")){
        ERROR("No BEGIN statement ? \n");
    }

}


void Instrclist(Program* p, Node* x)
{
    if (strsame(p->wds[p->cw], "}") && strsame(p->wds[p->cw+1], "")){ // strsame(p->wds[p->cw+1], "") means reaching the END
        return;
    }
    else if (!strsame(p->wds[p->cw], "}") && strsame(p->wds[p->cw+1], "")){
        ERROR("No } in the end ? \n");
    }
    Instrc(p, x);
    p->cw = p->cw + 1;
    Instrclist(p, x);

}



void Instrc(Program* p, Node* x)
{

    if (strsame(p->wds[p->cw], "PRINT")){
        p->cw = p->cw + 1;
        Print(p, x);
    }

    else if (strsame(p->wds[p->cw], "SET")){
        p->cw = p->cw + 1;
        Set(p, x);
    }

    else if (strsame(p->wds[p->cw], "ONES")){
        Create(p, x);
    }

    else if(strsame(p->wds[p->cw], "READ")){
        Create(p, x);
    }

    else if (strsame(p->wds[p->cw], "LOOP")){
        Loop(p, x);
    }


}

void Print(Program* p, Node* x)
{
    if (p->wds[p->cw][0] == '$'){ 
        /* Need to check the Varname's var_type */ 
        #ifdef INTERP
        char* temp = Varname(p);
        //printf("Print temp is %s \n", temp);
        if (search_varchain(x, temp).int_exist){
            
            printf("%d \n", search_varchain(x, temp).var_int);
        } 
        else if (search_varchain(x, temp).arr_exist){
            for (int j=0; j<(search_varchain(x, temp).arr_row); j++){
                for (int k=0; k<(search_varchain(x, temp).arr_col); k++){
                    printf("%d ", (search_varchain(x, temp).var_arr[j][k]));
                }
                printf("\n");
            }
        }
        else {
            //ERROR("Cannot find the VARNAME in PRINT \n");
        }
        #else
        if (x!=NULL){
            Varname(p);
        }
        #endif

        
    }

    else if (p->wds[p->cw][0] == '\"'){
        #ifdef INTERP
        char* temp;
        temp = String(p);
        printf("%s \n", temp);
        free(temp);
        #else
        char* temp;
        temp = String(p);
        free(temp);
        #endif
    }
    else {ERROR("Expecting a VARNAME or a STRING in PRINT \n")}

    return;
}


void Set(Program* p, Node* x)
{
    #ifdef INTERP
    char temp[MAXVARSIZE]= {'\0'};
    char* str = Varname(p);
    strcpy(temp, str);
    #else
    Varname(p);
    #endif

    p->cw = p->cw + 1;
    

    if (!strsame(p->wds[p->cw], ":=")){
        ERROR("Expecting a := in SET\n");
    }
    
    p->cw = p->cw + 1;
    #ifdef INTERP
    //printf("%s", p->wds[p->cw]);
    dict_Listelement* e = (dict_Listelement*)calloc(1, sizeof(dict_Listelement));
    Polishlist(p, x, e);
    
    
    dict_Listelement* temp1;
    temp1 = &(e[0]);
    //strcpy(temp1.type, Polishlist(p, x).type);
    if (strsame(temp1->type, "VI") || strsame(temp1->type, "IN")){
        insert_varintchain(x, temp, temp1->int_value);
    } 

    else if (strsame(temp1->type, "VA") || strsame(temp1->type, "IA")){
        int arr[MAXTOKENSIZE][MAXTOKENSIZE];
        int row = temp1->arr_row;
        int col = temp1->arr_col;
        for (int j=0; j<row; j++){
            for (int k=0; k<col; k++){
                arr[j][k] = temp1->arr_value[j][k];
                //printf("%d", arr[j][k]);
            }
        }
        insert_vararrchain(x, temp, row, col, arr);

    } else {
        ERROR("Expecting a Integer Variable/ Array Variable/ Constant Integer/ Constant Array behind := in SET \n")
    }
    
    free(e);
    #else
    Polishlistcheck(p, x);
    #endif

    //printf("%s", p->wds[p->cw]);

    return;

}

void Create(Program* p, Node* x)
{
    if(x!=NULL){
        if (strsame(p->wds[p->cw], "ONES")){
            //printf("yes");
            p->cw = p->cw + 1;
            #ifdef INTERP
            int row = atoi(Integer(p));
            #else
            Integer(p);
            #endif

            p->cw = p->cw + 1;
            #ifdef INTERP
            int col = atoi(Integer(p));
            #else
            Integer(p);
            #endif

            /* Create a int 2d-array full of ones */
            #ifdef INTERP
            int arr[MAXTOKENSIZE][MAXTOKENSIZE];
            //printf("The Ones array is %d \n");
            for (int j=0; j<row; j++){
                for (int k=0; k<col; k++){
                    arr[j][k] = One;
                    //printf("%d ", arr[j][k]);
                }
                //printf("\n");
            }
            #endif
            
            /* Assign the int 2d-array to the Varname */
            p->cw = p->cw + 1;
            #ifdef INTERP
            char temp[MAXVARSIZE]= {'\0'};
            char* str = Varname(p);
            strcpy(temp, str);
            insert_vararrchain(x, temp, row, col, arr);
            #else
            Varname(p);
            #endif

        }

       
            else if (strsame(p->wds[p->cw], "READ")){
                p->cw = p->cw + 1;
                //printf("%s", p->wds[p->cw]);
                #ifdef INTERP
                
                char* fname = String(p);
                FILE* fp = nfopen(fname, "rt");
                free(fname);
                int row, col;
                if (fscanf(fp, "%d %d", &row, &col)!=2){
                    ERROR ("Expecting enough elements in the .arr file");
                }
                //printf("%d %d \n", row, col);
                int i;
                int arr[MAXTOKENSIZE][MAXTOKENSIZE];
                
                for (int j=0; j<row; j++){
                    for (int k=0; k<col; k++){
                        if (fscanf(fp, "%d", &i)==1){
                            arr[j][k] = i;}
                        else {
                            ERROR ("Expecting enough elements in the .arr file");
                        }
                        //printf("%d ", arr[j][k]);
                    }
                    //printf("\n");
                    
                }

                /* Assign the int 2d-array to the Varname */
                p->cw = p->cw + 1;
                char temp[MAXVARSIZE]= {'\0'};
                char* str = Varname(p);
                strcpy(temp, str);
                insert_vararrchain(x, temp, row, col, arr);
                
                
                #else
                
                char* fname = String(p);
                free(fname);
                p->cw = p->cw + 1;
                Varname(p);
               
                #endif
            }
        

    }
    


    return;
}


void Loop(Program* p, Node* x)
{
    p->cw = p->cw + 1;

    #ifdef INTERP
    char temp[MAXVARSIZE]= {'\0'};
    char* str = Varname(p);
    strcpy(temp, str);
    #else
    Varname(p);
    #endif

    p->cw = p->cw + 1;

    #ifdef INTERP
    int loop_count = atoi(Integer(p));
    #else
    Integer(p);
    #endif

        
    p->cw = p->cw + 1;    
    if (!strsame(p->wds[p->cw], "{")){
        ERROR("No LBRACE IN LOOP? \n");
    }

 

    #ifdef INTERP
    int count=0;
    int cw_temp = p->cw;
    while (p->wds[cw_temp][0] != '}'){
        cw_temp++;
        count++;
    }
  
    //printf("%s", p->wds[p->cw+1]);
    
    
    int loop_num = One;
    insert_varintchain(x, temp, loop_num);

    while (search_varchain(x, temp).var_int<loop_count+1){
        //printf("%d", i);        
        while (p->wds[p->cw][0] != '}'){
            p->cw = p->cw + 1;
            //printf("%s", p->wds[p->cw]);
            Instrc(p, x);
        }
        p->cw = p->cw-count;
        
        //printf("%s's loop num is %d \n", temp, search_varchain(x,temp).var_int);
        
        insert_varintchain(x, temp, search_varchain(x,temp).var_int+1);
        
    }
    
  
    p->cw = p->cw+count;
    #else
    while (p->wds[p->cw][0] != '}'){
        p->cw = p->cw + 1;
            //printf("%s", p->wds[p->cw]);
        Instrc(p, x);
    }
    #endif
    //printf("%s", p->wds[p->cw]);

    return;

}


#ifdef INTERP
void Polishlist(Program* p, Node* x, dict_Listelement* e)
{
    stack list;
    dict_Listelement e1, e2;
    CreateStack(&list);


    while (!strsame(p->wds[p->cw], ";")){
        if (isBINARYOP(p)){
            //printf("yes");
            pop(&list, &e1);
            pop(&list, &e2);
            
            if (strsame(p->wds[p->cw], "B-ADD")){

                if ((strsame(e1.type, "VA") || strsame(e1.type, "IA")) && (strsame(e2.type, "VA") || strsame(e2.type, "IA"))){
                    if (e2.arr_row !=e1.arr_row || e2.arr_col !=e1.arr_col){
                        ERROR("Expecting 2 same sized arrays for B-OR");
                    } else {
                        strcpy(e[0].type, "IA");
                        e[0].arr_row =  e2.arr_row;
                        e[0].arr_col =  e2.arr_col;
                        for (int j=0; j<e[0].arr_row; j++){
                            for (int k=0; k<e[0].arr_col; k++){
                                    e[0].arr_value[j][k] = e2.arr_value[j][k] + e1.arr_value[j][k];
                                    //printf("%d", e.arr_value[j][k]);
                            }
                        }
                    }
                }
                
                else if ((strsame(e1.type, "VI") || strsame(e1.type, "IN")) && (strsame(e2.type, "VA") || strsame(e2.type, "IA"))){
                    strcpy(e[0].type, "IA");
                    int a = e1.int_value;
                    e[0].arr_row =  e2.arr_row;
                    e[0].arr_col =  e2.arr_col;
                    for (int j=0; j<e[0].arr_row; j++){
                        for (int k=0; k<e[0].arr_col; k++){
                                e[0].arr_value[j][k] = e2.arr_value[j][k] + a;
                                //printf("%d", e.arr_value[j][k]);
                        }
                    }
                }
    
                else if ((strsame(e1.type, "VI") || strsame(e1.type, "IN")) && (strsame(e2.type, "VI") || strsame(e2.type, "IN"))){
                    strcpy(e[0].type, "IN");
                    int a = e1.int_value;
                    int b = e2.int_value;
                    e[0].int_value = a+b;
                }
                push(&list, e[0]);
            }

            else if (strsame(p->wds[p->cw], "B-TIMES")){

                if ((strsame(e1.type, "VA") || strsame(e1.type, "IA")) && (strsame(e2.type, "VA") || strsame(e2.type, "IA"))){
                    if (e2.arr_row !=e1.arr_row || e2.arr_col !=e1.arr_col){
                        ERROR("Expecting 2 same sized arrays for B-OR");
                    } else {
                        strcpy(e[0].type, "IA");
                        e[0].arr_row =  e1.arr_row;
                        e[0].arr_col =  e1.arr_col;
                        
                        //printf("%d", e[0].arr_row);
                        for (int j=0; j<e[0].arr_row; j++){
                            for (int k=0; k<e[0].arr_col; k++){   
                                    e[0].arr_value[j][k]=e1.arr_value[j][k] * e2.arr_value[j][k]; 
                                //printf("%d", e[0].arr_value[j][k]);
                            }
                            //printf("\n");
                        }
                    }
                }
                else if ((strsame(e1.type, "VI") || strsame(e1.type, "IN")) && (strsame(e2.type, "VA") || strsame(e2.type, "IA"))){
                    strcpy(e[0].type, "IA");
                    //printf("%s", e2.word);
                    e[0].arr_row =  e2.arr_row;
                    e[0].arr_col =  e2.arr_col;
                    for (int j=0; j<e[0].arr_row; j++){
                        for (int k=0; k<e[0].arr_col; k++){ 
                            e[0].arr_value[j][k]=e1.int_value * e2.arr_value[j][k]; 
                        }
                    }

                }
             
                else if ((strsame(e1.type, "VI") || strsame(e1.type, "IN")) && (strsame(e2.type, "VI") || strsame(e2.type, "IN"))){
                    strcpy(e[0].type, "IN");
                    int a = e1.int_value * e2.int_value;
                    e[0].int_value = a;    
                }
                push(&list, e[0]);
            }

            else if (strsame(p->wds[p->cw], "B-EQUALS")){
                if ((strsame(e1.type, "VA") || strsame(e1.type, "IA")) && (strsame(e2.type, "VA") || strsame(e2.type, "IA"))){
                    if (e2.arr_row !=e1.arr_row || e2.arr_col !=e1.arr_col){
                        ERROR("Expecting 2 same sized arrays for B-OR");
                    } else {
                        strcpy(e[0].type, "IA");
                        e[0].arr_row =  e1.arr_row;
                        e[0].arr_col =  e1.arr_col;
                        
                        //printf("%d", e[0].arr_row);
                        for (int j=0; j<e[0].arr_row; j++){
                            for (int k=0; k<e[0].arr_col; k++){ 
                                if (e2.arr_value[j][k] == e1.arr_value[j][k]){
                                    e[0].arr_value[j][k]=1;
                                } else {e[0].arr_value[j][k]=0;}
                                //printf("%d", e[0].arr_value[j][k]);
                            }
                            //printf("\n");
                        }
                    }

                }
                else if ((strsame(e1.type, "VI") || strsame(e1.type, "IN")) && (strsame(e2.type, "VA") || strsame(e2.type, "IA"))){
                    strcpy(e[0].type, "IA");
                    //printf("%s", e2.word);
                    e[0].arr_row =  e2.arr_row;
                    e[0].arr_col =  e2.arr_col;
                    for (int j=0; j<e[0].arr_row; j++){
                        for (int k=0; k<e[0].arr_col; k++){ 
                            if (e1.int_value==e2.arr_value[j][k]){
                                e[0].arr_value[j][k]=1;
                            } else {e[0].arr_value[j][k]=0;}
                        }
                    }

                }
                else if ((strsame(e1.type, "VI") || strsame(e1.type, "IN")) && (strsame(e2.type, "VI") || strsame(e2.type, "IN"))){
                    strcpy(e[0].type, "IN");
                    if (e1.int_value == e2.int_value){
                        e[0].int_value=1;
                    } else {e[0].int_value=0;}
                }

                push(&list, e[0]);

            }

            else if (strsame(p->wds[p->cw], "B-AND")){
                if ((strsame(e1.type, "VA") || strsame(e1.type, "IA")) && (strsame(e2.type, "VA") || strsame(e2.type, "IA"))){
                    if (e2.arr_row !=e1.arr_row || e2.arr_col !=e1.arr_col){
                        ERROR("Expecting 2 same sized arrays for B-OR");
                    } else {
                        strcpy(e[0].type, "IA");
                        e[0].arr_row =  e1.arr_row;
                        e[0].arr_col =  e1.arr_col;
                        
                        //printf("%d", e[0].arr_row);
                        for (int j=0; j<e[0].arr_row; j++){
                            for (int k=0; k<e[0].arr_col; k++){ 
                                if (e2.arr_value[j][k]>=1 && e1.arr_value[j][k]>=1){
                                    e[0].arr_value[j][k]=1;
                                } else {e[0].arr_value[j][k]=0;}
                                //printf("%d", e[0].arr_value[j][k]);
                            }
                            //printf("\n");
                        }
                    }

                }
                else if ((strsame(e1.type, "VI") || strsame(e1.type, "IN")) && (strsame(e2.type, "VA") || strsame(e2.type, "IA"))){
                    strcpy(e[0].type, "IA");
                    e[0].arr_row =  e2.arr_row;
                    e[0].arr_col =  e2.arr_col;

                    for (int j=0; j<e[0].arr_row; j++){
                        for (int k=0; k<e[0].arr_col; k++){ 
                            if (e1.int_value>=1 && e2.arr_value[j][k]>=1){
                                    e[0].arr_value[j][k]=1;
                                } else {e[0].arr_value[j][k]=0;}
                                //printf("%d", e[0].arr_value[j][k]);
                            }
                            //printf("\n");
                    }

                }
                else if ((strsame(e1.type, "VI") || strsame(e1.type, "IN")) && (strsame(e2.type, "VI") || strsame(e2.type, "IN"))){
                    strcpy(e[0].type, "IN");
                    if (e1.int_value>=1 && e2.int_value>=1){
                        e[0].int_value=1;
                    } else {e[0].int_value=0;}
                }
                push(&list, e[0]);
            }

            else if (strsame(p->wds[p->cw], "B-OR")){
                if ((strsame(e1.type, "VA") || strsame(e1.type, "IA")) && (strsame(e2.type, "VA") || strsame(e2.type, "IA"))){
                    if (e2.arr_row !=e1.arr_row || e2.arr_col !=e1.arr_col){
                        ERROR("Expecting 2 same sized arrays for B-OR");
                    } else {
                        strcpy(e[0].type, "IA");
                        e[0].arr_row =  e1.arr_row;
                        e[0].arr_col =  e1.arr_col;
                        
                        //printf("%d", e[0].arr_row);
                        for (int j=0; j<e[0].arr_row; j++){
                            for (int k=0; k<e[0].arr_col; k++){ 
                                if (e2.arr_value[j][k]>=1 || e1.arr_value[j][k]>=1){
                                    e[0].arr_value[j][k]=1;
                                } else {e[0].arr_value[j][k]=0;}
                                //printf("%d", e[0].arr_value[j][k]);
                            }
                            //printf("\n");
                        }
                    }

                }
                else if ((strsame(e1.type, "VI") || strsame(e1.type, "IN")) && (strsame(e2.type, "VA") || strsame(e2.type, "IA"))){
                    strcpy(e[0].type, "IA");
                    e[0].arr_row =  e2.arr_row;
                    e[0].arr_col =  e2.arr_col;

                    for (int j=0; j<e[0].arr_row; j++){
                        for (int k=0; k<e[0].arr_col; k++){ 
                            if (e1.int_value>=1 || e2.arr_value[j][k]>=1){
                                    e[0].arr_value[j][k]=1;
                                } else {e[0].arr_value[j][k]=0;}
                                //printf("%d", e[0].arr_value[j][k]);
                            }
                            //printf("\n");
                    }


                }
                else if((strsame(e1.type, "VI") || strsame(e1.type, "IN")) && (strsame(e2.type, "VI") || strsame(e2.type, "IN"))){
                    strcpy(e[0].type, "IN");
                    if (e1.int_value>=1 || e2.int_value>=1){
                        e[0].int_value=1;
                    } else {e[0].int_value=0;}

                } 
                push(&list, e[0]);
                
            }


        } 
        else if(isUNARYOP(p)){
            pop(&list, &e1);
            if (strsame(p->wds[p->cw], "U-NOT")){
                if (strsame(e1.type, "VA") || strsame(e1.type, "IA")){
                    strcpy(e[0].type, "IA");
                    e[0].arr_row =  e1.arr_row;
                    e[0].arr_col =  e1.arr_col;
                     for (int j=0; j<e[0].arr_row; j++){
                        for (int k=0; k<e[0].arr_col; k++){ 
                            if (e1.arr_value[j][k]==0){
                                e[0].arr_value[j][k] = 1;
                            } else {
                                e[0].arr_value[j][k] = 0;
                            }
                            //e[0].arr_value[j][k] = !e1.arr_value[j][k];
                        }
                     }
                } 
                else if (strsame(e1.type, "VI") || strsame(e1.type, "IN")){
                    strcpy(e[0].type, "IN");
                    if (e1.int_value==0){
                        e[0].int_value = 1; 
                    } else {
                        e[0].int_value = 0; 
                    }
                    
                    
                }
                push(&list, e[0]);

            } 

            else if(strsame(p->wds[p->cw], "U-EIGHTCOUNT")){
                if (strsame(e1.type, "VA") || strsame(e1.type, "IA")){
                    strcpy(e[0].type, "IA");
                    e[0].arr_row =  e1.arr_row;
                    e[0].arr_col =  e1.arr_col;
                    
                    //int flat_arr[e[0].arr_row*e[0].arr_col];
                    for (int j=0; j<e[0].arr_row; j++){
                        for (int k=0; k<e[0].arr_col; k++){   
                            int count=0;           
                            if((j>=1 && k>=1 && e1.arr_value[j-1][k-1]==1)){ //North-west
                                count++;
                            }
                            if (j>=1 && e1.arr_value[j-1][k]==1){ //North
                                count++;
                            }
                            if (j>=1 && k<=e[0].arr_col-2 && e1.arr_value[j-1][k+1]==1){ //North-east
                                count++;
                            }
                            if (k>=1 && e1.arr_value[j][k-1]==1){ //West
                                count++;
                            }
                            if (k<=e[0].arr_col-2 && e1.arr_value[j][k+1]==1){ //East
                                count++;
                            }
                            if (j<=e[0].arr_row-2 && k>=1 && e1.arr_value[j+1][k-1]==1){ //South-west
                                count++;
                            }
                            if (j<=e[0].arr_row-2 && e1.arr_value[j+1][k]==1){ //South
                                count++;
                            }
                            if (j<=e[0].arr_row-2 && k<=e[0].arr_col-2 && e1.arr_value[j+1][k+1]==1){ //South-East
                                count++;
                            }
                            e[0].arr_value[j][k] = count;
                            //printf("j=%d, k=%d, count=%d \n", j, k, count);
                        }
                    }
                } 
                else if (strsame(e1.type, "VI") || strsame(e1.type, "IN")){
                    strcpy(e[0].type, "IN");
                    e[0].int_value = 0;
                }
                push(&list, e[0]);
            }


        }
        
        else {
            //printf("%s", p->wds[p->cw]);

            if (p->wds[p->cw][0]=='$'){
                strcpy(e[0].word, Pushdown(p));
                if (search_varchain(x, e[0].word).int_exist){
                    strcpy(e[0].type, "VI");
                    int a = search_varchain(x, e[0].word).var_int;
                    e[0].int_value = a;
                } 
                else if (search_varchain(x, e[0].word).arr_exist){
                    strcpy(e[0].type, "VA");
                    e[0].arr_row =  search_varchain(x, e[0].word).arr_row;
                    e[0].arr_col =  search_varchain(x, e[0].word).arr_col;
                     for (int j=0; j<e[0].arr_row; j++){
                        for (int k=0; k<e[0].arr_col; k++){ 
                            e[0].arr_value[j][k] = search_varchain(x, e[0].word).var_arr[j][k];
                        }
                     }
                }
                push(&list, e[0]);
            }
            else {
                int f = atoi(Pushdown(p));
                strcpy(e[0].type, "IN"); 
                e[0].int_value = f;
                push(&list, e[0]);
            } 
        }
        p->cw = p->cw+1;
        //printf("%s", p->wds[p->cw]);
    }


    ClearStack(&list);
    FreeStack(&list);

    return;
}


#else
void Polishlistcheck(Program* p, Node* x){
    if (x!=NULL){
        while (!strsame(p->wds[p->cw], ";")){
            if (p->wds[p->cw][0] == '$'){
                Varname(p);
            } 
            else if (isUNARYOP(p) || isBINARYOP(p)) {

            }
            else {
                Integer(p);
            }
            p->cw = p->cw+1;
        }
    }
    return;
    
}
#endif


bool isUNARYOP(Program* p){
    if (strsame(p->wds[p->cw], "U-NOT") || strsame(p->wds[p->cw], "U-EIGHTCOUNT")){
        return true;
    }
    return false;

}


bool isBINARYOP(Program* p){
    if (strsame(p->wds[p->cw], "B-ADD") || strsame(p->wds[p->cw], "B-TIMES") || \
    strsame(p->wds[p->cw], "B-EQUALS") ||  strsame(p->wds[p->cw], "B-OR") || strsame(p->wds[p->cw], "B-AND")){
        return true;
    }
    return false;
}

char* Pushdown(Program* p)
{
    char* temp;
    if (p->wds[p->cw][0] == '$'){
        temp = Varname(p);
        //p->cw = p->cw + 1;
    } else{
        temp = Integer(p);
    }
    return temp;
}


char* Varname(Program* p)
{  
    int i, len;

    if (p->wds[p->cw][0] != '$'){
        ERROR("Expecting a $ in VARNAME? \n");
    }

    len = (int)strlen(p->wds[p->cw]);
    if (len < 2) { // check whether VARNAME's len is over 2. The correct example: $A, $B
        ERROR("Illegal VARNAME \n");
    }
    for (i=1; i<len; i++){
        if (!isupper(p->wds[p->cw][i])){
            ERROR("Illegal VARNAME. VARNAME should be made of upper-case characters. \n");
        };
    }
    //char temp[MAXVARSIZE] = {'\0'};

    //printf("The valname is %s", strcpy(temp, p->wds[p->cw]+1));
    

    return p->wds[p->cw]+1;
}

char* String(Program* p)
{
    int i, j, len;

    if (p->wds[p->cw][0] != '\"'){
        ERROR("Expecting a \" in STRING \n");
    }

    len = (int)strlen(p->wds[p->cw]);
    if (p->wds[p->cw][1] == '\"'){ // check whether the string contains characters
        ERROR("Illegal STRING. STRING should contain at least one character \n");
    } 
   
    char* tempstr = (char* )calloc(len-1, sizeof(char));   
    j = 0;
    for (i=1; i<len-1; i++){ // i's range[1,len-1): elimating the " " in the string
        tempstr[j] = p->wds[p->cw][i];
        //printf("%c", p->wds[p->cw][i]);
        j++;
    }
    //printf("TEMP 1 is %s \n", tempstr);

    return tempstr;
}



char* Integer(Program* p)
{
    int i, len;
    char ch;
    
    len = (int)strlen(p->wds[p->cw]);
    for (i=0; i<len; i++){
        ch = p->wds[p->cw][i];
        if ((ch<'0' || ch> '9')){
            printf("%s \n", p->wds[p->cw]);
            ERROR("Illegal INTEGER as above \n");
        }
    }

    return p->wds[p->cw];

}








