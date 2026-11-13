/*******************************************************************************
 *License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>*
 *This is free software: you are free to change and redistribute it.           *
 *******************************************************************************/

#define _GNU_SOURCE

#include "mexecve.h"
#include <stdio.h>
#include <errno.h>

#include "image.c"
// generate by command: ../calc/calc -q 'import(common);size=:0;falloc("a.out",&size)-->p;assert(p);size;printf("#include <stddef.h>\nconst char image[]={%#.*E};\nsize_t image_size=(size_t)(%:zu);\n",#size,p,#size);free(p)' >image.c
// when a.out was from example.c

extern char **envp __asm__("environ");
int main(int argc,char **argv){
	(void)argc;
	errno=-mexecve(image,image_size,argv,envp);
	perror("mexecve()");
	return 0;
}
