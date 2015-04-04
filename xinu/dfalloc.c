#include "conf.h"
#include "kernel.h"
#include "disk.h"
#include "lfile.h"

/*------------------------------------------------------------------------
 *  dfalloc  --  allocate a device table entry for a disk file; return id
 *------------------------------------------------------------------------
 */
#ifdef	Ndf
int
dfalloc(void)
{				// assume exclusion for dir. provided by caller
	for (int i = 0; i < Ndf; i++)
		if (fltab[i].fl_pid == 0) {
			fltab[i].fl_pid = getpid();
			return i;
		}

	return SYSERR;
}
#endif
