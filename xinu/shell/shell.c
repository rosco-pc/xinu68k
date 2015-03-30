// shell.c - shell

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <shell.h>
#include <cmd.h>
#include <tty.h>

struct	shvars	Shl;			// globals used by Xinu shell
struct	cmdent	cmds[]  = {		// shell commands
	{"bpool",	FALSE,		x_bpool},
	{"cat",		FALSE,		x_cat},
	{"close",	FALSE,		x_close},
	{"cp",		FALSE,		x_cp},
	{"create",	FALSE,		x_creat},
	{"date",	FALSE,		x_date},
	{"devs",	FALSE,		x_devs},
	{"dg",		FALSE,		x_dg},
	{"echo",	FALSE,		x_echo},
	{"exit",	TRUE,		x_exit},
	{"help",	FALSE,		x_help},
	{"kill",	TRUE,		x_kill},
	{"logout",	TRUE,		x_exit},
	{"mem",		FALSE,		x_mem},
	{"mount",	FALSE,		x_mount},
	{"mv",		FALSE,		x_mv},
	{"netstat",	FALSE,		x_net},
	{"ps",		FALSE,		x_ps},
	{"reboot",	TRUE,		x_reboot},
	{"rf",		FALSE,		x_rf},
	{"rls",		FALSE,		x_rls},
	{"rm",		FALSE,		x_rm},
	{"routes",	FALSE,		x_routes},
	{"ruptime",	FALSE,		x_uptime},
	{"sleep",	FALSE,		x_sleep},
	{"snap",	FALSE,		x_snap},
	{"time",	FALSE,		x_date},
	{"unmount",	FALSE,		x_unmou},
	{"uptime",	FALSE,		x_uptime},
	{"who",		FALSE,		x_who},
	{"?",		FALSE,		x_help}
};
//extern	struct	cmdent	cmds[];
LOCAL	char	errhd[] = "Syntax error\n";// global error messages
LOCAL	char	fmt[]   = "Cannot open %s\n";
LOCAL	char	fmt2[]  = "[%d]\n";

/*------------------------------------------------------------------------
 *  shell  -  Xinu shell with file redirection and background processing
 *------------------------------------------------------------------------
 */
int
shell(int dev)
{
	int	ntokens;
	int	i, j, len;
	int	com;
	char	*outnam, *innam;
	int	stdin, stdout, stderr;
	Bool	backgnd;
	char	ch, mach[SHMLEN];
	int	child;

	Shl.shncmds = sizeof(cmds)/sizeof(struct cmdent);
	for (getname(mach) ; TRUE ; ) {
		fprintf(dev, "%s %% ", mach);
		getutim(&Shl.shlast);
		if ( (len = read(dev, Shl.shbuf, SHBUFLEN)) == 0)
			len = read(dev, Shl.shbuf, SHBUFLEN);
		if (len == EOF)
			break;
		Shl.shbuf[len-1] = NULLCH;
		if ( (ntokens=lexan(Shl.shbuf)) == SYSERR) {
			fprintf(dev, errhd);
			continue;
		} else if (ntokens == 0)
			continue;
		outnam = innam = NULL;
		backgnd = FALSE;

		// handle '&'

		if (Shl.shtktyp[ntokens-1] == '&') {
			ntokens-- ;
			backgnd = TRUE;
		}

		// scan tokens, accumulating length;  handling redirect

		for (len=0,i=0 ; i<ntokens ; ) {
			if ((ch = Shl.shtktyp[i]) == '&') {
				ntokens = -1;
				break;
			} else if (ch == '>') {
				if (outnam != NULL || i >= --ntokens) {
					ntokens = -1;
					break;
				}
				outnam = Shl.shtok[i+1];
				for (ntokens--,j=i ; j<ntokens ; j++) {
					Shl.shtktyp[j] = Shl.shtktyp[j+2];
					Shl.shtok  [j] = Shl.shtok  [j+2];
				}
				continue;
			} else if (ch == '<') {
				if (innam != NULL || i >= --ntokens) {
					ntokens = -1;
					break;
				}
				innam = Shl.shtok[i+1];
				for (ntokens--,j=i ; j < ntokens ; j++) {
					Shl.shtktyp[j] = Shl.shtktyp[j+2];
					Shl.shtok  [j] = Shl.shtok  [j+2];
				}
				continue;
			} else {
				 len += strlen(Shl.shtok[i++]);
			}
		}
		if (ntokens <= 0) {
			fprintf(dev, errhd);
			continue;
		}
		stdin = stdout = stderr = dev;

		// Look up command in table

		for (com=0 ; com<Shl.shncmds ; com++) {
			if (strcmp(cmds[com].cmdnam,Shl.shtok[0]) == 0)
				break;
		}
		if (com >= Shl.shncmds) {
			fprintf(dev, "%s: not found\n", Shl.shtok[0]);
			continue;
		}

		// handle built-in commands with procedure call

		if (cmds[com].cbuiltin) {
			if (innam != NULL || outnam != NULL || backgnd)
				fprintf(dev, errhd);
			else if ( (*cmds[com].cproc)(stdin, stdout,
				stderr, ntokens, Shl.shtok) == SHEXIT)
				break;
			continue;
		}

		// Open files and redirect I/O if specified

		if (innam != NULL && (stdin=open(NAMESPACE,innam,"ro"))
			== SYSERR) {
			fprintf(dev, fmt, innam);
			continue;
		}
		if (outnam != NULL && (stdout=open(NAMESPACE,outnam,"w"))
			== SYSERR) {
			fprintf(dev, fmt, outnam);
			continue;
		}

		// compute space needed for string args. (in bytes)

		len += (ntokens+2) * (sizeof(char *) + sizeof(char));
		if (isodd(len))
			len--;
		control(dev, TCINT, getpid());

		// create process to execute conventional command

		if ( (child = create(cmds[com].cproc, SHCMDSTK, SHCMDPRI,
				Shl.shtok[0],(len/sizeof(int)) + 4,
				stdin, stdout, stderr, ntokens))
				== SYSERR) {
			fprintf(dev, "Cannot create\n");
			close(stdout);
			close(stdin);
			continue;
		}
		addarg(child, ntokens, len);
		setdev(child, stdin, stdout);
		if (backgnd) {
			fprintf(dev, fmt2, child);
			resume(child);
		} else {
			setnok(getpid(), child);
			recvclr();
			resume(child);
			if (receive() == INTRMSG) {
				setnok(BADPID, child);
				fprintf(dev, fmt2, child);
			}
		}
	}
	return(OK);
}
