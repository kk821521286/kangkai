#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/clk.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/wait.h>

typedef struct {
	unsigned int ADCCON;
	unsigned int ADCTSC;
	unsigned int ADCDLY;
	unsigned int ADCDAT0;				
	unsigned int ADCDAT1;
	unsigned int ADCUPDN;
	unsigned int ADCCLRINT;
	unsigned int ADCMUX;
	unsigned int ADCPNDCLR;
}adc;

static void *adc_reg_base = NULL; 
#define ADC	(* (volatile adc * )adc_reg_base)
static struct clk *clk_adc; 

//irq handle
static unsigned int adv = 0;
static bool adc_int_flag = false;
wait_queue_head_t wait_adc_head ;
irqreturn_t adc_int_handler (int num, void *dev_id)
{
	adv = ADC.ADCDAT0 & 0xfff ;

	adc_int_flag = true;
	printk (KERN_ALERT"ADC IRQ_HANDLED = %d\n", num);
	wake_up_interruptible (&wait_adc_head);

	//CLEAR FLAG
	ADC.ADCCLRINT = 1; 
	return IRQ_HANDLED;	
}

static ssize_t adc_int_read(struct file *filp, char *buffer, size_t len, loff_t* fpos)
{
	//start adc_converion 
	ADC.ADCCON = ADC.ADCCON | (1 << 0) ;
#if 0
	while (false == adc_int_flag) {
	
		sleep () ;
	}
	adc_int_flag = false;
#endif
	//wait_event 
	//wait_event_timeout
	//wait_event_interruptible 
	//wait_event_interruptible_timeout
	
	//jiffies  HZ 
	//wait_event_interruptible_timeout (wait_adc_head, adc_int_flag, HZ / 2);
	wait_event_interruptible (wait_adc_head, adc_int_flag);
	adc_int_flag = false;
	if (copy_to_user ((void *)buffer, (void *)&adv, sizeof (adv)))
		return -ENOMEM;

	return len;
}

static void adc_hardware_init (void)
{
	ADC.ADCMUX = 0 ;
	//12BIT      预分频              分频值
	//读转换模式
	ADC.ADCCON = (1 << 16) | (1 << 14) | (65 << 6) | (0	<< 1);
}


static struct file_operations adc_int_ops = {
	.read = adc_int_read, 
};

static int major = 249, minor = 0;
static struct cdev adc_int_dev;
__init static int adc_int_init (void)
{
	int ret = 0;
	dev_t devno = MKDEV(major, minor);
	ret = register_chrdev_region (devno, 1, "adc_int_devno");
	if (ret) {
		goto err1;
	} 

	cdev_init (&adc_int_dev, &adc_int_ops);
	ret = cdev_add (&adc_int_dev, devno, 1) ;
	if (ret) {
		goto err;
	}

	adc_reg_base = ioremap (0xF3000000, sizeof (adc));

	clk_adc = clk_get (NULL, "adc");
	clk_enable (clk_adc);
#if 0
	CLK_GATE_D1_5 = ioremap (0xE0100534, sizeof (int));
	*CLK_GATE_D1_5 = *CLK_GATE_D1_5 | (1 << 7) ;
#endif
	adc_hardware_init ();
	
	//init wait_queue_head 
	init_waitqueue_head (&wait_adc_head);

	ret = request_irq (32 + 87, adc_int_handler, IRQF_DISABLED, "adc_irq", NULL);
	if (ret) {
		goto err2 ;
	}
	printk (KERN_ALERT"adc_int_init \n");
	return 0;

err2:
	cdev_del (&adc_int_dev);
	clk_disable (clk_adc);
err:
	unregister_chrdev_region (devno, 1);
err1:
	return ret;
}

__exit static void adc_int_exit (void)
{
	disable_irq (32 + 87);
	free_irq (32 + 87, NULL);
	clk_disable (clk_adc) ;
	iounmap (adc_reg_base);
	unregister_chrdev_region (MKDEV(major, minor), 1);
	cdev_del (&adc_int_dev);
	printk (KERN_ALERT"adc_int_exit\n");
} 

MODULE_LICENSE("GPL");
module_init (adc_int_init);
module_exit (adc_int_exit);
