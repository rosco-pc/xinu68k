#include "conf.h"
#include "kernel.h"
#include "network.h"

//------------------------------------------------------------------------
//  rfio  --  perform input or output using remote file server
//------------------------------------------------------------------------
int
rfio(struct devsw *devptr, int rop, char *buff, int len)
{
	struct rfblk *rfptr;
	int retcode;

	rfptr = (struct rfblk *)devptr->iobuf;
	wait(rfptr->rf_mutex);
	if (len < 0 || rfptr->rf_state == RFREE) {
		signal(rfptr->rf_mutex);
		return SYSERR;
	}
	retcode = rfmkpac(rop, rfptr->rf_name, &rfptr->rf_pos, buff, len);
	signal(rfptr->rf_mutex);

	return retcode;
}
