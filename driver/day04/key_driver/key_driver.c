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

static int major = 249 ;
static int minor = 0 ;
static struct cdev key_dev ;
unsigned int *GPH0DAT ;
static char key_val = 0 ;
static struct work_struct key_work ;
static struct timer_list timer0 ;

void soft_long_delay (int s) 
{
	int delay = jiffies + s * HZ ;

#if 0
	while (jiffies < delay );
	while (time_after (delay, jiffies)) ;
	while (delay > jiffies) ;
#endif
}

// jincheng 
void key_xiaodou (void) 
{
	//mdelay (20) ;
	msleep (100) ;
	soft_long_delay (10) ;
	
	key_val = *GPH0DAT ;
	if (key_val & (1 << 1)) {
		// KEY UP
		key_val = 0;
	}
	else {
		//KEY DOWN 
		key_val = 'a' ;
		printk (KERN_ALERT "KEY DOWN = %d, jiffies = %d\n", key_val, jiffies) ;
	}

	enable_irq (32 + 1) ;
}

irqreturn_t key_irq_handler (int num, void *dev_id)
{
	unsigned int key_val ;
	printk (KERN_ALERT "IRQ CALL = %d jiffies = %d\n", num, jiffies) ;
	// delay_20ms 
	// read key data == 0 ; key down else 

	disable_irq_nosync (32 + 1) ;

	schedule_work (&key_work) ;
#if 0
	timer0.expires = jiffies + HZ / 10 ;
	add_timer (&timer0) ;
#endif	
	return IRQ_HANDLED;
}

static int key_open (struct inode* inode, struct file* filp) 
{
	int ret ;
	//key1										IRQF_DISABLED | IRQF_SHARED 
	ret = request_irq (32 + 1, key_irq_handler, IRQF_DISABLED | IRQF_TRIGGER_FALLING, "KEY_INT", NULL) ;
	if (ret)
		return ret ;
	return 0 ;
}

static int key_release (struct inode* inode, struct file* filp)
{
	disable_irq (32 + 1) ;
	free_irq (32 + 1, NULL);
	return 0 ;
}

static ssize_t key_read (struct file* filp, char *buf, size_t len, loff_t *fpos)
{
	printk (KERN_ALERT "KEY READ CALL!\n") ;
	return len ;
}

static struct file_operations key_ops = {
	.open = key_open ,
	.release = key_release ,
	.read = key_read ,
} ;

//__init
__init int key_init (void)
{
	int ret ;
	dev_t devno = MKDEV (major, minor) ;
	//cat /proc/devices 
	ret = register_chrdev_region (devno, 1, "key_dev") ;
	if (ret) {
		printk (KERN_ERR"REGISTER DEV NUMBER ERROR!\n");
		goto err ;
	}

	cdev_init (&key_dev, &key_ops) ;
	ret = cdev_add (&key_dev, devno, 1) ;
	if (ret) {
		printk (KERN_ERR"CDEV ADD ERROR!\n") ;
		goto err1 ;
	}

	INIT_WORK (&key_work, key_xiaodou) ;
	GPH0DAT = ioremap (0xE0300C04, 4) ;

	init_timer (&timer0) ;
	setup_timer (&timer0, key_xiaodou, 0) ;
	printk (KERN_INFO "key module init suc!\n") ;
	return 0 ;

err1: 
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
err :
	return ret ;
}

//__exit 
__exit void key_exit (void)
{
	iounmap (GPH0DAT) ;
	cdev_del (&key_dev) ;
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
	printk (KERN_INFO "key module exit suc\n") ;
}

MODULE_LICENSE ("GPL") ;
module_init (key_init);
module_exit (key_exit);
