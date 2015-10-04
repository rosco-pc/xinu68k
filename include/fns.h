struct epacket;
struct dgblk;
struct dqsetup;
struct dreg;
struct dsblk;
struct devsw;
struct etblk;
struct flblk;
struct fphdr;
struct mblock;
struct pt;
struct tty;
struct xgram;

#include "disk.h"
#include "iblock.h"
#include "ip.h"

SYSCALL access(char *name, int mode);
void adump(void);
int arp_in(struct epacket *packet, int dev);
int arpfind(IPaddr faddr);
void arpinit(void);
int bpdump(void);
SYSCALL chprio(int pid, int newprio);
int ckmode(char *mode);
void clkinit(void);
void clkint(void);
SYSCALL close(int descrp);
SYSCALL control(int descrp, int func, int addr);
SYSCALL create(PROCESS(*procaddr) (), int ssize, int priority, char *name,
	       int nargs, ...);
void devdump(void);
int dfalloc(void);
struct fdes *dfdsearch(struct dsblk *dsptr, char *filenam, int mbits);
int dgalloc(void);
int dgclose(struct devsw *devptr);
int dgcntl(struct devsw *devptr, int func, int arg);
int dgdump(void);
void dginit(struct devsw *devptr);
int dgmcntl(struct devsw *devptr, int func, char *addr);
int dgmopen(struct devsw *devptr, char *forport, int locport);
int dgparse(struct dgblk *dgptr, char *fspec);
int dgread(struct devsw *devptr, struct xgram *buff, int len);
int dgwrite(struct devsw *devptr, struct xgram *buff, int len);
void dot2ip(char *ip, int nad1, int nad2, int nad3, int nad4);
int dscntl(struct devsw *devptr, int func);
void dsinit(struct devsw *devptr);
INTPROC dsinter(struct dsblk *dsptr);
char *dskbcpy(void *oldbuf);
int dskenq(struct dreq *drptr, struct dsblk *dsptr);
int dskqopt(struct dreq *p, struct dreq *q, struct dreq *drptr);
void dskstart(struct dsblk *dsptr);
int dsksync(struct devsw *devptr);
int dsopen(struct devsw *devptr, char *filenam, char *mode);
int dsread(struct devsw *devptr, void *buff, DBADDR block);
int dsseek(struct devsw *devptr, DBADDR block);
int dswrite(struct devsw *devptr, void *buff, DBADDR block);
int dudir(int diskdev);
int dumkfs(int diskdev, int niblks, unsigned int nblocks, int diskid,
	   Bool verbose);
