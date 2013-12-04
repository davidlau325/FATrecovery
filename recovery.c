#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entry.h"
#include "basic.h"

/*
available inputs:

extern char* devicename;
extern char* filename;
extern char* md5;
*/
void recoveryLFN(FILE *dev,BOOTSECTOR be,unsigned int *FAT){
	unsigned int preSector = be.BPB_RsvdSecCnt + (be.BPB_FATSz32 * be.BPB_NumFATs) - (2 * be.BPB_SecPerClus);
    unsigned int oneClusterSizeByte = be.BPB_SecPerClus * be.BPB_BytsPerSec;
    unsigned int numEntryPerCluster = oneClusterSizeByte / sizeof(DIRENTRY);
    DIRENTRY *de;
    unsigned char *lfn = malloc(sizeof(char)*32);
    unsigned char *firstByte = malloc(sizeof(char));
    unsigned int NofLFN;
    unsigned char *tempLFNName;
    unsigned char *LFNarray[11];
    unsigned int cluster,startCluster;
    int entry,i,j,k,LFNlength,fnameLength,totalLFNlength,fileFound;
    unsigned char *tempIndex = malloc(sizeof(char));
    char fname[13];
    long currentPoint;
    unsigned char *LFNName = NULL;
    totalLFNlength=0;
    NofLFN=0;
    fileFound=0;

    // printf("PreSector: %d\n",preSector);
    // printf("OneClusterSizeByte: %d\n",oneClusterSizeByte);
    // printf("EntrySize: %d\n",sizeof(DIRENTRY));

    // Loop from Cluster 2 to trace the root dir cluster, for the case of more than 1 cluster for root dir
    for(cluster = be.BPB_RootClus & EOC_HI; cluster && cluster < EOC_LO; cluster = FAT[cluster] & EOC_HI){
        fseek(dev,(long)(preSector + cluster * be.BPB_SecPerClus) * be.BPB_BytsPerSec,SEEK_SET);
        for(entry = 0;entry < numEntryPerCluster;entry++){
        de = malloc(sizeof(DIRENTRY));
        fread(de,sizeof(DIRENTRY),1,dev);
       // printf("first: %hhx, Attr: %hhx\n",de->DIR_Name[0],de->DIR_Attr);

        if(de->DIR_Attr == 0x0f && de->DIR_Name[0] == 0xe5){
            // printf("I got a long file name here\n");

        	currentPoint = ftell(dev);
            fseek(dev,(long)(currentPoint - 32),SEEK_SET);
            memset(lfn,0,sizeof(char)*32);
            fread(lfn,(sizeof(char)*32),1,dev);

            LFNlength=0;
            tempLFNName = malloc(sizeof(char)*32);
            for(i=1;i<32;i+=2){
                if(i == 11){ i+=3;}
                if(i == 26){ continue;}
                if(lfn[i] == 0){ break; }
                // printf("%c\n",lfn[i]);
                tempLFNName[LFNlength++]=lfn[i];
                totalLFNlength++;
            }
           //  printf("LENGTH: %d\n",LFNlength);
            LFNarray[NofLFN] = malloc(sizeof(char)* LFNlength);
            memcpy(LFNarray[NofLFN],tempLFNName,LFNlength);
            NofLFN++;
           // printf("NofLFN: %d\n",NofLFN);

        }else if(de->DIR_Attr != 0x0f && de->DIR_Name[0] == 0xe5){
            startCluster = (((unsigned int) de->DIR_FstClusHI << 16) + de->DIR_FstClusLO) & EOC_HI;
            fnameLength = checkFileName(fname,de->DIR_Name);
            
          	LFNName = malloc(sizeof(char) * 255);
            for(i=(NofLFN-1);i>-1;i--){
                if(i == (NofLFN-1)){
                strcpy(LFNName,LFNarray[i]);
                }else{
                strcat(LFNName,LFNarray[i]);
                }
             //   printf("%s\n",LFNarray[i]);
            }
            LFNName[totalLFNlength]=0;

            if(de->DIR_Attr & 0b00010000){
                printf("This is a folder\n");
            }else{
            if(LFNName != NULL){
                // printf("%s, %s, %u, %u\n",fname,LFNName,de->DIR_FileSize,startCluster);
                if(strcmp(LFNName,filename)==0){
                	// printf("Find the exact same match\n");
                	// printf("Start recovering...\n");

                	if(FAT[startCluster] == 0){
        				FAT[startCluster] = 0xfffffff;
        			printf("%s: recovered\n",filename);
        			fseek(dev,(long)(preSector + (cluster * be.BPB_SecPerClus)) * be.BPB_BytsPerSec + entry * sizeof(DIRENTRY),SEEK_SET);
        			toUpperCase(filename);
        			fwrite(filename,1,1,dev);

        			for(j=1;j<NofLFN;j++){
        				tempIndex[0]=(char)j;
        				fseek(dev,(long)(preSector + (cluster * be.BPB_SecPerClus)) * be.BPB_BytsPerSec + ((entry - j) * sizeof(DIRENTRY)),SEEK_SET);
        				fwrite(tempIndex,1,1,dev);
        			}
        			tempIndex[0] = (char)(NofLFN | 0x40);
        			fseek(dev,(long)(preSector + (cluster * be.BPB_SecPerClus)) * be.BPB_BytsPerSec + ((entry - j) * sizeof(DIRENTRY)),SEEK_SET);
        			fwrite(tempIndex,1,1,dev);

        			for(j=0;j<be.BPB_NumFATs;j++){
        				fseek(dev,(be.BPB_RsvdSecCnt + (j * (long) be.BPB_FATSz32 + be.BPB_BytsPerSec) + (4 * startCluster)),SEEK_SET);
        				fwrite(FAT + startCluster,4,1,dev);
        			}
        				
        			}else{
        				printf("%s: error - fail to recover\n",filename);
        			}
                	fileFound=1;
                	break;
                }

                free(LFNName);
                LFNName = NULL;
            }else{
            	printf("Some problem with the LFN generation\n");
            }
        	}
        	totalLFNlength=0;
   			NofLFN=0;
        }else if(de->DIR_Attr == 0){
        	free(de);
        	break;
        }
        free(de);
        }

    }

    if(fileFound==0){
    	printf("%s: error - file not found\n",filename);
    }
}

