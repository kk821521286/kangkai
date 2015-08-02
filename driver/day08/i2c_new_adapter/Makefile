obj-m := i2c_new_virtual_adapter.o

PWD=$(shell pwd) 

# 
KERNEL=/lib/modules/$(shell uname -r)/build/

module:
	$(MAKE) -C $(KERNEL) M=$(PWD) modules
