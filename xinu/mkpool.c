#include "conf.h"
#include "kernel.h"
#include "mark.h"
#include "bufpool.h"

//------------------------------------------------------------------------
//  mkpool  --  allocate memory for a buffer pool and link together
//------------------------------------------------------------------------
SYSCALL
mkpool(int bufsiz, int numbufs)
{
	int ps;
	int poolid;
	char *where;

	if (unmarked(bpmark))
		poolinit();
	ps = disable();
	if (bufsiz < BPMINB || bufsiz > BPMAXB
	    || numbufs < 1 || numbufs > BPMAXN
	    || nbpools >= NBPOOLS
	    || (where = (char *)getmem((bufsiz + sizeof(int)) * numbufs)) == (char *)SYSERR) {
		restore(ps);
		return SYSERR;
	}
	poolid = nbpools++;
	bptab[poolid].bpnext = where;
	bptab[poolid].bpsize = bufsiz;
	bptab[poolid].bpsem = screate(numbufs);
	bufsiz += sizeof(int);
	for (numbufs--; numbufs > 0; numbufs--, where += bufsiz)
		*((int *)where) = (int) (where + bufsiz);
	*((int *)where) = (int) NULL;
	restore(ps);

	return poolid;
}
