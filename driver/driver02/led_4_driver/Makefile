obj-m := led_4_driver.o

PWD=$(shell pwd) 
#KERDIR=/lib/modules/$(shell uname -r)/build/
KERDIR=/home/kangkai/linux/linux-2.6.35-farsight/

module:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules

clean:
	rm -rf .tmp* .*cmd *.o *.ko *.mod.c *.symvers *.order

