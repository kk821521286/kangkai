#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> //module_init module_exit 
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <asm/io.h>

void *base = NULL ;
/* GPG3 */
typedef struct {
	unsigned int GPG3CON;
	unsigned int GPG3DAT;
	unsigned int GPG3PULL;
	unsigned int GPG3DRV;
	unsigned int GPG3PDNCON;
	unsigned int GPG3PDNPULL;
}gpg3;
#define GPG3 (* (volatile gpg3 * )base )

static int major = 249 ;
static int minor = 0 ;
#define MINOR_NR 4

static void led_on (int num)
{
	GPG3.GPG3DAT = GPG3.GPG3DAT |(1 << num) ;
}

static void led_off (int num)
{
	GPG3.GPG3DAT = GPG3.GPG3DAT & ~(1 << num) ;
}

static void led_config (void) 
{
	GPG3.GPG3CON = (GPG3.GPG3CON & ~(0xffff)) | 0X1111;
}

#define CMD_LED_ON  _IO('k', 0)
#define CMD_LED_OFF _IO('k', 1)
static int led_ioctl (struct inode *inode, struct file* filp, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case CMD_LED_ON:
		led_on (iminor(inode)) ;
		break ;
	case CMD_LED_OFF :
		led_off (iminor(inode));
		break ;
	default:
		return -EINVAL ; //Command id not support
		break ;
	}

	return 0;
}

static int led_open (struct inode*inode, struct file* file) 
{
	led_off (iminor (inode));
	return  0;
}

static int led_release (struct inode *inode, struct file* file)
{
	led_off (iminor (inode)); 
	return  0;
}

static struct file_operations led_ops = {
	.open    = led_open ,
	.release = led_release ,
	.ioctl   = led_ioctl ,
} ;

static struct cdev led_cdev ;
__init static int led_driver_init (void)
{
	int ret ; 
	dev_t devno = MKDEV (major, minor) ;
	
	ret = register_chrdev_region (devno, MINOR_NR, "led_device") ;
	if (ret) {
		printk (KERN_ALERT"char device num fail!\r\n") ;
		goto err ;
	}
	
	cdev_init (&led_cdev, &led_ops) ;
	ret = cdev_add (&led_cdev, devno, MINOR_NR) ; // mknod /dev/led0 c 249 0 1 2 3
	
	if (ret) {
		printk (KERN_ALERT"CDEV ADD ERROR") ;
		goto err1;
	}

	base = ioremap (0xe03001c0, sizeof (gpg3)) ;
	if (NULL == base) {
		printk (KERN_ALERT"IOREMAP ERR!\r\n") ;
		goto err2;
	}
	led_config () ;

	printk (KERN_ALERT"LED 4 DEVICE DRIVER INSMOD DONE!\n") ; 
	return 0 ; 
err2:
	cdev_del (&led_cdev) ;
err1:
	unregister_chrdev_region (devno, MINOR_NR);
err:
	return ret ;
}

__exit static void led_driver_exit (void) 
{
	unregister_chrdev_region (MKDEV(major, minor), MINOR_NR) ;
	cdev_del (&led_cdev) ;
	iounmap (base) ;
	printk(KERN_ALERT"LED 4 DEVICE DRIVER RMMOD DONE\r\n") ;
}

MODULE_LICENSE ("GPL") ;
MODULE_AUTHOR("GUO") ;
MODULE_DESCRIPTION("THIS IS SIMAPLE led MODULE") ;
//
module_init (led_driver_init) ;
module_exit (led_driver_exit) ;


