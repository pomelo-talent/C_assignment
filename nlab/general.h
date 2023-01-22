#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

void on_error(const char* s, char* fname);
void* nfopen(char* fname, char* mode);

