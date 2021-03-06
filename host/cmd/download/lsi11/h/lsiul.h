/* lsiul.h -- definitions specific to PDP-11 uploader */

#define	MAXADDR		0157777
#define	CSRBASE		0177560
#define	LOADSTART	0
typedef	unsigned short	lsiword;

#define	LOADER		"/usr/Xinu/lib/sul.boot.lsi"
#define	MAXSERPKT	526
#define	STACKLEN	20
#define INTDISABLE	0340
#define	NOAUTOSTART	-1
#define	DEFFILE		"core11"
#define	ACKTIME		3

struct	spkt	{
	lsiword	len, base;
	lsiword	buf[MAXSERPKT];
	lsiword	sum;
};
