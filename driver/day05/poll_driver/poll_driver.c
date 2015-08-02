#include <linux/init.h>     // __init __exit 
#include <linux/kernel.h>	//printk 
#include <linux/module.h>	//module_init module_exit 
#include <linux/cdev.h>		//struct cdev
#include <linux/fs.h>		//struct file_operations 
#include <linux/errno.h>	
#include <linux/kdev_t.h>	//MKDEV 
#include <asm/io.h>			
#include <asm/uaccess.h>	
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/poll.h>

static int major = 249 ;
static int minor = 0 ;
static struct cdev poll_drv_dev ;
static bool rflag = false ;

static ssize_t poll_drv_read (struct file* filp, char *buf, size_t len, loff_t *fpos)
{
	return 0;
}


wait_queue_head_t poll_header ;
static ssize_t poll_drv_write(struct file* filp, const char *buf, size_t len, loff_t* fpos)
{
	printk (KERN_ALERT "POLL WRITE CALL!") ;
	rflag = true ;

	wake_up_interruptible (&poll_header) ;
	return 0;
}

static unsigned int poll_drv_poll(struct file *filp, struct poll_table_struct *wait)
{
	unsigned int mask = 0 ;
	
	// poll_wait () ;
	poll_wait (filp, &poll_header, wait) ;
	if (rflag) {
		rflag = false ;
		mask |= POLLIN | POLLRDNORM ;
		printk (KERN_ALERT"POLL IN CALL!\n") ;
	}
	return mask ;
}

static struct file_operations poll_drv_ops = {
	.read = poll_drv_read ,
	.write = poll_drv_write,
	.poll = poll_drv_poll ,
} ;

//__init
__init int poll_drv_init (void)
{
	int ret ;
	dev_t devno = MKDEV (major, minor) ;
	//cat /proc/devices 
	ret = register_chrdev_region (devno, 1, "poll_drv_dev") ;
	if (ret) {
		printk (KERN_ERR"REGISTER DEV NUMBER ERROR!\n");
		goto err ;
	}

	cdev_init (&poll_drv_dev, &poll_drv_ops) ;
	ret = cdev_add (&poll_drv_dev, devno, 1) ;
	if (ret) {
		printk (KERN_ERR"CDEV ADD ERROR!\n") ;
		goto err1 ;
	}

	init_waitqueue_head (&poll_header) ;
	printk (KERN_INFO "poll_drv module init suc!\n") ;
	return 0 ;

err1: 
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
err :
	return ret ;
}

//__exit 
__exit void poll_drv_exit (void)
{
	cdev_del (&poll_drv_dev) ;
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
	printk (KERN_INFO "poll_drv module exit suc\n") ;
}

MODULE_LICENSE ("GPL") ;
module_init (poll_drv_init);
module_exit (poll_drv_exit);
