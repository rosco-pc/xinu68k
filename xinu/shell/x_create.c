#include <stddef.h>
#include <string.h>

#include "conf.h"
#include "kernel.h"
#include "a.out.h"

static char symfile[] = "a.out";	// name of object file to search

//------------------------------------------------------------------------
//  x_creat  -  (command create) create a process given a starting address
//------------------------------------------------------------------------
COMMAND
x_creat(int stdin, int stdout, int stderr, int nargs, char *args[])
{
	int ssize, prio;
	struct exec *aoutptr;
	int dev, len;
	int pid;
	int (*loc) ();
	char *buf;
	Bool found;
	long offset;
	struct nlist *symptr;
	struct nlist *last;
	char tmp[30];

	if (nargs < 4 || nargs > 5) {
		fprintf(stderr,
			"usage: create procedure stack-size prio [name]\n");
		return SYSERR;
	}
	ssize = atoi(args[2]);
	prio = atoi(args[3]);
	if ((dev = open(NAMESPACE, symfile, "ro")) == SYSERR) {
		fprintf(stderr, "Cannot open %s\n", symfile);
		return SYSERR;
	}
	if (((int)(buf = (char *)getmem(512))) == SYSERR) {
		fprintf(stderr, "no memory\n");
		return SYSERR;
	}
	strlcpy(tmp, "_", sizeof(tmp));
	strlcat(tmp, args[1], sizeof(tmp));
	printf("Looking up %s\n", tmp);
	read(dev, buf, 16);
	aoutptr = (struct exec *)buf;
	offset = (long)(aoutptr->a_text + aoutptr->a_data
			+ (unsigned)sizeof(struct exec));
	seek(dev, offset);
	for (found = FALSE; !found;) {
		len = read(dev, buf, 42 * sizeof(struct nlist));
		if (len <= 0) {
			fprintf(stderr, "not found\n");
			close(dev);
			freemem(buf, 512);
			return SYSERR;
		}
		last = (struct nlist *)&buf[len];
		for (symptr = (struct nlist *)buf; symptr < last; symptr++) {
			if (symptr->n_type == (N_TEXT | N_EXT)
			    && strncmp(symptr->n_name, tmp, 8) == 0) {
				loc = (int (*)())symptr->n_value;
				found = TRUE;
				break;
			}
		}
	}
	close(dev);
	freemem(buf, 512);
	pid = create(loc, ssize, prio, nargs == 5 ? args[4] : tmp, 0);
	fprintf(stderr, "[%d]\n", pid);

	return resume(pid);
}
