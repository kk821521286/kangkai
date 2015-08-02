obj-m:= adc_poll_driver.o

KERDIR=/home/linux/linux-2.6.35-farsight
PWD=$(shell pwd)

module:
	$(MAKE) -C $(KERDIR) M=$(PWD) modules

