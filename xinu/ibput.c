#include "conf.h"
#include "kernel.h"
#include "io.h"
#include "disk.h"
#include "lfile.h"
#include "dir.h"

//------------------------------------------------------------------------
//  ibput  --  write an iblock back to disk given its number
//------------------------------------------------------------------------
int
ibput(int diskdev, IBADDR inum, struct iblk *loc)
{
	DBADDR dba;
	char *buff;
	char *to, *from;
	int i;
	int ibsem;

	dba = ibtodb(inum);
	buff = getbuf(dskdbp);
	ibsem = ((struct dsblk *)devtab[diskdev].iobuf)->dibsem;
	wait(ibsem);
	read(diskdev, buff, dba);
	to = buff + ibdisp(inum);
	from = (char *)loc;
	for (i = 0; i < sizeof(struct iblk); i++)
		*to++ = *from++;
	write(diskdev, buff, dba);
	signal(ibsem);
	return OK;
}