int dumkdl(int diskdev, DBADDR firstfb, DBADDR lastfb, Bool verbose);
int dumkil(int diskdev, int niblks, Bool verbose);
void ethinit(struct devsw *devptr);
INTPROC ethinter(struct etblk *etptr);
int ethread(struct devsw *devptr, char *buff, int len);
int ethrstrt(struct etblk *etptr, char *buf, int len);
int ethstrt(struct etblk *etptr, struct dqsetup *setup);
int ethwrite(struct devsw *devptr, char *buff, int len);
void ethwstrt(struct etblk *etptr, char *buf, int len, int setup);
SYSCALL freebuf(void *buf);
SYSCALL freemem(void *block, size_t size);
SYSCALL getaddr(IPaddr address);
void *getbuf(int poolid);
SYSCALL getc(int descrp);
int getfirst(int head);
int getlast(int tail);
SYSCALL *getmem(size_t nbytes);
SYSCALL getname(char *name, size_t size);
SYSCALL getnet(IPaddr address);
int getpath(IPaddr faddr);
SYSCALL getpid(void);
SYSCALL getprio(int pid);
SYSCALL *getstk(size_t size);
SYSCALL gettime(long *timvar);
SYSCALL getutim(long *timvar);
void ibclear(struct iblk *ibptr, long ibbyte);
int ibfree(int diskdev, IBADDR inum);
void ibget(int diskdev, IBADDR inum, struct iblk *loc);
int iblfree(int diskdev, IBADDR iblist);
int ibnew(int diskdev, int writedir);
int ibput(int diskdev, IBADDR inum, struct iblk *loc);
int icmp_in(struct epacket *packet, int icmpp, int lim);
int init(int descrp);
int nulluser(void);
int insert(int proc, int head, int key);
int insertd(int pid, int head, int key);
int disable(void);
void enable(void);
void restore(int sr);
int ioerr(void);
INTPROC interr(void);
int iosetvec(int dev, void (*handler)(void), void *arg);
int ionull(void);
void initnull(void);
SYSCALL ip2name(IPaddr ip, char *nam);
int ip_in(struct epacket *packet, int icmpp, int lim);
int ipsend(IPaddr faddr, struct epacket *packet, int datalen);
SYSCALL kill(int pid);
int kprintf(const char *fmt, ...);
int lfclose(struct devsw *devptr);
int lfgetc(struct devsw *devptr);
void lfinit(struct devsw *devptr);
int lfputc(struct devsw *devptr, int ch);
int lfread(struct devsw *devptr, char *buff, int count);
int lfsdfree(int diskdev, DBADDR dba);
int lfseek(struct devsw *devptr, long offset);
void lfsetup(int diskdev, struct flblk *flptr);
int lfsflush(struct flblk *flptr);
int lfsnewd(int diskdev, struct flblk *flptr);
int lfwrite(struct devsw *devptr, char *buff, int count);
void _mkinit(void);
SYSCALL mark(int *loc);
void mdump(void);
struct epacket *mkarp(int typ, int op, IPaddr spaddr, IPaddr tpaddr);
SYSCALL mkpool(int bufsiz, int numbufs);
SYSCALL mount(char *prefix, int dev, char *replace);
void nameinit(void);
SYSCALL namemap(char *name, char *newname);
int nameopen(struct devsw *devptr, char *filenam, char *mode);
SYSCALL namereplace(char *name, char *newname, size_t size);
void ndump(void);
int netdump(void);
PROCESS netin(int userpid);
int netinit(void);
int netnum(IPaddr netpart, IPaddr address);
PROCESS netout(int userpid, int icmpp);
int newqueue(void);
int nqalloc(void);
SYSCALL open(int descrp, char *nam, char *mode);
void pause(void);
SYSCALL pcount(int portid);
SYSCALL pcreate(int count);
SYSCALL pdelete(int portid, int (*dispose) (void *));
SYSCALL pinit(int maxmsgs);
int poolinit(void);
void prdump(void);
void prdumph(void);
void prdumpa(void);
SYSCALL preceive(int portid);
SYSCALL preset(int portid, int (*dispose) (void *));
SYSCALL psend(int portid, uword msg);
void _ptclear(struct pt *ptptr, int newstate, int (*dispose) (void *));
SYSCALL putc(int descrp, int ch);
void qdump(void);
void qdumph(void);
void qdumpa(void);
int enqueue(int item, int tail);
int dequeue(int item);
int rarp_in(struct epacket *packet, int dev);
SYSCALL read(int descrp, void *buff, int count);
int ready(int pid);
int readysched(int pid);
SYSCALL receive(void);
SYSCALL recvclr(void);
SYSCALL recvtim(int maxwait);
SYSCALL remove(char *name, int key);
SYSCALL rename(char *old, char *new);
int resched(void);
SYSCALL resume(int pid);
int rfalloc(void);
int rfclose(struct devsw *devptr);
int rfcntl(struct devsw *devptr, int func, char *addr, char *addr2);
void rfdump(void);
int rfgetc(struct devsw *devptr);
void rfinit(struct devsw *devptr);
int rfio(struct devsw *devptr, int rop, char *buff, int len);
int rfmkpac(int rop, char *rname, long *rpos, char *buff, int len);
int rfopen(struct devsw *devptr, char *name, char *mode);
int rfputc(struct devsw *devptr, int ch);
int rfread(struct devsw *devptr, char *buff, int len);
int rfseek(struct devsw *devptr, long offset);
int rfsend(struct fphdr *fptr, int reqlen, int rplylen);
int rfwrite(struct devsw *devptr, char *buff, int len);
int route(IPaddr faddr, struct epacket *packet, int totlen);
void rwho(void);
PROCESS rwhod(void);
PROCESS rwhoind(void);
SYSCALL scount(int sem);
SYSCALL screate(int count);
SYSCALL sdelete(int sem);
SYSCALL seek(int descrp, long pos);
SYSCALL send(int pid, int msg);
SYSCALL sendf(int pid, int msg);
SYSCALL setdev(int pid, int dev1, int dev2);
SYSCALL setnok(int nok, int pid);
SYSCALL signal(int sem);
SYSCALL signaln(int sem, int count);
SYSCALL sleep(int n);
SYSCALL sleep10(int n);
int sndrarp(void);
SYSCALL sreset(int sem, int count);
void stopclk(void);
void strtclk(void);
SYSCALL suspend(int pid);
void tdump(void);
void tdumph(void);
void tdump1(int tnum);
int main(void);
int ttycntl(struct devsw *devptr, int func);
int ttygetc(struct devsw *devptr);
INTPROC ttyint(void *arg);
void ttyinit(struct devsw *devptr);
int ttyopen(struct devsw *devptr, char *nam, char *mode);
int ttyputc(struct devsw *devptr, int ch);
int ttyread(struct devsw *devptr, char *buff, int count);
int ttywrite(struct devsw *devptr, char *buff, int count);
PROCESS udpecho(void);
int udpnxtp(void);
int udpsend(IPaddr faddr, int fport, int lport, struct epacket *packet,
	    int datalen);
SYSCALL unmount(char *prefix);
SYSCALL unsleep(int pid);
void userret(void);
void vecduart(void);
SYSCALL wait(int sem);
INTPROC wakeup(void);
SYSCALL write(int descrp, const void *buff, int count);
void xdone(void);

int login(int dev);
int shell(int dev);

void setclkr(void);
void panic(const char *);
void lowcore(void);
void ctxsw(uword *, uword *);
void restart(void);

// Shell.
int addarg(int pid, int nargs, int len);
int ascdate(long time, char *str);
int lexan(char *line);
int login(int dev);
int shell(int dev);
int x_bpool(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_cat(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_close(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_cp(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_creat(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_date(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_devs(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_dg(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_echo(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_exit(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_help(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_kill(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_mem(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_mount(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_mv(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_net(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_ps(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_reboot(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_rf(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_rls(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_rm(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_routes(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_sleep(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_snap(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_unmou(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_uptime(int stdin, int stdout, int stderr, int nargs, char *args[]);
int x_who(int stdin, int stdout, int stderr);
