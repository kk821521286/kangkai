#include <linux/init.h>   //module_init module_exit 
#include <linux/module.h> //module_init 
#include <linux/kernel.h> //printk
#include <linux/cdev.h>   //cdev init
#include <linux/fs.h>	  //file_operations 	
#include <linux/errno.h>  //
#include <linux/platform_device.h>

/*
 *
 */
unsigned int *GPDCON ;
void *gpdcon_base ;
#define GPDCON (*((unsigned int *)gpdcon_base))
void *tcfg_base ;
#define TCFG0 (*((unsigned int *)tcfg_base))
#define TCFG1 (*((unsigned int *)(tcfg_base + 4)))
void *tcon_base ;
#define TCNTB1  (*((unsigned int *)tcfg_base))
#define TCMPB1  (*((unsigned int *)(tcfg_base + 4)))
#define TCON 	(*((unsigned int *)(tcfg_base + 8)))

#define LIGHT_ON 	  __IO('m', 0)
#define LIGHT_OFF 	  __IO('m', 1)
#define LIGHT_TUNNING __IO('m', 2)
// virtual address  = ioremap (phy_address, len) 
// phy address 		  iounmap (virtual_address) ;	
static unsigned int light_ioctl (struct file* filp, unsigned int cmd, unsigned long argv)
{
	switch (cmd) {
		case LIGHT_ON: 
			// 
			TCON  = (TCON & ~(1 << 9)) | (1 << 11) | (1 << 8) ;
			
			//	writel (); writew() writeb()
			//  readl () ; readw()  readb()
			
			break ;
		case LIGHT_OFF: 
			//
			TCON = TCON & ~(1 << 8) ;
			break ;
		case LIGHT_TUNNING: 
			//
			break ;
	}

	return 0 ;
}

void light_init (void) 
{
	GPDCON = (2 << 4) ;
	TCFG0 = 65;
	TCNTB1 = 1000 ;
	TCMPB1 = 500 ;
	TCON  = *TCON | (1 << 9) ;
	//	*TCON  = (*TCON & ~(1 << 9)) | (1 << 11) | (1 << 8) ;
}


//linux version > 2.6.39 
//ioctl -> unlocked_ioctl 
static struct file_operations light_ops = {
	.unlocked_ioctl = light_ioctl ,
};

//static ??? 
static struct cdev light_dev ;
static int major = 249, minor = 0;
//__init 
int light_probe (struct platform_device *dev)
{
	int ret = 0;
	struct resource *resource_tmp ;

	dev_t devno = MKDEV (major, minor) ;
	ret = register_chrdev_region (devno, 1, "light_dev_number");
	if (ret) {
		goto err;
	}
	cdev_init (&light_dev, &light_ops);
	ret = cdev_add (&light_dev, devno, 1) ;
	if (ret) {
		goto err1 ;
	}
	resource_tmp = platform_get_resource (dev, IORESOURCE_MEM, 0);
	gpdcon_base = ioremap (resource_tmp->start, resource_tmp->end - resource_tmp->start + 1);
	
	resource_tmp = platform_get_resource (dev, IORESOURCE_MEM, 1);
	tcfg_base = ioremap (resource_tmp->start, resource_tmp->end - resource_tmp->start + 1);

	resource_tmp = platform_get_resource (dev, IORESOURCE_MEM, 2);
	tcon_base = ioremap (resource_tmp->start, resource_tmp->end - resource_tmp->start + 1);
	
	light_init ();  
	return 0 ;
err1:
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
err:
	return ret;
}

//__exit 
int light_remove (struct platform_device *dev)
{
	cdev_del(&light_dev);
	unregister_chrdev_region (MKDEV(major, minor), 1) ;
	iounmap (gpdcon_base) ;
	iounmap (tcfg_base) ;
	iounmap (tcon_base);
	
	return 0;
}

static struct platform_driver light_drv =
{
	.probe  = light_probe,
	.remove = light_remove ,
	.driver = {
		.name = "light",
	}
};

__init int light_init (void)
{
	platform_driver_register (&light_drv) ;
	return 0;
}

__exit void light_exit (void)
{
	platform_driver_unregister (&light_drv) ;
}

MODULE_LICENSE ("GPL");
module_init (light_init) ;
module_exit (light_exit) ;
/*
 * struct module {
 *	.init = 
 *  .exit =  
 * }
 */








