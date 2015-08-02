obj-m := key_int1.o

PWD=$(shell pwd) 
#KERDIR=/lib/modules/$(shell uname -r)/build/
KERDIR=/home/linux/linux-2.6.35-farsight/

module:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules

clean:
	rm -rf .tmp* .*cmd *.o *.ko *.mod.c *.symvers *.order

