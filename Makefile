KVER   ?= $(shell uname -r)
KDIR   ?= /lib/modules/$(KVER)/build/
obj-m   = xt_pktsample.o
XFLAGS ?= $(shell pkg-config xtables --cflags 2>/dev/null)

.PHONY: all clean
all: xt_pksample.ko libxt_pktsample.so

xt_pksample.ko: xt_pktsample.c xt_pktsample.h
	make -C $(KDIR) M=$(CURDIR) modules
	-sync

libxt_pktsample.so: libxt_pktsample.c xt_pktsample.h
	gcc -O0 -ggdb -Wall -shared -Wunused -fPIC ${XFLAGS} ${CFLAGS} -o $@ $<

clean:
	-make -C $(KDIR) M=$(CURDIR) clean
	-rm -f *.so *_sh.o *.o modules.order
