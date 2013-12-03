#include <stdio.h>
#include <stdlib.h>
#include "entry.h"
#include "basic.h"
#include "recovery.h"

/*
available inputs:

extern char* devicename;
extern char* filename;
extern char* md5;
*/

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
        switch(detect){
            case 'i':
                printf("reached i\n");
                printf("Device Name: %s\n",devicename);
                // print boot sector information
                break;
            case 'l':
                printf("reached l\n");
                printf("Device Name: %s\n",devicename);
                // list all the directory entries
                break;
            case 'r':
                printf("reached r\n");
                printf("Device Name: %s\n",devicename);
                printf("File Name: %s\n",filename);
                // recovery 8.3 filename
                break;
            case 'm':
                printf("reached m\n");
                printf("Device Name: %s\n",devicename);
                printf("File Name: %s\n",filename);
                printf("MD5: %s\n",md5);
                // recovery 8.3 filename with MD5
                break;
            case 'R':
                printf("reached R\n");
                printf("Device Name: %s\n",devicename);
                printf("File Name: %s\n",filename);
                // recovery long filename
                break;
            default:
                printf("reached to Default, something wrong with interpreter\n");
        }
    }

    return 0;
}

