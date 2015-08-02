#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h> //module_init module_exit 

//byte charp 
int param = 10 ;
module_param (param, int, 0600) ;

//insmod modulenamei
//int init_module (void) 
int hello_init (void)
{
	printk (KERN_ALERT"init module : param = %d\r\n", param) ;
	return 0 ;
}

//rmmod modulenae
void hello_exit (void) 
{
	printk(KERN_ALERT"cleanup module\r\n") ;
}

void shashabuzuo (void) 
{

}

MODULE_LICENSE ("GPL") ;
MODULE_AUTHOR("GUO") ;
MODULE_DESCRIPTION("THIS IS SIMAPLE HELLO MODULE") ;
//EXPORT_SYMBOL(shashabuzuo) ;
EXPORT_SYMBOL_GPL(shashabuzuo) ;
//
module_init (hello_init) ;
module_exit (hello_exit) ;


