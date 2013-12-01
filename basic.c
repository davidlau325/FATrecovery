#include <stdio.h>
#include <stdlib.h>
#include "basic.h"

char* devicename = NULL;
char* filename = NULL;
char* md5 = NULL;

char detectArgv(int argc,char** argv){
    if(argc < 4){
        return 0;
    }
    return 1;
}
