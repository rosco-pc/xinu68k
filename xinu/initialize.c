#include "conf.h"
#include "kernel.h"
#include "proc.h"
#include "sem.h"
#include "sleep.h"
#include "mem.h"
#include "tty.h"
#include "q.h"
#include "io.h"
#include "disk.h"
#include "network.h"

// Declarations of major kernel variables
struct pentry proctab[NPROC];	// process table
int nextproc;			// next process slot to use in create
struct sentry semaph[NSEM];	// semaphore table
int nextsem;			// next semaphore slot to use in screate
struct qent q[NQENT];		// q table (see queue.c)
int nextqueue;			// next slot in q structure to use
char *maxaddr;			// max memory address (set by sizmem)
#ifdef	NDEVS
struct intmap intmap[NDEVS];	// interrupt dispatch table
#endif
struct mblock memlist;		// list of free memory blocks
#ifdef	Ntty
struct tty tty[Ntty];		// SLU buffers and mode control
#endif

// active system status
int numproc;			// number of live user processes
int currpid;			// id of currently running process
int reboot = 0;			// non-zero after first boot

int rdyhead, rdytail;		// head/tail of ready list (q indexes)
char vers[] = VERSION;		// Xinu version printed at startup

//**********************************************************************
//**				NOTE:				      **
//**								      **
//**   This is where the system begins after the C environment has    **
//**   been established.  Interrupts are initially DISABLED, and      **
//**   must eventually be enabled explicitly.  This routine turns     **
//**   itself into the null process after initialization.  Because    **
//**   the null process must always remain ready to run, it cannot    **
//**   execute code that might cause it to be suspended, wait for a   **
//**   semaphore, or put to sleep, or exit.  In particular, it must   **
//**   not do I/O unless it uses kprintf for polled output.           **
//**								      **
//**********************************************************************

//------------------------------------------------------------------------
//  sysinit  --  initialize all Xinu data structures and devices
//------------------------------------------------------------------------
static int
sysinit(void)
{
	static int (*nulluserp)() = &nulluser;
	static uword *nulluserpp = (uword *)&nulluserp;

	int i;
	struct pentry *pptr;
	struct sentry *sptr;
	struct mblock *mptr;

	numproc = 0;		// initialize system variables
	nextproc = NPROC - 1;
	nextsem = NSEM - 1;
	nextqueue = NPROC;	// q[0..NPROC-1] are processes

	// initialize free memory list
	memlist.mnext = mptr = (struct mblock *)roundew(&end);
	mptr->mnext = (struct mblock *)NULL;
	mptr->mlen = (uword)truncew((uword)maxaddr - NULLSTK - (uword)&end);

	for (i = 0; i < NPROC; i++)	// initialize process table
		proctab[i].pstate = PRFREE;

	pptr = &proctab[NULLPROC];	// initialize null process entry
	pptr->pstate = PRCURR;
	pptr->pprio = 0;
	strcpy(pptr->pname, "prnull");
	pptr->plimit = ((char *)maxaddr) - NULLSTK - sizeof(int);
	pptr->pbase = (char *)maxaddr;
	*((int *)pptr->pbase) = MAGIC;
	pptr->paddr = (char *)(*nulluserpp);
	pptr->phasmsg = FALSE;
	pptr->pargs = 0;
	currpid = NULLPROC;

	for (i = 0; i < NSEM; i++) {	// initialize semaphores
		(sptr = &semaph[i])->sstate = SFREE;
		sptr->sqtail = 1 + (sptr->sqhead = newqueue());
	}

	rdytail = 1 + (rdyhead = newqueue());	// initialize ready list

	_mkinit();		// initialize memory marking
#ifdef	RTCLOCK
	clkinit();		// initialize r.t.clock
#endif
#ifdef	Ndsk
	dskdbp = mkpool(DBUFSIZ, NDBUFF);	// initialize disk buffers
	dskrbp = mkpool(DREQSIZ, NDREQ);
#endif
	for (i = 0; i < NDEVS; i++)	// initialize devices
		init(i);

	return OK;
}

//------------------------------------------------------------------------
//  nulluser  -- initialize system and become the null process (id==0)
//------------------------------------------------------------------------
int
nulluser(void)
{				// babysit CPU when no one home
	int userpid;

	kprintf("\n\nXinu Version %s", vers);
	if (reboot++ > 0)
		kprintf("   (reboot %d)", reboot);
	kprintf("\n");
	sysinit();		// initialize all of Xinu

	kprintf("%lu real mem\n", (uword)maxaddr + sizeof(int));
	kprintf("%lu avail mem\n", (uword)maxaddr - (uword)&end + sizeof(int));
	kprintf("clock %sabled\n\n", clkruns == 1 ? "en" : "dis");
	enable();		// enable interrupts

	// create a process to execute the user's main program
	userpid = create(main, INITSTK, INITPRIO, INITNAME, INITARGS);

#ifdef	NETDAEMON
	// start the network input daemon process
	resume(create(NETIN, NETISTK, NETIPRI, NETINAM, NETIARGC, userpid));
#else
	resume(userpid);
#endif

	while (TRUE) {		// run forever without actually
		pause();	// executing instructions
	}

	return SYSERR;
}
