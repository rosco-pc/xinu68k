#include "conf.h"
#include "kernel.h"
#include "shell.h"

//------------------------------------------------------------------------
//  x_kill  -  (command kill) terminate a process
//------------------------------------------------------------------------
BUILTIN
x_kill(int stdin, int stdout, int stderr, int nargs, char *args[])
{
	int pid;

	if (nargs != 2) {
		fprintf(stderr, "use: kill process-id\n");
		return SYSERR;
	}
	if ((pid = atoi(args[1])) == getpid())
		fprintf(stderr, "Shell killed\n");
	return kill(pid);
}
