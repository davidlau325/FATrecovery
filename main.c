#include <stdio.h>
#include <stdlib.h>
#include "entry.h"
#include "basic.h"
#include "recovery.h"

int main(int argc, char** argv) {
    char detect;
    detect = detectArgv(argc,argv);
    if(detect == 0){
        printf("Usage: ./recover -d [device filename] [other arguments]\n");
        printf("-i                    Print boot sector information\n");
        printf("-l                    List all the directory entries\n");
        printf("-r filename [-m md5]  File recovery with 8.3 filename\n");
        printf("-R filename           File recovery with long filename\n");
    }else{
        printf("Pass!\n");
    }

    return 0;
}

