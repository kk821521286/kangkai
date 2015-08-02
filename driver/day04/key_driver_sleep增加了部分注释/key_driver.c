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

static int major = 249 ;
static int minor = 0 ;
static struct cdev mukey_dev ;
unsigned int *GPH0DAT ;
static char mukey_val = 0 ;
static struct work_struct mukey_work ;
static struct task_struct *task_sleep = NULL;

// fork  
void mukey_xiaodou (struct work_struct *work) 
{
	//mdelay (20) ;
	msleep (100) ;
	
	mukey_val = *GPH0DAT ;
	if (mukey_val & (1 << 1)) {
		// mukey UP
		mukey_val = 0;
	} else {
		//mukey DOWN 
		mukey_val = 'a' ;
		
		//唤醒被睡眠的进程 
		if (task_sleep != NULL)
			wake_up_process (task_sleep) ;
		printk (KERN_ALERT "mukey DOWN = %ld, jiffies = %ld\n", mukey_val, jiffies) ;
	}

	enable_irq (32 + 1) ;
}


irqreturn_t mukey_irq_handler (int num, void *dev_id)
{
	printk (KERN_ALERT "IRQ CALL = %d jiffies = %d\n", num, jiffies) ;
	disable_irq_nosync (32 + 1) ;
	schedule_work (&mukey_work) ;
	
	return IRQ_HANDLED;
}

static int mukey_open (struct inode* inode, struct file* filp) 
{
	int ret ;
	//mukey1										IRQF_DISABLED | IRQF_SHARED 
	ret = request_irq (32 + 1, mukey_irq_handler, IRQF_DISABLED | IRQF_TRIGGER_FALLING, "mukey_INT", NULL) ;
	if (ret)
		return ret ;
	return 0 ;
}

static int mukey_release (struct inode* inode, struct file* filp)
{
	disable_irq (32 + 1) ;
	free_irq (32 + 1, NULL);
	return 0 ;
}

//使用阻塞与非阻塞方式实现IO请求
static ssize_t mukey_read (struct file* filp, char *buf, size_t len, loff_t *fpos)
{
	// 当应用层程序需要实现阻塞IO或者非阻塞IO的时候， 一般把标志位记录在struct file -> state 中， 
	// 我们可以通过判断 //struct file -> state 值判断当前是阻塞还是非阻塞操作
	// 当程序设置了非阻塞， 我们可以判断f_flags中的O_NONBLOCK位
	
	// 若程序设置程序为非阻塞，则当前设备是否有数据我们都立即返回
	if (filp->f_flags & O_NONBLOCK) {
		// nonblock 
		if (len == sizeof (mukey_val)) {
			if (mukey_val) {
				if (0 == copy_to_user(buf, &mukey_val, len)) {
					mukey_val = 0 ;
					return len ;
				}
			} else {
				return 0 ;
			}
		} else {
			return -EINVAL ;
		}
	}
	
	// 若程序设置程序为阻塞，若没有按键按下， 则程序进入睡眠状态， 睡眠即程序从系统的运行队列中移除，
	if (mukey_val) {
		if (0 == copy_to_user(buf, &mukey_val, len)) {
			mukey_val = 0 ;
			return len ;
		}
	} else {
		// 获得当前进程的task_struct, 谁调用read获得是谁的task_struct
		task_sleep = get_current ();
		//改变当前进程的状态
		get_current()->state = TASK_INTERRUPTIBLE;
		//内核提供的调度器函数， 可将本进程从内核的运行队列中移除， 并进入睡眠状态， 可使用ps aus查看进程状态
		//程序在schedule处暂停， 被唤醒后继续执行		
		schedule ();  //wake_up_process ();
		if (0 == copy_to_user(buf, &mukey_val, len)) {
			task_sleep = NULL ;
			mukey_val = 0 ;
			return len ;
		}
	}
	
	return 0;
}

static struct file_operations mukey_ops = {
	.open = mukey_open ,
	.release = mukey_release ,
	.read = mukey_read ,
} ;

//__init
__init int mukey_init (void)
{
	int ret ;
	dev_t devno = MKDEV (major, minor) ;
	//cat /proc/devices 
	ret = register_chrdev_region (devno, 1, "mukey_dev") ;
	if (ret) {
		printk (KERN_ERR"REGISTER DEV NUMBER ERROR!\n");
		goto err ;
	}

	cdev_init (&mukey_dev, &mukey_ops) ;
	ret = cdev_add (&mukey_dev, devno, 1) ;
	if (ret) {
		printk (KERN_ERR"CDEV ADD ERROR!\n") ;
		goto err1 ;
	}

	INIT_WORK (&mukey_work, mukey_xiaodou) ;
	GPH0DAT = ioremap (0xE0300C04, 4) ;

	printk (KERN_INFO "mukey module init suc!\n") ;
	return 0 ;

err1: 
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
err :
	return ret ;
}

//__exit 
__exit void mukey_exit (void)
{
	iounmap (GPH0DAT) ;
	cdev_del (&mukey_dev) ;
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
	printk (KERN_INFO "mukey module exit suc\n") ;
}

MODULE_LICENSE ("GPL") ;
module_init (mukey_init);
module_exit (mukey_exit);
