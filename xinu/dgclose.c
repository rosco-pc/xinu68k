#include "conf.h"
#include "kernel.h"
#include "proc.h"
#include "network.h"

/*------------------------------------------------------------------------
 *  dgclose  -  close a datagram pseudo device, making it available again
 *------------------------------------------------------------------------
 */
int
dgclose(struct devsw *devptr)
{
	char ps;
	struct dgblk *dgptr;
	struct netq *nqptr;
	int nq;

	dgptr = (struct dgblk *) devptr->dvioblk;
	disable(ps);
	nq = dgptr->dg_netq;
	nqptr = &Net.netqs[nq];
	nqptr->valid = FALSE;
	nqptr->uport = -1;
	nqptr->pid = BADPID;
	dgptr->dg_state = DG_FREE;
	restore(ps);
	return OK;
}
