#include "conf.h"
#include "kernel.h"
#include "proc.h"
#include "q.h"
#include "sem.h"

//------------------------------------------------------------------------
//  signaln -- signal a semaphore n times
//------------------------------------------------------------------------
SYSCALL
signaln(int sem, int count)
{
	struct sentry *sptr;
	int ps;

	ps = disable();
	if (isbadsem(sem) || semaph[sem].sstate == SFREE || count <= 0) {
		restore(ps);
		return SYSERR;
	}
	sptr = &semaph[sem];
	for (; count > 0; count--)
		if ((sptr->semcnt++) < 0)
			ready(getfirst(sptr->sqhead));
	resched();
	restore(ps);

	return OK;
}
