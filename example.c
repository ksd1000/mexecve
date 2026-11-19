/******************************************************************************
*License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>*
*This is free software: you are free to change and redistribute it.           *
******************************************************************************/

#define _GNU_SOURCE

#include <stdio.h>

int main(int argc,char **argv){
	printf("argc=%d\n",argc);
	for(int i=0;i<argc;++i)
		printf("argv[%d]=\"%s\"\n",i,argv[i]);
	return 0;
}
