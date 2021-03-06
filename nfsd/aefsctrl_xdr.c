/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "aefsctrl.h"

bool_t
xdr_addfsargs(register XDR *xdrs, addfsargs *objp)
{

	register long *buf;


	if (xdrs->x_op == XDR_ENCODE) {
		if (!xdr_string(xdrs, &objp->path, AEFSCTRL_MAXPATHLEN))
			return (FALSE);
		if (!xdr_string(xdrs, &objp->key, 1024))
			return (FALSE);
		buf = XDR_INLINE(xdrs, 6 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			if (!xdr_int(xdrs, &objp->flags))
				return (FALSE);
			if (!xdr_int(xdrs, &objp->stor_uid))
				return (FALSE);
			if (!xdr_int(xdrs, &objp->stor_gid))
				return (FALSE);
			if (!xdr_int(xdrs, &objp->stor_mode))
				return (FALSE);
			if (!xdr_int(xdrs, &objp->fs_uid))
				return (FALSE);
			if (!xdr_int(xdrs, &objp->fs_gid))
				return (FALSE);
		} else {
			IXDR_PUT_LONG(buf, objp->flags);
			IXDR_PUT_LONG(buf, objp->stor_uid);
			IXDR_PUT_LONG(buf, objp->stor_gid);
			IXDR_PUT_LONG(buf, objp->stor_mode);
			IXDR_PUT_LONG(buf, objp->fs_uid);
			IXDR_PUT_LONG(buf, objp->fs_gid);
		}
		return (TRUE);
	} else if (xdrs->x_op == XDR_DECODE) {
		if (!xdr_string(xdrs, &objp->path, AEFSCTRL_MAXPATHLEN))
			return (FALSE);
		if (!xdr_string(xdrs, &objp->key, 1024))
			return (FALSE);
		buf = XDR_INLINE(xdrs, 6 * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			if (!xdr_int(xdrs, &objp->flags))
				return (FALSE);
			if (!xdr_int(xdrs, &objp->stor_uid))
				return (FALSE);
			if (!xdr_int(xdrs, &objp->stor_gid))
				return (FALSE);
			if (!xdr_int(xdrs, &objp->stor_mode))
				return (FALSE);
			if (!xdr_int(xdrs, &objp->fs_uid))
				return (FALSE);
			if (!xdr_int(xdrs, &objp->fs_gid))
				return (FALSE);
		} else {
			objp->flags = IXDR_GET_LONG(buf);
			objp->stor_uid = IXDR_GET_LONG(buf);
			objp->stor_gid = IXDR_GET_LONG(buf);
			objp->stor_mode = IXDR_GET_LONG(buf);
			objp->fs_uid = IXDR_GET_LONG(buf);
			objp->fs_gid = IXDR_GET_LONG(buf);
		}
		return (TRUE);
	}

	if (!xdr_string(xdrs, &objp->path, AEFSCTRL_MAXPATHLEN))
		return (FALSE);
	if (!xdr_string(xdrs, &objp->key, 1024))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->flags))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->stor_uid))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->stor_gid))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->stor_mode))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->fs_uid))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->fs_gid))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_addfsstat(register XDR *xdrs, addfsstat *objp)
{

	register long *buf;

	if (!xdr_enum(xdrs, (enum_t *)objp))
		return (FALSE);
	return (TRUE);
}

bool_t
xdr_addfsres(register XDR *xdrs, addfsres *objp)
{

	register long *buf;

	if (!xdr_addfsstat(xdrs, &objp->stat))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->cr))
		return (FALSE);
	return (TRUE);
}
