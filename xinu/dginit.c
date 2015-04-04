#include "conf.h"
#include "kernel.h"
#include "network.h"

//------------------------------------------------------------------------
//  dginit  -  initialize datagram protocol pseudo device marking it free
//------------------------------------------------------------------------
void
dginit(struct devsw *devptr)
{
	struct dgblk *dgptr;

	devptr->dvioblk = (char *)(dgptr = &dgtab[devptr->dvminor]);
	dgptr->dg_dnum = devptr->dvnum;
	dgptr->dg_state = DG_FREE;
}

#ifdef	Ndg
struct dgblk dgtab[Ndg];	// dg device control blocks
#endif