#ifndef BASIC_H
#define	BASIC_H

extern char* devicename;
extern char* filename;
extern char* md5;

char detectArgv(int argc,char** argv);
void printInfo(BOOTSECTOR be,unsigned int *FAT,unsigned int totalDataCluster);
void listDIR(FILE *dev,BOOTSECTOR be,unsigned int *FAT);

#endif

