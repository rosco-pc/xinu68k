#include "conf.h"
#include "kernel.h"
#include "network.h"

//------------------------------------------------------------------------
//  udpnxtp  -  return the next available UDP local "port" number
//------------------------------------------------------------------------
int
udpnxtp(void)
{
	int i;
	int try;
	Bool inuse;
	struct netq *nqptr;
	int ps;

	ps = disable();
	for (inuse = TRUE; inuse;) {
		inuse = FALSE;
		try = Net.nxtprt++;
		for (i = 0; i < NETQS; i++)
			if ((nqptr = &Net.netqs[i])->valid &&
			    nqptr->uport == try) {
				inuse = TRUE;
				break;
			}
	}
	restore(ps);

	return try;
}
