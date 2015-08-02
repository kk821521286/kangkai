obj-m := plat_drv.o
obj-m += plat_dev.o

KERDIR=/lib/modules/$(shell uname -r)/build/
PWD=$(shell pwd)

module:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules

clean:
	@rm rf *.o *.ko *.cmd *mod* .tmp* *.order Mod*
