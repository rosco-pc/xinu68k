#include <stddef.h>
#include <string.h>

#include "conf.h"
#include "kernel.h"
#include "name.h"

//------------------------------------------------------------------------
//  unmount  -  remove an entry from the name prefix mapping table
//------------------------------------------------------------------------
SYSCALL
unmount(char *prefix)
{
	struct nament *nptr;
	int ps;
	int i;

	if (prefix == NULL)
		prefix = NULLSTR;
	else if (strlen(prefix) >= NAMPLEN)
		return SYSERR;
	ps = disable();
	for (i = 0; i < Nam.nnames; i++) {
		nptr = &Nam.nametab[i];
		if (strcmp(prefix, nptr->npre) == 0) {
			for (Nam.nnames--; i < Nam.nnames; i++)
				Nam.nametab[i] = Nam.nametab[i + 1];
			restore(ps);
			return OK;
		}
	}
	restore(ps);

	return SYSERR;
}
