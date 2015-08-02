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

void task_let_handler (unsigned long data) 
{
	unsigned int cpsr_val ;
	printk (KERN_ALERT"TASK LET CALL = %d\n", data) ;
	asm volatile("mrs %0, cpsr\n\t"
			:"=r"(cpsr_val)
			:
			:"memory") ;	

    printk (KERN_ALERT"CPSR = 0X%x\n", cpsr_val) ;	
}
DECLARE_TASKLET(mytask_let0, task_let_handler, 0) ;

irqreturn_t key_int_handler (int num, void* devid) 
{

	unsigned int cpsr_val ;
	// IRQ DISABLED
	printk (KERN_ALERT"IRQ CALL = %d\n", num) ;
	switch (num) {
	case 33:   //key 1
		*PEND = (1 << 1) ;
		break ;
	case 34:   //key 2
		*PEND = (1 << 2) ;
		break ;
	default:
		break ;
	}

	asm volatile("mrs %0, cpsr\n\t"
			:"=r"(cpsr_val)
			:
			:"memory") ;	
	
    printk (KERN_ALERT"CPSR = 0X%x\n", cpsr_val) ;	
	tasklet_schedule (&mytask_let0) ;
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
	GPH0.GPH0CON = (GPH0.GPH0CON & (~(0xf<<8))) | (2 << 8) ;  //GPH0_2 EXT2

	*WKUP_INT0_7_MASK = *WKUP_INT0_7_MASK &  (~(1<<1)) ;
	*WKUP_INT0_7_MASK = *WKUP_INT0_7_MASK &  (~(1<<2)) ;
	ret = request_irq (32 + 1, key_int_handler, IRQF_DISABLED | IRQF_TRIGGER_FALLING, "KEY1_IRQ", NULL) ;
	if (ret) {
		printk (KERN_ALERT"REQUEST IRQ ERROR\n") ;
		goto err2 ;
	}

	ret = request_irq (32 + 2, key_int_handler, IRQF_DISABLED | IRQF_TRIGGER_FALLING, "KEY2_IRQ", NULL) ;
	if (ret) {
		printk (KERN_ALERT"REQUEST IRQ ERROR\n") ;
		goto err3 ;
	}

	printk (KERN_ALERT"keyint1 DRIVER INSMOD DONE!\n") ; 
	return 0 ;

err3:
	free_irq (32 + 1, NULL) ;
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
	disable_irq (32 + 2) ;
	free_irq (32 + 2, NULL) ;
	free_irq (32 + 1, NULL) ;
	
	iounmap (gph0base) ;
	iounmap (WKUP_INT0_7_MASK) ;
	iounmap (PEND) ;
	
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


