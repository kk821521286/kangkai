obj-m := key_driver.o

KERDIR=/home/linux/linux-2.6.35-farsight/
#KERDIR=/lib/modules/$(shell uname -r)/build/
PWD=$(shell pwd) 

module:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules
