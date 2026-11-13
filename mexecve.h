/*******************************************************************************
 *License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>*
 *This is free software: you are free to change and redistribute it.           *
 *******************************************************************************/

/*
 * History: 2026-11-14
 */

#ifndef _MEXECVE_H_
#define _MEXECVE_H_

#include <stddef.h>

int mexecve(const void *elf_image,size_t elf_size,char *const *argv,char *const *envp);

#endif
