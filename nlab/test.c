# include "specific.h"


int main(int argc, char* argv[])
{   
    if (argc != 2){
        fprintf(stderr, "Usage : %s <xxx.nlb> \n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    FILE* fp = nfopen(argv[1], "rt");
    

    Node* varchain = init_varchain();

    Program* prog = calloc(1, sizeof(Program));
    
    
    int t=0;
    while(fscanf(fp, "%s", prog->wds[t++])==1 && t<MAXNUMTOKENS);
    assert(t<MAXNUMTOKENS);
    prog->len = t;

    
    Prog(prog,varchain);
    #ifdef INTERP // Usage:-DINTERP
    //printf("Interpreted OK\n");
    #else
    printf("Parsed OK\n"); // Although Interperter will not print "Parsed OK", it will also run the parser.
    #endif
    //print_varchain(varchain);

    free(prog);
    free(varchain);
    
    return 0;
}

