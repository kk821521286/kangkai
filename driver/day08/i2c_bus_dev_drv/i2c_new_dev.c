#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/i2c.h>


/*
 * #define I2C_BOARD_INFO(dev_type, dev_addr) \
 * 272     .type = dev_type, .addr = (dev_addr)   
 */
static struct i2c_board_info i2c_new_info = {
	I2C_BOARD_INFO ("lm75", 0x48);	
};
static struct i2c_client *new_client ;
//struct i2c_client *
// i2c_new_device(struct i2 c_adapter *adap, struct i2c_board_info const
// *info)
__init int i2c_new_device_init (void)
{
	struct i2c_adapter *adap = i2c_get_adapter(0);
	new_client = i2c_new_device (adap, &i2c_new_info) ;
	return 0;
}

__exit void i2c_new_device_exit (void)
{	
	i2c_unregister_device (new_client);
}

MODULE_LICENSE ("GPL");
module_init (i2c_new_device_init);
module_exit (i2c_new_device_exit);
