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

static int major = 249 ;
static int minor = 0 ;
static struct cdev sleep_task_dev ;

wait_queue_head_t rheader ;
bool rflag = false ;
static ssize_t sleep_task_read (struct file* filp, char *buf, size_t len, loff_t *fpos)
{
	if (filp->f_flags & O_NONBLOCK) 
		return 0 ;
	// io mode : block 
	// shou gong shui mian 手工睡眠
    // step 1: task_struct -> remove task_struct system rqueue 	
	// step 2: schedule () ;
	// task_struct -> state = TASK_RUNNING -> TASK_INTERRUPTIBLE
	//TASK_UNINTERRUPTIBEL
	//schedule () ;
#if 0
	task = get_current () ;
	task->state = TASK_INTERRUPTIBLE ;
	//get_current()->state = TASK_INTERRUPTIBLE ;
	schedule () ; // pause 
#endif
	//condition = wake flag 
	//	wait_event_interruptible (rheader, rflag) ; 
	wait_event(rheader, rflag) ;
	rflag = false ;
	printk (KERN_ALERT"READ OPS CALL \n") ;
	return 0;
}

static ssize_t sleep_task_write(struct file* filp, const char *buf, size_t len, loff_t* fpos)
{
	rflag = true ;
	wake_up (&rheader) ;
	//wake_up_interruptible (&rheader) ;
	return 0;
}

static struct file_operations sleep_task_ops = {
	.read = sleep_task_read ,
	.write = sleep_task_write,

} ;

//__init
__init int sleep_task_init (void)
{
	int ret ;
	dev_t devno = MKDEV (major, minor) ;
	//cat /proc/devices 
	ret = register_chrdev_region (devno, 1, "sleep_task_dev") ;
	if (ret) {
		printk (KERN_ERR"REGISTER DEV NUMBER ERROR!\n");
		goto err ;
	}

	cdev_init (&sleep_task_dev, &sleep_task_ops) ;
	ret = cdev_add (&sleep_task_dev, devno, 1) ;
	if (ret) {
		printk (KERN_ERR"CDEV ADD ERROR!\n") ;
		goto err1 ;
	}
	
	init_waitqueue_head (&rheader) ;
	printk (KERN_INFO "sleep_task module init suc!\n") ;
	return 0 ;

err1: 
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
err :
	return ret ;
}

//__exit 
__exit void sleep_task_exit (void)
{
	cdev_del (&sleep_task_dev) ;
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
	printk (KERN_INFO "sleep_task module exit suc\n") ;
}

MODULE_LICENSE ("GPL") ;
module_init (sleep_task_init);
module_exit (sleep_task_exit);
