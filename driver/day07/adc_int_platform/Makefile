obj-m:= adc_int_platform.o

KERDIR=/home/linux/linux-2.6.35-farsight
PWD=$(shell pwd)

module:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules


clean:
	@rm -rf *.ko *.o *.cmd *cmd* *mod* *.order Mo* .tmp*
