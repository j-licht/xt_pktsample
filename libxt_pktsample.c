#include "xt_pktsample.h"
#include <stdio.h>
#include <xtables.h>
#include <string.h>

static void pktsample_mt_print(const void *entry, const struct xt_entry_match *match, int numeric)
{
	const struct xt_pktsampleinfo *info = (const void *)match->data;
	printf(" sample interval %u", info->sample_interval);
}

static const struct xt_option_entry pktsample_mt_opts[] = {
	{.name = "sample_interval", .type = XTTYPE_UINT32, .id = 1,
	.flags = XTOPT_PUT, XTOPT_POINTER(struct xt_pktsampleinfo, sample_interval),
	.min = 1, .max = 1000000},
	XTOPT_TABLEEND,
};

static void pktsample_mt_parse(struct xt_option_call *cb)
{
	xtables_option_parse(cb);
}

static void pktsample_mt_help(void)
{
	printf(
			"pktsample match options:\n [!] --sample_interval interval   Sets die sample_interval\n"
			);
}

static void pktsample_save(const void *ip, const struct xt_entry_match *match)
{
	const struct xt_pktsampleinfo *info = (const void *)match->data;

	printf(" --sample_interval %u", info->sample_interval);
}

static struct xtables_match pktsample_mt_reg = {
	.version = XTABLES_VERSION,
	.name = "pktsample",
	.revision = 0,
	.family = NFPROTO_UNSPEC,
	.size = XT_ALIGN(sizeof(struct xt_pktsampleinfo)),
	.userspacesize = offsetof(struct xt_pktsampleinfo, master),
	.help = pktsample_mt_help,
	.x6_parse = pktsample_mt_parse,
	.print = pktsample_mt_print,
	.x6_options = pktsample_mt_opts,
	.save = pktsample_save
};

void _init(void)
{
	xtables_register_match(&pktsample_mt_reg);
}
