#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/i2c.h>

u32 functionality (struct i2c_adapter *adap)
{
	return 0;
}

int s5pc100_master_xfer(struct i2c_adapter *adap, struct i2c_msg *msgs,int num)
{
	printk (KERN_ALERT"msgs->addr = 0x%02x msgs->flags = %d msgs->len = %d\r\n", msgs->addr, msgs->flags, msgs->len);
	return 0;
}

static struct i2c_algorithm new_algo = {
	.master_xfer = s5pc100_master_xfer,
	.functionality = functionality,
};

static struct i2c_adapter new_adapter0 = {
	.name = "new_adapter0",
	.nr = 0,
	.algo = &new_algo,
};

__init int i2c_new_adapter_init (void)
{
	//i2c_add_adapter() //
	i2c_add_adapter (&new_adapter0);
	return 0;
}

__exit void i2c_new_adapter_exit (void)
{
	//i2c_del_adapter() //
	i2c_del_adapter (&new_adapter0);
}


MODULE_LICENSE ("GPL");
module_init (i2c_new_adapter_init);
module_exit (i2c_new_adapter_exit);
