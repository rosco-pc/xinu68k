// fprintf.c - fprintf

#include <stdarg.h>

extern void _doprnt(const char *fmt, va_list args, int (*func)(int, int), int farg);
extern int putc(int, int);

#define	OK	1

//------------------------------------------------------------------------
//  fprintf  --  print a formatted message on specified device (file)
//------------------------------------------------------------------------
int
fprintf(int dev, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	_doprnt(fmt, args, putc, dev);
	va_end(args);

	return OK;
}
