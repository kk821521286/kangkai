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
#include <linux/platform_device.h>

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
} adc;
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
static int adc_int_probe (struct platform_device *dev)
{
	int ret = 0;
	struct resource *r_temp ;

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


	r_temp = platform_get_resource (dev, IORESOURCE_MEM, 0);
	adc_reg_base = ioremap (r_temp->start, r_temp->end - r_temp->start + 1);


	clk_adc = clk_get (NULL, "adc");
	clk_enable (clk_adc);
#if 0
	CLK_GATE_D1_5 = ioremap (0xE0100534, sizeof (int));
	*CLK_GATE_D1_5 = *CLK_GATE_D1_5 | (1 << 7) ;
#endif
	adc_hardware_init ();
	
	//init wait_queue_head 
	init_waitqueue_head (&wait_adc_head);
	
	//irq
	r_temp = platform_get_resource (dev, IORESOURCE_IRQ, 0);
	ret = request_irq (r_temp->start, adc_int_handler, IRQF_DISABLED, "adc_irq", NULL);
	if (ret) {
		goto err2 ;
	}
	return 0;

err2:
	cdev_del (&adc_int_dev);
	clk_disable (clk_adc);
err:
	unregister_chrdev_region (devno, 1);
err1:
	return ret;
}

static int adc_int_remove (struct platform_device *dev)
{
	int irq;	
	//platform_get_irq => platform_get_resource (dev, IORESOURCE_IRQ, )
	irq = platform_get_irq (dev, 0) ;
	disable_irq (irq);
	free_irq (irq, NULL);
	clk_disable (clk_adc) ;
	iounmap (adc_reg_base);
	unregister_chrdev_region (MKDEV(major, minor), 1);
	cdev_del (&adc_int_dev);
	
	return 0;
} 

#ifdef  CONFIG_PM
int adc_suspend(struct platform_device *dev, pm_message_t state)
{
	clk_disable (clk_adc) ;
	ADC.ADCCON = ADC.ADCCON | (1 << 2) ;
}

int adc_resume(struct platform_device *dev)
{
	clk_enable (clk_adc);
	ADC.ADCCON = ADC.ADCCON & ~(1 << 2) ;
}
#endif

struct resource adc_resources [] = {
	[0] = {
		.start=0xF3000000,
		.end=0xF3000000+4 * 9-1,
		.flags=IORESOURCE_MEM,
	},
	[1] = {
		.start = 87 + 32,
		.end   = 87 + 32,
		.flags = IORESOURCE_IRQ,
	},	
};

void adc_release (struct device *devi)
{

}
struct platform_device adc_device_info ={
	.name ="adc_int_driver",
	.id = -1 ,
	.resource = adc_resources,
	.num_resources = 2,
	.dev = {
		.release = adc_release,
	}
} ;

struct platform_driver adc_driver = {
	.probe  = adc_int_probe,
	.remove = adc_int_remove,
#ifdef CONFIG_PM
	.suspend = adc_suspend,
	.resume  = adc_resume,
#endif
	.driver  = {
		.name ="adc_int_driver",
	}
};

__init int adc_int_init (void)
{
	platform_device_register(&adc_device_info);
	platform_driver_register(&adc_driver);
	return 0;
}

__exit void adc_int_exit (void)
{
	platform_device_unregister(&adc_device_info);
	platform_driver_unregister(&adc_driver);
}

MODULE_LICENSE("GPL");
module_init (adc_int_init);
module_exit (adc_int_exit);
