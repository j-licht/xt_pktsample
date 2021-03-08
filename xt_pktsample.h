#ifndef _LINUX_NETFILTER_XT_PKTSAMPLE_H
#define _LINUX_NETFILTER_XT_PKTSAMPLE_H 1

#include <linux/types.h>

struct xt_pktsample_priv;

struct xt_pktsampleinfo {
	__u32 sample_interval;

	struct xt_pktsample_priv* master;
};
#endif
