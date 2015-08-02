obj-m := key_driver.o

KERDIR=/home/linux/linux-2.6.35-farsight/
#KERDIR=/lib/modules/$(shell uname -r)/build/
PWD=$(shell pwd) 

module:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules
	arm-cortex_a8-linux-gnueabi-gcc -o test_arm_server test_arm_server.c
	gcc -o test_pc_client test_pc_client.c

clean:
	-rm -rf *.o *.ko *.cmd* *.cmd *order Mod* .tmp* 
