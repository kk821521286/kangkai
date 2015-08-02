obj-m := wait_queue.o

#KERDIR=/home/linux/linux-2.6.35-farsight/
KERDIR=/lib/modules/$(shell uname -r)/build/
PWD=$(shell pwd) 

module:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules

clean:
	rm -rf *.mod* *.order *.o *.ko *.cmd* *.cmd .tmp*
