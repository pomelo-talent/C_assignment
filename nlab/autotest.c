/* An auto_checker:
Usage of autotest:
     ./autotest <xxx.rst> <xxx.rst> 
      (the first xxx.rst file is the interpreted output of its homonymous .nlb file which is generated by "make savetestinterp")
      (the second xxx.rst file is our expected output of the xxx.nlb and we don't know whether the interpreted output of xxx.nlb 
      will match it now) 
      e.g. ./autotest ./expectedrst/setprinta.rst setprinta.rst
*/
# include "specific.h"
#define MAXLEN 10000


int main(int argc, char* argv[]){
    if(argc != 3){
      fprintf(stderr, "Usage : %s <xxx.rst> <xxx.rst> \n \
      (the first xxx.rst file is the output of one xxx.nlb with the same name )\n \
      (the second xxx.rst file is the expected answer and we don't know whether the output of xxx.nlb match it now) \n", argv[0]);
      exit(EXIT_FAILURE);
   }

    FILE* fp1 = nfopen(argv[1], "rt");
    Program* prog1 = calloc(1, sizeof(Program));
    int t=0;
    while(fscanf(fp1, "%s", prog1->wds[t++])==1 && t<MAXLEN);
    assert(t<MAXLEN);
    

    FILE* fp2 = nfopen(argv[2], "rt");
    Program* prog2 = calloc(1, sizeof(Program));
    int k=0;
    while(fscanf(fp2, "%s", prog2->wds[k++])==1 && k<MAXLEN);
    assert(k<MAXLEN);
     
    prog1->cw = 0;
    prog2->cw = 0;
    while (!strsame(prog1->wds[prog1->cw+1], "")){
      if (!strsame(prog1->wds[prog1->cw], prog2->wds[prog2->cw])){
        printf("The first file displays in the %dth token: %s \n", prog1->cw, prog1->wds[prog1->cw]);
        printf("The second file displays in the %dth token: %s \n", prog1->cw, prog2->wds[prog2->cw]);
        //ERROR("The input file may have some mistakes or the interp may have some mistakes.");
      }
      prog1->cw = prog1->cw + 1;
      prog2->cw = prog2->cw + 1;
    }

    free(prog1);
    free(prog2);


    return 0;
}

