/* getbuf.c - getbuf */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <bufpool.h>

/*------------------------------------------------------------------------
 *  getbuf  --  get a buffer from a preestablished buffer pool
 *------------------------------------------------------------------------
 */
SYSCALL	*getbuf(poolid)
int poolid;
{
	char	ps;
	int	*buf;

#ifdef	MEMMARK
	if ( unmarked(bpmark) )
		return((int *)SYSERR);
#endif
	if (poolid<0 || poolid>=nbpools)
		return((int *)SYSERR);
	wait(bptab[poolid].bpsem);
	disable(ps);
	buf = bptab[poolid].bpnext;
	bptab[poolid].bpnext = *buf;
	restore(ps);
	*buf++ = poolid;
	return(buf);
}
