#include "conf.h"
#include "kernel.h"
#include "network.h"

//------------------------------------------------------------------------
//  getname  -  get name of this host and place it where specified
//------------------------------------------------------------------------
SYSCALL
getname(char *name)
{
strcpy(name, "shireen");
/*
	IPaddr myaddr;
	char *p;

	*name = NULLCH;
	if (!Net.mnvalid) {
		getaddr(myaddr);
		if (ip2name(myaddr, Net.myname) == SYSERR)
			return SYSERR;
		Net.mnvalid = TRUE;
	}
	for (p = Net.myname; *p != NULLCH && *p != '.';)
		*name++ = *p++;
	*name = NULLCH;
*/
	return OK;
}
