#include "conf.h"
#include "kernel.h"
#include "tty.h"
#include "io.h"
#include "slu.h"

//------------------------------------------------------------------------
//  writecopy - high-speed copy from user's buffer into system buffer
//------------------------------------------------------------------------
static int
writecopy(char *buff, struct tty *ttyp, int count)
{
	int avail;
	char *cp, *qhead, *qend, *uend;

	avail = scount(ttyp->osem);
	qhead = &ttyp->obuff[ttyp->ohead];
	qend = &ttyp->obuff[OBUFLEN];
	cp = buff;
	uend = buff + count;
	while (avail-- > 1 && cp < uend) {
		if (*cp == NEWLINE && ttyp->ocrlf) {
			*qhead++ = RETURN;
			--avail;
			if (qhead >= qend)
				qhead = ttyp->obuff;
		}
		*qhead++ = *cp++;
		if (qhead >= qend)
			qhead = ttyp->obuff;
	}			// avail decremented one
	ttyp->ohead = qhead - ttyp->obuff;	// extra time when loop
	sreset(ttyp->osem, ++avail);	// condition fails.
	ttyp->imr |= DUART_TxINTABLE;
	(ttyp->ioaddr)->imr = ttyp->imr;

	return cp - buff;
}

//------------------------------------------------------------------------
//  ttywrite - write one or more characters to a tty device
//------------------------------------------------------------------------
int
ttywrite(struct devsw *devptr, char *buff, int count)
{
	struct tty *ttyp;
	int ncopied;
	int ps;

	if (count < 0)
		return SYSERR;
	if (count == 0)
		return OK;
	ps = disable();
	ttyp = &tty[devptr->minor];
	count -= (ncopied = writecopy(buff, ttyp, count));
	buff += ncopied;
	for (; count > 0; count--)
		ttyputc(devptr, *buff++);
	restore(ps);

	return OK;
}
