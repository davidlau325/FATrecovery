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
    FILE *dev;
    BOOTSECTOR be;
    detect = detectArgv(argc,argv);
    if(detect == 0){
        printf("Usage: ./recover -d [device filename] [other arguments]\n");
        printf("-i                    Print boot sector information\n");
        printf("-l                    List all the directory entries\n");
        printf("-r filename [-m md5]  File recovery with 8.3 filename\n");
        printf("-R filename           File recovery with long filename\n");
    }else{
        if((dev = fopen(devicename,"r+")) == NULL){
            printf("error - fail to open the device file\n");
            perror(devicename);
            return 1;
        }
        fread(&be,sizeof(BOOTSECTOR),1,dev);
        unsigned int totalDataCluster = be.BPB_TotSec32 - (be.BPB_NumFATs * be.BPB_FATSz32) - be.BPB_RsvdSecCnt;
        unsigned int *FAT = malloc(sizeof(unsigned int) * totalDataCluster);

        /* Debugging info
        printf("Total Cluster: %d\n",be.BPB_TotSec32);
        printf("Total Data Cluster: %d\n",totalDataCluster);
        printf("First FAT starts: %d\n",be.BPB_RsvdSecCnt * be.BPB_BytsPerSec);
        */

        fseek(dev,(long) be.BPB_RsvdSecCnt * be.BPB_BytsPerSec,SEEK_SET);
        fread(FAT,sizeof(int),totalDataCluster,dev);

        switch(detect){
            case 'i':
                // printf("Device Name: %s\n",devicename);
                // print boot sector information
                printInfo(be,FAT,totalDataCluster);
                break;
            case 'l':
                // printf("Device Name: %s\n",devicename);
                // list all the directory entries
                listDIR(dev,be,FAT);
                break;
            case 'r':
                // printf("Device Name: %s\n",devicename);
                // printf("File Name: %s\n",filename);
                // recovery 8.3 filename
                toUpperCase(filename);
                recoveryNormal(dev,be,FAT);
                break;
            case 'm':
                // printf("Device Name: %s\n",devicename);
                // printf("File Name: %s\n",filename);
                // printf("MD5: %s\n",md5);
                // recovery 8.3 filename with MD5
                recoveryMD5(dev,be,FAT);
                break;
            case 'R':
                // printf("Device Name: %s\n",devicename);
                // printf("File Name: %s\n",filename);
                // recovery long filename
                recoveryLFN(dev,be,FAT);
                break;
            default:
                printf("reached to Default, something wrong with interpreter\n");
        }
    }

    return 0;
}

