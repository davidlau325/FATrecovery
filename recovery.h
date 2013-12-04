#ifndef RECOVERY_H
#define	RECOVERY_H

void recoveryNormal(FILE *dev,BOOTSECTOR be,unsigned int *FAT);
void recoveryLFN(FILE *dev,BOOTSECTOR be,unsigned int *FAT);

#endif	

