#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/clk.h>

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
unsigned int *CLK_GATE_D1_5 = NULL;
static struct clk *clk_adc; 

static ssize_t adc_poll_read(struct file *filp, char *buffer, size_t len, loff_t* fpos)
{
	unsigned int adv = 0 ;

	adv = ADC.ADCDAT0 & 0xfff ;
	
	//adc conversion end flag
	while (!(ADC.ADCCON & (1 << 15))) ;
	adv = ADC.ADCDAT0 & 0xfff ;

	printk (KERN_ALERT"adc poll driver adv = 0x%x\n", adv);
	if (copy_to_user ((void *)buffer, (void *)&adv, sizeof (adv)))
		return -ENOMEM;

	return len;
}

static void adc_hardware_init (void)
{
	ADC.ADCMUX = 0 ;
	//12BIT      预分频              分频值
	//读转换模式
	ADC.ADCCON = (1 << 16) | (1 << 14) | (65 << 6) | (1	<< 1);
}

static struct file_operations adc_poll_ops = {
	.read = adc_poll_read, 
};

static int major = 249, minor = 0;
static struct cdev adc_poll_dev;
__init static int adc_poll_init (void)
{
	int ret = 0;
	dev_t devno = MKDEV(major, minor);
	ret = register_chrdev_region (devno, 1, "adc_poll_devno");
	if (ret) {
		goto err1;
	} 

	cdev_init (&adc_poll_dev, &adc_poll_ops);
	ret = cdev_add (&adc_poll_dev, devno, 1) ;
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
	printk (KERN_ALERT"adc_poll_init \n");
	return 0;

err:
	unregister_chrdev_region (devno, 1);
err1:
	return ret;
}

__exit static void adc_poll_exit (void)
{
	clk_disable (clk_adc) ;
	iounmap (adc_reg_base);
	unregister_chrdev_region (MKDEV(major, minor), 1);
	cdev_del (&adc_poll_dev);
	printk (KERN_ALERT"adc_poll_exit\n");
} 

MODULE_LICENSE("GPL");
module_init (adc_poll_init);
module_exit (adc_poll_exit);
