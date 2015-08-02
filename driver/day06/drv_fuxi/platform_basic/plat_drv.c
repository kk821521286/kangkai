#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/platform_device.h>

int test_probe (struct platform_device *dev)
{
	printk ("============== probe call!\n");
	return 0;
}

int test_remove (struct platform_device *dev)
{
	printk ("==============remove call!\n");
	return 0;
}

struct platform_driver test_drv = {
	.probe  = test_probe, 
	.remove = test_remove,
	.driver = {
		.name = "test_haha",
	}
} ;

int platform_test_init (void)
{
	platform_driver_register (&test_drv);
	return 0;
}

void platform_test_exit (void)
{
	platform_driver_unregister (&test_drv);
}

MODULE_LICENSE ("GPL");
module_init (platform_test_init) ;
module_exit (platform_test_exit) ;



