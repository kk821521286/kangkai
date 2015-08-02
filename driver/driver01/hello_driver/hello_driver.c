#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> //module_init module_exit 
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

//
static int major = 249 ;
static int minor = 0 ;

static int hello_open (struct inode*inode, struct file* file) 
{
	printk (KERN_ALERT"char device open!\r\n") ;
	return  0;
}

static int hello_release (struct inode *inode, struct file* file)
{
	printk (KERN_ALERT"char device close!\r\n") ;
	return  0;
}

static struct file_operations hello_ops = {
	.open = hello_open ,
	.release = hello_release ,
} ;

static struct cdev hello_cdev ;
//int init_module (void) 
static int hello_driver_init (void)
{
	int ret ; 
	dev_t devno = MKDEV (major, minor) ;
	//
	ret = register_chrdev_region (devno, 1, "hello_num") ;
	if (ret) {
		printk (KERN_ALERT"char device num fail!\r\n") ;
		goto err ;
	}

	cdev_init (&hello_cdev, &hello_ops) ; // hello_ops -> cdev->ops
	cdev_add (&hello_cdev, devno, 1) ;
	printk(KERN_ALERT"init module\r\n") ;
	return 0 ;

err:
	return ret ;
}

//rmmod modulenae
static void hello_driver_exit (void) 
{
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
	cdev_del (&hello_cdev) ;
	printk(KERN_ALERT"cleanup module\r\n") ;
}

MODULE_AUTHOR("GUO") ;
MODULE_DESCRIPTION("THIS IS SIMAPLE HELLO MODULE") ;
//
module_init (hello_driver_init) ;
module_exit (hello_driver_exit) ;


