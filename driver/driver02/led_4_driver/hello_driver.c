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

static void led_on (void )
{
	GPG3.GPG3DAT = 0xf ;
}

static void led_off (void)
{
	GPG3.GPG3DAT = 0x0 ;
}

static void led_config (void) 
{
	GPG3.GPG3CON = 0X1111;
}

#define CMD_LED_ON  _IO('k', 0)
#define CMD_LED_OFF _IO('k', 1)

static int led_ioctl (struct inode *inode, struct file* filp, unsigned int cmd, long arg)
{
	switch (cmd) {
	case CMD_LED_ON:
		led_on () ;
		break ;
	case CMD_LED_OFF :
		led_off ();
		break ;
	default:
		return -EINVAL ; //Command id not support
		break ;
	}

	return 0;
}

static int hello_open (struct inode*inode, struct file* file) 
{
	printk (KERN_ALERT"char device open! =  %d\r\n", iminor (inode)) ;
	return  0;
}

static int hello_release (struct inode *inode, struct file* file)
{
	printk (KERN_ALERT"char device close! = %d\r\n", iminor (inode)) ;
	return  0;
}

static struct file_operations hello_ops = {
	.open = hello_open ,
	.release = hello_release ,
	.ioctl = led_ioctl ,
#if 0
	.read = hello_read ,
	.write = hello_write ,
#endif
} ;

static struct cdev hello_cdev ;
__init static int hello_driver_init (void)
{
	int ret ; 
	dev_t devno = MKDEV (major, minor) ;
	//
	ret = register_chrdev_region (devno, MINOR_NR, "hello_device") ;
	if (ret) {
		printk (KERN_ALERT"char device num fail!\r\n") ;
		goto err ;
	}
	
#if 0
	for (i = 0; i < MINOR_NR; i++) {
		cdev_init (&hello_cdev[i], &hello_ops) ; // hello_ops -> cdev->ops
		ret = cdev_add (&hello_cdev[i], devno + i, 1) ;
	}
#endif
	cdev_init (&hello_cdev, &hello_ops) ;
	ret = cdev_add (&hello_cdev, devno, MINOR_NR) ;

#if 1
	if (ret) {
		printk (KERN_ALERT"CDEV ADD ERROR") ;
		goto err1;
	}
#endif

	base = ioremap (0xe03001c0, sizeof (gpg3)) ;
	if (NULL == base) {
		printk (KERN_ALERT"IOREMAP ERR!\r\n") ;
		goto err2;
	}
	led_config () ;

	return 0 ;
err2:
	cdev_del (&hello_cdev) ;
err1:
	unregister_chrdev_region (devno, MINOR_NR);
err:
	return ret ;
}

__exit static void hello_driver_exit (void) 
{
	unregister_chrdev_region (MKDEV(major, minor), MINOR_NR) ;
	cdev_del (&hello_cdev) ;
#if 0
	for (i = 0; i < MINOR_NR; i++) {
		cdev_del (&hello_cdev[i]) ;
	}
#endif
	iounmap (base) ;
	printk(KERN_ALERT"cleanup module\r\n") ;
}

MODULE_LICENSE ("GPL") ;
MODULE_AUTHOR("GUO") ;
MODULE_DESCRIPTION("THIS IS SIMAPLE HELLO MODULE") ;
//
module_init (hello_driver_init) ;
module_exit (hello_driver_exit) ;


