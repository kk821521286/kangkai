obj-m := hello_module.o

PWD=$(shell pwd) 
KERDIR=/lib/modules/$(shell uname -r)/build/
#KERDIR=/home/linux/linux-2.6.35-farsight/

module:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules

clean:
	rm *.o *.ko *.mod.c *.symvers *.order

