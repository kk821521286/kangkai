#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> //module_init module_exit 
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <asm/io.h>
//
static int major = 249 ;
static int minor = 0 ;
#define MINOR_NR 4

static int hello_open (struct inode*inode, struct file* file) 
{
	printk (KERN_ALERT"char device open! =  %d\r\n", iminor (inode)) ;
	return  0;
}

static int hello_release (struct inode *inode, struct file* file)
{
	printk (KERN_ALERT"char device close! = %d\r\n", iminor (inode)) ;
	return  0;
}

#if 0
static ssize_t hello_read(struct file *file, char __user *buf, size_t len, loff_t *lof)
{
	if (copy_to_user (buf, p, len)) 
		return -ENOMEM ;
	
	return len;
}

static ssize_t hello_write(struct file *file, const char __user *buf, size_t len, loff_t *lof)
{
	//if (copy_from_user (p, buf, len)) 
	//	return -1 ;

	return len ;
}
#endif

static struct file_operations hello_ops = {
	.open = hello_open ,
	.release = hello_release ,
#if 0
	.read = hello_read ,
	.write = hello_write ,
#endif
} ;

static struct cdev hello_cdev ;
__init static int hello_driver_init (void)
{
	int ret ; 
	int i = 0 ;
	dev_t devno = MKDEV (major, minor) ;
	//
	ret = register_chrdev_region (devno, MINOR_NR, "hello_device") ;
	if (ret) {
		printk (KERN_ALERT"char device num fail!\r\n") ;
		goto err ;
	}
	
#if 0
	for (i = 0; i < MINOR_NR; i++) {
		cdev_init (&hello_cdev[i], &hello_ops) ; // hello_ops -> cdev->ops
		ret = cdev_add (&hello_cdev[i], devno + i, 1) ;
	}
#endif
	ret = cdev_add (&hello_cdev, devno, 4) ;

#if 0
		if (ret) {
		printk (KERN_ALERT"CDEV ADD ERROR") ;
		goto err1 ;
	}
#endif
	return 0 ;
err1:
	unregister_chrdev_region (devno, 1) ;
err:
	return ret ;
}

__exit static void hello_driver_exit (void) 
{
	int i = 0 ;
	unregister_chrdev_region (MKDEV(major, minor), MINOR_NR) ;
	cdev_del (&hello_cdev) ;
#if 0
	for (i = 0; i < MINOR_NR; i++) {
		cdev_del (&hello_cdev[i]) ;
	}
#endif
	printk(KERN_ALERT"cleanup module\r\n") ;
}

MODULE_LICENSE ("GPL") ;
MODULE_AUTHOR("GUO") ;
MODULE_DESCRIPTION("THIS IS SIMAPLE HELLO MODULE") ;
//
module_init (hello_driver_init) ;
module_exit (hello_driver_exit) ;


