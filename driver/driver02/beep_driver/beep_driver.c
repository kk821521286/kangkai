#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> //module_init module_exit 
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/errno.h>
#include <asm/io.h>

static int major = 248 ;
static int minor = 0 ;
#define MINOR_NR 4

void *gpd_base = NULL ;
typedef struct {
				unsigned int GPDCON;
				unsigned int GPDDAT;
				unsigned int GPDPULL;
				unsigned int GPDDRV;
				unsigned int GPDPDNCON;
				unsigned int GPDPDNPULL;
}gpd;
#define GPD (* (volatile gpd * )gpd_base ) 

static void *timer_base ;
typedef struct {
				unsigned int TCFG0;
				unsigned int TCFG1;
				unsigned int TCON;
}timer_type;
#define  TIMER  (* (volatile timer_type *)timer_base)

static void *timer1_base ;
typedef struct {

				unsigned int TCNTB1;
				unsigned int TCMPB1;
				unsigned int TCNTO1;
}timer1_type;
#define  TIMER1 (* (volatile timer1_type *)timer1_base ) 

void beep_init (void)
{
	GPD.GPDCON = (GPD.GPDCON & (~(0xf << 4))) | (2 << 4) ; //读改写
	TIMER.TCFG0 =  TIMER.TCFG0 | 65 ;
	TIMER.TCFG1 = TIMER.TCFG1 & (~(0xf << 4)) ;
	TIMER1.TCNTB1 = 10000 ;
	TIMER1.TCMPB1 = 10000 / 2 ;
	TIMER.TCON = TIMER.TCON | (1 << 9) ;
}

void beep_on (void)
{
	TIMER.TCON &= ~(1 << 9) ;
	TIMER.TCON = TIMER.TCON | (1 << 8) | (1 << 11) ;
}

void beep_off (void)
{
	TIMER.TCON &=  ~(1 << 8 | 1 << 11);
}

void set_fre (int fre) 
{
	TIMER1.TCNTB1 = 1000000 / fre ;
	TIMER1.TCMPB1 = 1000000 / fre / 2 ;
}

#define CMD_BEEP_ON  _IO('k', 0)
#define CMD_BEEP_OFF _IO('k', 1)
#define CMD_SET_FRE  _IO('k', 2) 
static int beep_ioctl (struct inode *inode, struct file* filp, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case CMD_BEEP_ON:
		beep_on () ;
		break ;
	case CMD_BEEP_OFF :
		beep_off ();
		break ;
	case CMD_SET_FRE :
		set_fre (arg) ;
		break ;
	default:
		return -EINVAL ; //Command id not support
		break ;
	}

	return 0;
}

static int beep_open (struct inode*inode, struct file* file) 
{
	beep_off ();
	return  0;
}

static int beep_release (struct inode *inode, struct file* file)
{
	beep_off (); 
	return  0;
}

static struct file_operations beep_ops = {
	.open    = beep_open ,
	.release = beep_release ,
	.ioctl   = beep_ioctl ,
} ;

static struct cdev beep_cdev ;
__init static int beep_driver_init (void)
{
	int ret ; 
	dev_t devno = MKDEV (major, minor) ;
	
	ret = register_chrdev_region (devno, MINOR_NR, "beep_device") ;
	if (ret) {
		printk (KERN_ALERT"char device num fail!\r\n") ;
		goto err ;
	}
	
	cdev_init (&beep_cdev, &beep_ops) ;
	ret = cdev_add (&beep_cdev, devno, MINOR_NR) ;
	
	if (ret) {
		printk (KERN_ALERT"CDEV ADD ERROR") ;
		goto err1;
	}

	timer_base = ioremap (0xEA000000, sizeof (timer_type)) ;
	timer1_base = ioremap (0xEA000018, sizeof (timer1_type)) ;
	gpd_base = ioremap (0xE0300080, sizeof (gpd)) ;

	beep_init () ;
	printk (KERN_ALERT"beep DRIVER INSMOD DONE!\n") ; 
	return 0 ; 
err1:
	unregister_chrdev_region (devno, MINOR_NR);
err:
	return ret ;
}

__exit static void beep_driver_exit (void) 
{
	unregister_chrdev_region (MKDEV(major, minor), MINOR_NR) ;
	cdev_del (&beep_cdev) ;
	iounmap (timer_base) ;
	iounmap (timer1_base) ;
	iounmap (gpd_base) ;
	printk(KERN_ALERT"beep DRIVER RMMOD DONE\r\n") ;
}

MODULE_LICENSE ("GPL") ;
MODULE_AUTHOR("GUO") ;
MODULE_DESCRIPTION("THIS IS SIMAPLE beep MODULE") ;
//
module_init (beep_driver_init) ;
module_exit (beep_driver_exit) ;


