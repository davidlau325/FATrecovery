#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic.h"

char* devicename = NULL;
char* filename = NULL;
char* md5 = NULL;

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
