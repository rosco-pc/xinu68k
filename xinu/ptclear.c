#include "conf.h"
#include "kernel.h"
#include "mark.h"
#include "ports.h"

/*------------------------------------------------------------------------
 *  _ptclear  --  used by pdelete and preset to clear a port
 *------------------------------------------------------------------------
 */
void
_ptclear(struct pt *ptptr, int newstate, int (*dispose)(void *))
{
	struct ptnode *p;


	// put port in limbo until done freeing processes
	ptptr->ptstate = PTLIMBO;
	ptptr->ptseq++;
	if ((p = ptptr->pthead) != (struct ptnode *)NULL) {
		for (; p != (struct ptnode *)NULL; p = p->ptnext)
			(*dispose)((void *)p->ptmsg);
		(ptptr->pttail)->ptnext = ptfree;
		ptfree = ptptr->pthead;
	}
	if (newstate == PTALLOC) {
		ptptr->pttail = ptptr->pthead = (struct ptnode *)NULL;
		sreset(ptptr->ptssem, ptptr->ptmaxcnt);
		sreset(ptptr->ptrsem, 0);
	} else {
		sdelete(ptptr->ptssem);
		sdelete(ptptr->ptrsem);
	}
	ptptr->ptstate = newstate;
}
