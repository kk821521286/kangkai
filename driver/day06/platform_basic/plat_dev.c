#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/platform_device.h>

void test_release (struct device* dev)
{

}

struct platform_device test_dev = {
	.name = "test_haha" ,
	.id = -1 ,
	.dev = {
		.release = test_release ,
	}
};

int platform_test_init (void)
{
	platform_device_register (&test_dev);
	
	return 0;
}

void platform_test_exit (void)
{
	platform_device_unregister (&test_dev);
}

MODULE_LICENSE ("GPL");
module_init (platform_test_init) ;
module_exit (platform_test_exit) ;



