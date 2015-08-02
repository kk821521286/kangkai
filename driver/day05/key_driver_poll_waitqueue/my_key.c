#include <linux/init.h>  //__init __exit
#include <linux/kernel.h>  //printk
#include <linux/module.h>  //module_init module_exit
#include <linux/cdev.h> //  struct cdev 
#include <linux/fs.h> //struct file_operations
#include <linux/errno.h> 
#include <linux/kdev_t.h> //MKDEV


static int major = 249;
static int minor = 0 ;
static struct cdev mykey_dev;

void mykey_xiaodou ()
{

}

irqreturn_t mykey_irq_handler ()
{
	
}

static int mykey_open (struct inode* inode,struct file* filp)
{
	return 0;
}

static ssize_t mykey_release (struct inode* inode,struct file*filp)
{
	return 0;
}
static ssize_t mykey_read (struct file* filp,char *buf,size_t len,loff_t *fpos)
{
	return 0;
}

static unsigned int mykey_poll(struct file *filp,struct poll_table_struct *wait)
{
	unsigned int mask = 0;
	return mask;
}
static struct file_operations mykey_ops = {
	.open = mykey_open,
	.release = mykey_release,
	.read = mykey_read,
	.poll = mykey_poll,
};


__init int mykey_init (void)
{
	int ret ;
	dev_t devno = MKDEV (major,minor);
	ret = register_chrdev_region (devno,1,"mykey_dev");
	if (ret) {
		printk (KERN_ERR "REGISTER DEV NUMBER ERROR!\n");
		goto err;
	}
	cdev_init (&mykey_dev,&mykey_ops);
	ret = cdev_add (&mykey_dev,devno,1);
	if (ret) {
		printk (KERN_ERR"CDEV ADD ERROR!\n");
		goto err1;
	}

	printk (KERN_INFO"mykey module init suc !\n");
	return 0;
err1:
	unregister_chrdev_region (MKDEV(major,minor),1);
err:
	return ret;
}

__exit void mykey_exit (void)
{
	cdev_del (&mykey_dev);
	unregister_chrdev_region (MKDEV(major,minor),1);
	printk (KERN_INFO"mykey module  exit suc!\n");
}

MODULE_LICENSE ("GPL");
module_init (mykey_init);
module_exit (mykey_exit);


