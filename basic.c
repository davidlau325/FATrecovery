#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entry.h"
#include "basic.h"

char* devicename = NULL;
char* filename = NULL;
char* md5 = NULL;

void listDIR(FILE *dev,BOOTSECTOR be,unsigned int *FAT){
    unsigned int preSector = be.BPB_RsvdSecCnt + (be.BPB_FATSz32 * be.BPB_NumFATs) - (2 * be.BPB_SecPerClus);
    unsigned int oneClusterSizeByte = be.BPB_SecPerClus * be.BPB_BytsPerSec;
    unsigned int numEntryPerCluster = oneClusterSizeByte / sizeof(DIRENTRY);
    DIRENTRY *de;
    unsigned char *firstByte = malloc(sizeof(char));
    unsigned int NofLFN;
    unsigned int cluster;
    int entry;
    // printf("PreSector: %d\n",preSector);
    // printf("OneClusterSizeByte: %d\n",oneClusterSizeByte);
    // printf("EntrySize: %d\n",sizeof(DIRENTRY));

    // Loop from Cluster 2 to trace the root dir cluster, for the case of more than 1 cluster for root dir
    for(cluster = be.BPB_RootClus & EOC_HI; cluster && cluster < EOC_LO; cluster = FAT[cluster] & EOC_HI){
        fseek(dev,(long)(preSector + cluster * be.BPB_SecPerClus) * be.BPB_BytsPerSec,SEEK_SET);
        for(entry = 0;entry < numEntryPerCluster;entry++){
        de = malloc(sizeof(DIRENTRY));
        fread(de,sizeof(DIRENTRY),1,dev);
        if(de->DIR_Attr == 0x0f){
            printf("I got a long file name here\n");
            // use ftell to check
        }else if(de->DIR_Name[0] != 0 && de->DIR_Name[0] != 0xe5){
            printf("8.3 here\n");
        }else{

            printf("no more here\n");
        }
        free(de);
        }
    }

}

void printInfo(BOOTSECTOR be,unsigned int *FAT,unsigned int totalDataCluster){
    int i;
    int numOfAlloc = 0;
    int numOfFree = 0;
    printf("Number of FATs = %hhu\n",be.BPB_NumFATs);
    printf("Number of bytes per sector = %hu\n",be.BPB_BytsPerSec);
    printf("Number of sectors per cluster = %hhu\n",be.BPB_SecPerClus);
    printf("Number of reserved sectors = %hu\n",be.BPB_RsvdSecCnt);
    for(i=2;i<totalDataCluster;i++){
           // printf("%d: 0x%x (%d)\n",i,FAT[i],FAT[i]);
        if(FAT[i]==0){
            numOfFree++;
        }else{
            numOfAlloc++;
        }
    }
    printf("Number of allocated clusters = %d\n",numOfAlloc);
    printf("Number of free clusters = %d\n",numOfFree+2);
}

char detectArgv(int argc,char** argv){
    int i;
    int hasD = 0;
    int hasI = 0;
    int hasL = 0;
    int hasR = 0;
    int hasr = 0;
    int hasM = 0;

    if(argc < 4){
        return 0;
    }else{
    	for(i = 1; i < argc; i++){
    		if(hasD == 2){
    			if(argv[i][0]=='-'){
    				return 0;
    			}else{
    				devicename = argv[i];
    				hasD=1;
    			}
    		}else if(hasr == 2){
    			if(argv[i][0]=='-'){
    				return 0;
    			}else{
    				filename = argv[i];
    				hasr=1;
    			}
    		}else if(hasR == 2){
    			if(argv[i][0]=='-'){
    				return 0;
    			}else{
    				filename = argv[i];
    				hasR=1;
    			}
    		}else if(hasM == 2){
    			if(argv[i][0]=='-'){
    				return 0;
    			}else{
    				md5 = argv[i];
    				hasM=1;
    			}
    		}else if(strcmp(argv[i],"-d")==0){
    			if(hasD == 0){ hasD = 2; }else{ return 0; }
    		}else if(strcmp(argv[i],"-r")==0){
    			if(hasr == 0){ hasr = 2; }else{ return 0; }
    		}else if(strcmp(argv[i],"-R")==0){
    			if(hasR == 0){ hasR = 2; }else{ return 0; }
    		}else if(strcmp(argv[i],"-m")==0){
    			if(hasM == 0){ hasM = 2; }else{ return 0; }
    		}else if(strcmp(argv[i],"-i")==0){
    			if(hasI == 0){ hasI = 1; }else{ return 0; }
    		}else if(strcmp(argv[i],"-l")==0){
    			if(hasL == 0){ hasL = 1; }else{ return 0; }
    		}else{
    			return 0;
    		}
    	}
    	if(hasD == 0 || hasD == 2){ return 0;}
    	if((hasI + hasL + hasR + hasr)>1){ return 0; }
    	if((hasM + hasI) > 1 || (hasM + hasL) > 1 || (hasM + hasR) > 1){ return 0;}
    	if(hasI == 1){ return 'i';}
    	if(hasL == 1){ return 'l';}
		if(hasr == 1){ if(hasM == 1){ return 'm';}
					   else if(hasM == 2){ return 0; }
					   else{return 'r';}
					 }
		if(hasR == 1){ return 'R';}
		return 0;
    }
}

