#include "conf.h"
#include "kernel.h"
#include "proc.h"
#include "tty.h"
#include "io.h"
#include "slu.h"

//------------------------------------------------------------------------
//  ttyinit - initialize buffers and modes for a tty line
//------------------------------------------------------------------------
void
ttyinit(struct devsw *devptr)
{
	struct tty *iptr;
	struct csr *cptr;
	int isconsole;

	// set up interrupt vector and interrupt dispatch table
	iptr = &tty[devptr->minor];
	iosetvec(devptr->num, vecduart, iptr);

	devptr->iobuf = (char *)iptr;			// fill tty control blk
	isconsole = (devptr->num == CONSOLE);		// make console cooked
	iptr->ioaddr = (struct csr *)devptr->csr;	// copy in csr addr.
	iptr->ihead = iptr->itail = 0;			// empty input queue
	iptr->isem = screate(0);			// chars. read so far=0
	iptr->osem = screate(OBUFLEN);			// buffer available=all
	iptr->odsend = 0;				// sends delayed so far
	iptr->ohead = iptr->otail = 0;			// output queue empty
	iptr->ehead = iptr->etail = 0;			// echo queue empty
	iptr->imode = (isconsole ? IMCOOKED : IMRAW);
	iptr->iecho = iptr->evis = isconsole;		// echo console input
	iptr->ierase = iptr->ieback = isconsole;	// console honors erase
	iptr->ierasec = BACKSP;	// using ^h
	iptr->ecrlf = iptr->icrlf = isconsole;		// map RETURN on input
	iptr->ocrlf = iptr->oflow = isconsole;		// map RETURN on output
	iptr->ieof = iptr->ikill = isconsole;		// set line kill == @
	iptr->iintr = FALSE;
	iptr->iintrc = INTRCH;
	iptr->iintpid = BADPID;
	iptr->ikillc = KILLCH;
	iptr->ieofc = EOFC;
	iptr->oheld = FALSE;
	iptr->ostart = STRTCH;
	iptr->ostop = STOPCH;
	iptr->icursor = 0;
	iptr->ifullc = TFULLC;
	iptr->imr = DUART_RxINTABLE;			// enable input interrupts
	iptr->imr &= ~DUART_TxINTABLE;			// disable output  "
	cptr = (struct csr *)devptr->csr;
	cptr->imr = iptr->imr;
}
