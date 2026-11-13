/*******************************************************************************
 *License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>*
 *This is free software: you are free to change and redistribute it.           *
 *******************************************************************************/

/*
 * History: 2026-11-14
 */

#define _GNU_SOURCE
#include "expr.h"

#include <syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
// to fetch the macros

static void me_itos(char *buf,int val){
	char tmpbuf[32];
	char *p=tmpbuf+32;
	char *p1;
	while(val){
		*(--p)=(val%10)+'0';
		val/=10;
	}
	p1=tmpbuf+32;
	for(;p<p1;++buf,++p){
		*buf=*p;
	}
	*buf=0;
}

static const char *me_paths[]={"/tmp",".",NULL};

#ifdef SYS_open
#define me_open(...) ((int)expr_internal_syscall(SYS_open,__VA_ARGS__))
#else
#define me_open(...) ((int)expr_internal_syscall(SYS_openat,AT_FDCWD,__VA_ARGS__))
#endif

#define CALLED_OPEN(fd) \
	r=(ssize_t)expr_internal_syscall(SYS_write,fd,elf_image,elf_size);\
	if(expr_unlikely(r<0)){\
		out=(int)r;\
		goto err;\
	}\
	if(expr_unlikely((size_t)r<elf_size)){\
		out=-ENOMEM;\
		goto err;\
	}
int mexecve(const void *elf_image,size_t elf_size,char *const *argv,char *const *envp){
#if defined(SYS_execveat)||!defined(SYS_write)
	const char **mp;
	char *p;
	ssize_t r;
	int fd,out;
	char fpath[32];
	if(expr_unlikely(!elf_image||elf_size<4||!argv||!envp))
		return -EINVAL;
	// C standard does not ensure SYS_* are macros.
	fd=(int)expr_internal_syscall(SYS_memfd_create,"",MFD_CLOEXEC);
	if(expr_unlikely(fd<0))
		return fd;
	// memfd_create() is older than execveat()
	CALLED_OPEN(fd);
	out=(int)expr_internal_syscall(SYS_execveat,fd,"",argv,envp,AT_EMPTY_PATH);
	if(out!=-EACCES&&out!=-ENOSYS&&out!=-EPERM)
		goto err;
	expr_internal_syscall(SYS_close,fd);
	for(mp=me_paths;*mp;++mp){
		fd=me_open(*mp,O_TMPFILE|O_RDWR|O_CLOEXEC,0700);
		if(fd>=0)
			goto open_ok;
	}
	out=fd;
	goto err;
open_ok:
	CALLED_OPEN(fd);
	p=fpath;
	*(p++)='/';
	*(p++)='p';
	*(p++)='r';
	*(p++)='o';
	*(p++)='c';
	*(p++)='/';
	*(p++)='s';
	*(p++)='e';
	*(p++)='l';
	*(p++)='f';
	*(p++)='/';
	*(p++)='f';
	*(p++)='d';
	*(p++)='/';
	me_itos(p,fd);
	out=me_open(fpath,O_RDONLY|O_CLOEXEC,0700);
	if(expr_unlikely(out<0))
		goto err;
	expr_internal_syscall(SYS_close,fd);
	fd=out;
	out=(int)expr_internal_syscall(SYS_execveat,fd,"",argv,envp,AT_EMPTY_PATH);
err:
	expr_internal_syscall(SYS_close,fd);
	return out;
#else
#error "not implemented"
#endif
}
