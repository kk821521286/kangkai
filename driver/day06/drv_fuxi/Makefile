obj-m := light_drv.o

KERDIR=/lib/modules/$(shell uname -r)/build/
PWD=$(shell pwd)

module:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules

