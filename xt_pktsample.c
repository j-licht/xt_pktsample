#include <linux/netfilter/x_tables.h>
#include "xt_pktsample.h"
#include <linux/spinlock.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jonas Licht <jonas.licht@tu-ilmenau.de");
MODULE_DESCRIPTION("Xtables: packet sampling");
MODULE_ALIAS("ipt_pktsample");
MODULE_ALIAS("ip6t_pktsample");

struct xt_pktsample_priv {
	spinlock_t lock;
	uint32_t counter;
};

static bool pktsample_mt(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_pktsampleinfo *p = par->matchinfo;
	struct xt_pktsample_priv *priv = p->master;

	spin_lock_bh(&priv->lock);
	priv->counter++;
	if (priv->counter >= p->sample_interval) {
		priv->counter = 0;
		spin_unlock_bh(&priv->lock);
		return true;
	}

	spin_unlock_bh(&priv->lock);
	return false;
}

static int pktsample_mt_check(const struct xt_mtchk_param *par)
{
	struct xt_pktsampleinfo *r = par->matchinfo;
	struct xt_pktsample_priv *priv;

	priv = kmalloc(sizeof(*priv), GFP_KERNEL);
	if (priv == NULL)
		return -ENOMEM;

	r->master = priv;
	spin_lock_init(&priv->lock);
	priv->counter = 0;

	pr_info("xt_pktsample: sample_interval is %u", r->sample_interval);

	return 0;
}

static void pktsample_mt_destroy(const struct xt_mtdtor_param *par)
{
	const struct xt_pktsampleinfo *p = par->matchinfo;

	kfree(p->master);
}

static struct xt_match pktsample_mt_reg __read_mostly = {
	.name = "pktsample",
	.revision = 0,
	.family = NFPROTO_UNSPEC,
	.match = pktsample_mt,
	.matchsize = sizeof(struct xt_pktsampleinfo),
	.usersize = offsetof(struct xt_pktsampleinfo, master),
	.me = THIS_MODULE,
	.checkentry = pktsample_mt_check,
	.destroy = pktsample_mt_destroy
};

static int __init pktsample_mt_init(void)
{
	return xt_register_match(&pktsample_mt_reg);
}

static void __exit pktsample_exit(void)
{
	xt_unregister_match(&pktsample_mt_reg);
}

module_init(pktsample_mt_init);
module_exit(pktsample_exit);

