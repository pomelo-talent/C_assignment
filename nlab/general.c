#include "general.h"

void on_error(const char* s, char* fname)
{
   fprintf(stderr, "%s %s\n", s, fname);
   exit(EXIT_FAILURE);
}


void* nfopen(char* fname, char* mode)
{
   FILE* fp;
   fp = fopen(fname, mode);
   if(!fp){
      on_error("Cannot open file: ", fname);
   }
   return fp;
}