void recoveryNormal(FILE *dev,BOOTSECTOR be,unsigned int *FAT){
	unsigned int preSector = be.BPB_RsvdSecCnt + (be.BPB_FATSz32 * be.BPB_NumFATs) - (2 * be.BPB_SecPerClus);
    unsigned int oneClusterSizeByte = be.BPB_SecPerClus * be.BPB_BytsPerSec;
    unsigned int numEntryPerCluster = oneClusterSizeByte / sizeof(DIRENTRY);
    DIRENTRY *de;
    unsigned char *lfn;
    unsigned char *firstByte = malloc(sizeof(char));
    unsigned int NofLFN;
    unsigned char *tempLFNName;
    unsigned char *LFNarray[11];
    unsigned int cluster,startCluster,deletedCluster;
    int entry,i,j,k,LFNlength,fnameLength,totalLFNlength,SameFileName,deletedEntry;
    char fname[13];
    long currentPoint;
    unsigned char *LFNName = NULL;


    // printf("PreSector: %d\n",preSector);
    // printf("OneClusterSizeByte: %d\n",oneClusterSizeByte);
    // printf("EntrySize: %d\n",sizeof(DIRENTRY));

    // Loop from Cluster 2 to trace the root dir cluster, for the case of more than 1 cluster for root dir
    SameFileName=0;
    for(cluster = be.BPB_RootClus & EOC_HI; cluster && cluster < EOC_LO; cluster = FAT[cluster] & EOC_HI){
        fseek(dev,(long)(preSector + cluster * be.BPB_SecPerClus) * be.BPB_BytsPerSec,SEEK_SET);
        for(entry = 0;entry < numEntryPerCluster;entry++){
        de = malloc(sizeof(DIRENTRY));
        fread(de,sizeof(DIRENTRY),1,dev);

        if(de->DIR_Name[0] == 0xe5 && de->DIR_Attr != 0x0f){
        	if(de->DIR_Attr & 0b00010000){
        		printf("Deleted Folder\n");
        		continue;
        	}else{
        		fnameLength = checkFileName(fname,de->DIR_Name);
        		// printf("Deleted: %s\n",fname);
        		// printf("Checking: %s\n",filename);
        		if(strcmp(fname +1,filename +1)==0){
        		//	printf("Yes, they are same!\n");
        			startCluster = (((unsigned int) de->DIR_FstClusHI << 16) + de->DIR_FstClusLO) & EOC_HI;
        		//	printf("Start Cluster: %d\n",startCluster);
        			deletedEntry = entry;
        			deletedCluster = cluster;
        			SameFileName++;
        		}
        	}
        }

        free(de);
        }
    }

    if(SameFileName == 0){
        	printf("%s: error - file not found\n",filename);
        }else if(SameFileName > 1){
        	printf("%s: error - ambiguous\n",filename);
        }else{
        	// recover here
        	// printf("Start recovering...\n");
        	if(FAT[startCluster] == 0){
        		// printf("No occupy yet!\n");
        		FAT[startCluster] = 0xfffffff;
        		// printf("cluster: %d\n",deletedCluster);
        		// printf("entry: %d\n",deletedEntry);

        		fseek(dev,(long)(preSector + (deletedCluster * be.BPB_SecPerClus)) * be.BPB_BytsPerSec + deletedEntry * sizeof(DIRENTRY),SEEK_SET);
        		fwrite(filename,1,1,dev);

        		for(j=0;j<be.BPB_NumFATs;j++){
        			fseek(dev,(be.BPB_RsvdSecCnt + (j * (long) be.BPB_FATSz32 + be.BPB_BytsPerSec) + (4 * startCluster)),SEEK_SET);
        			fwrite(FAT + startCluster,4,1,dev);
        		}
        		printf("%s: recovered\n",filename);
        	}else{
        		printf("%s: error - fail to recover\n",filename);
        	}
        }
}
