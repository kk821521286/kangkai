#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> //module_init module_exit 
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
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

static char p [128] = "hello read!" ;
static ssize_t hello_read(struct file *file, char __user *buf, size_t len, loff_t *lof)
{
	if (copy_to_user (buf, p, len)) 
		return -1 ;
	//copy_to_user 
	//copy_from_user 
	//
	return len;
}

static ssize_t hello_write(struct file *file, const char __user *buf, size_t len, loff_t *lof)
{
	if (copy_from_user (p, buf, len)) 
		return -1 ;

	return len ;
}

#define CMD_SET_BAUD _IO ('k', 0)
#define CMD_SET_DATA _IO ('k', 1)
#define CMD_SET_ODD	 _IO ('k', 2) 
long hello_ioctl (struct file *file, unsigned int cmd, unsigned long arg)
{
	printk (KERN_ALERT"IOCTL CALL") ;
	switch (cmd) {
	case CMD_SET_BAUD:
		printk ("set baud = %d\n", arg) ;
		break; 
	case CMD_SET_DATA:
		printk ("set data = %d\n", arg) ;
		break ;
	case CMD_SET_ODD: 
		printk ("set odd = %d\n", arg) ;
		break ;
	}
	return 0 ;
}

static struct file_operations hello_ops = {
	.open = hello_open ,
	.release = hello_release ,
	.read = hello_read ,
	.write = hello_write ,
	.unlocked_ioctl = hello_ioctl ,
} ;

static struct cdev hello_cdev ;
__init static int hello_driver_init (void)
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
	return 0 ;

err:
	return ret ;
}

__exit static void hello_driver_exit (void) 
{
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
	cdev_del (&hello_cdev) ;
	printk(KERN_ALERT"cleanup module\r\n") ;
}

MODULE_LICENSE ("GPL") ;
MODULE_AUTHOR("GUO") ;
MODULE_DESCRIPTION("THIS IS SIMAPLE HELLO MODULE") ;
//
module_init (hello_driver_init) ;
module_exit (hello_driver_exit) ;


