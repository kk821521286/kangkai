#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> //module_init module_exit 
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <asm/io.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>

static int major = 248 ;
static int minor = 0 ;

unsigned int *WKUP_INT0_7_MASK ;
unsigned int *PEND ;
void *gph0base ;
/* GPH0 */
typedef struct {
	unsigned int GPH0CON;
	unsigned int GPH0DAT;
	unsigned int GPH0PULL;
	unsigned int GPH0DRV;
}gph0;
#define GPH0 (* (volatile gph0 *)gph0base)


static struct file_operations keyint1_ops = {

};

irqreturn_t key1_int_handler (int num, void* devid) 
{
	printk (KERN_ALERT"IRQ CALL = %d\n", num) ;
	*PEND = (1 << 1) ;

	return IRQ_HANDLED ;
}

static struct cdev keyint1_cdev ;
__init static int keyint1_driver_init (void)
{
	int ret ; 
	dev_t devno = MKDEV (major, minor) ;
	
	ret = register_chrdev_region (devno, 1, "keyint1_device") ;
	if (ret) {
		printk (KERN_ALERT"char device num fail!\r\n") ;
		goto err ;
	}
	
	cdev_init (&keyint1_cdev, &keyint1_ops) ;
	ret = cdev_add (&keyint1_cdev, devno, 1) ;
	
	if (ret) {
		printk (KERN_ALERT"CDEV ADD ERROR") ;
		goto err1;
	}

	gph0base = ioremap (0xE0300C00, sizeof (gph0)) ;
	WKUP_INT0_7_MASK  = ioremap(0XE0300F00, 4) ;
	PEND = ioremap (0XE0300F40, 4) ;
	
	GPH0.GPH0CON = (GPH0.GPH0CON & (~(0xf<<4))) | (2 << 4) ;
	*WKUP_INT0_7_MASK = *WKUP_INT0_7_MASK &  (~(1<<1)) ;
	ret = request_irq (32 + 1, key1_int_handler, IRQF_DISABLED | IRQF_TRIGGER_FALLING, "KEY1_IRQ", NULL) ;
	if (ret) {
		printk (KERN_ALERT"REQUEST IRQ ERROR\n") ;
		goto err2 ;
	}

	//enable_irq (32 + 1) ;
	printk (KERN_ALERT"keyint1 DRIVER INSMOD DONE!\n") ; 
	return 0 ;
err2:
	cdev_del (&keyint1_cdev) ;
err1:
	unregister_chrdev_region (devno, 1);
err:
	return ret ;
}

__exit static void keyint1_driver_exit (void) 
{
	disable_irq (32 + 1) ;
	free_irq (32 + 1, NULL) ;
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
	cdev_del (&keyint1_cdev) ;
	printk(KERN_ALERT"keyint1 DRIVER RMMOD DONE\r\n") ;
}

MODULE_LICENSE ("GPL") ;
MODULE_AUTHOR("GUO") ;
MODULE_DESCRIPTION("THIS IS SIMAPLE keyint1 MODULE") ;
//
module_init (keyint1_driver_init) ;
module_exit (keyint1_driver_exit) ;


