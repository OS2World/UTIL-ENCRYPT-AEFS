/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "mount.h"
#include <stdio.h>
#include <stdlib.h> /* getenv, exit */
#include <rpc/pmap_clnt.h> /* for pmap_unset */
#include <string.h> /* strcmp */
#include <signal.h>
#ifdef __cplusplus
#include <sysent.h> /* getdtablesize, open */
#endif /* __cplusplus */
#include <unistd.h> /* setsid */
#include <sys/types.h>
#include <memory.h>
#include <sys/resource.h> /* rlimit */
#include <syslog.h>

#ifndef SIG_PF
#define	SIG_PF void(*)(int)
#endif

#ifdef DEBUG
#define	RPC_SVC_FG
#endif

#define	_RPCSVC_CLOSEDOWN 120
#ifndef lint
/*static char sccsid[] = "from: @(#)mount.x 1.2 87/09/18 Copyr 1987 Sun Micro";*/
/*static char sccsid[] = "from: @(#)mount.x	2.1 88/08/01 4.0 RPCSRC";*/
static char rcsid[] = "mount.x,v 1.1 1994/08/04 19:01:46 wollman Exp";
#endif /* not lint */
extern int _rpcpmstart;		/* Started by a port monitor ? */
	/* States a server can be in wrt request */

#define	_IDLE 0
#define	_SERVED 1
#define	_SERVING 2

static int _rpcsvcstate = _IDLE;	 /* Set when a request is serviced */

static
void _msgout(char* msg)
{
#ifdef RPC_SVC_FG
	if (_rpcpmstart)
		syslog(LOG_ERR, msg);
	else
		(void) fprintf(stderr, "%s\n", msg);
#else
	syslog(LOG_ERR, msg);
#endif
}

void
mountprog_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		dirpath mountproc_mnt_1_arg;
		dirpath mountproc_umnt_1_arg;
	} argument;
	char *result;
	xdrproc_t xdr_argument, xdr_result;
	char *(*local)(char *, struct svc_req *);

	_rpcsvcstate = _SERVING;
	switch (rqstp->rq_proc) {
	case MOUNTPROC_NULL:
		xdr_argument = (xdrproc_t) xdr_void;
		xdr_result = (xdrproc_t) xdr_void;
		local = (char *(*)(char *, struct svc_req *)) mountproc_null_1_svc;
		break;

	case MOUNTPROC_MNT:
		xdr_argument = (xdrproc_t) xdr_dirpath;
		xdr_result = (xdrproc_t) xdr_fhstatus;
		local = (char *(*)(char *, struct svc_req *)) mountproc_mnt_1_svc;
		break;

	case MOUNTPROC_DUMP:
		xdr_argument = (xdrproc_t) xdr_void;
		xdr_result = (xdrproc_t) xdr_mountlist;
		local = (char *(*)(char *, struct svc_req *)) mountproc_dump_1_svc;
		break;

	case MOUNTPROC_UMNT:
		xdr_argument = (xdrproc_t) xdr_dirpath;
		xdr_result = (xdrproc_t) xdr_void;
		local = (char *(*)(char *, struct svc_req *)) mountproc_umnt_1_svc;
		break;

	case MOUNTPROC_UMNTALL:
		xdr_argument = (xdrproc_t) xdr_void;
		xdr_result = (xdrproc_t) xdr_void;
		local = (char *(*)(char *, struct svc_req *)) mountproc_umntall_1_svc;
		break;

	case MOUNTPROC_EXPORT:
		xdr_argument = (xdrproc_t) xdr_void;
		xdr_result = (xdrproc_t) xdr_exports;
		local = (char *(*)(char *, struct svc_req *)) mountproc_export_1_svc;
		break;

	case MOUNTPROC_EXPORTALL:
		xdr_argument = (xdrproc_t) xdr_void;
		xdr_result = (xdrproc_t) xdr_exports;
		local = (char *(*)(char *, struct svc_req *)) mountproc_exportall_1_svc;
		break;

	default:
		svcerr_noproc(transp);
		_rpcsvcstate = _SERVED;
		return;
	}
	(void) memset((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs(transp, xdr_argument, (caddr_t) &argument)) {
		svcerr_decode(transp);
		_rpcsvcstate = _SERVED;
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, xdr_result, result)) {
		svcerr_systemerr(transp);
	}
	if (!svc_freeargs(transp, xdr_argument, (caddr_t) &argument)) {
		_msgout("unable to free arguments");
		exit(1);
	}
	_rpcsvcstate = _SERVED;
	return;
}