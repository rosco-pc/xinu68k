#include "conf.h"
#include "kernel.h"
#include "network.h"

//------------------------------------------------------------------------
//  ethinit  -  initialize ethernet I/O device and buffers
//------------------------------------------------------------------------
void
ethinit(struct devsw *devptr)
{
	struct etblk *etptr;
	struct dqregs *dqptr;
	struct dcmd *dcmptr;
	short *iptr;
	int i;
	struct dqsetup setup;

	etptr = &eth[devptr->minor];
	devptr->iobuf = (char *)etptr;
	//iosetvec(devptr->num, etptr, etptr);
	etptr->eioaddr = dqptr = (struct dqregs *)devptr->csr;
	etptr->etdev = devptr;
	etptr->etrsem = screate(1);
	etptr->etwsem = screate(1);
	etptr->etrpid = 0;

	// establish read and write buffer descriptor lists

	dcmptr = &etptr->ercmd[1];
	dcmptr->dc_flag = DC_NUSED;
	dcmptr->dc_bufh = DC_ENDL;
	dcmptr->dc_buf = (short) NULL;

	dcmptr = &etptr->ewcmd[1];
	dcmptr->dc_flag = DC_NUSED;
	dcmptr->dc_bufh = DC_ENDL;
	dcmptr->dc_buf = (short) NULL;
	dcmptr->dc_st1 = dcmptr->dc_st2 = DC_INIT;

	// extract physical ethernet address

	for (iptr = (short *)dqptr, i = 0; i < EPADLEN; i++)
		etptr->etpaddr[i] = LOWBYTE & *iptr++;

	ethstrt(etptr, &setup);
}

#ifdef	Neth
struct etblk eth[Neth];
#endif
