#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/i2c.h>

/*
 *
 *  int (*probe)(struct i2c_client *, const struct i2c_device_id *);
 *  150     int (*remove)(struct i2c_client *);
 *
/*
 * struct i2c_device_id *id_table;
 */

static struct i2c_client *new_client ;

// int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)
static ssize_t lm75_read (struct file* file, char *buf, size_t len, loff_t *fpos)
{
	unsigned char point = 0 ;
	unsigned char recvbuf [2] ;
	struct i2c_msg msg;
	msg.addr = client.addr ;
	msg.flags = I2C_M_WR;
	msg.len = 1;
	msg.buf = &point ;
	i2c_transfer (client->adapter, &msg, 1);
	
	msg.addr = client.addr;
	msg.flags = I2C_M_RD;
	msg.len = 2;
	msg.buf = recvbuf ;
	i2c_transfer (client->adapter, &msg, 1);
	
	if (copy_to_user (buf, recvbuf, 2)) 
		return -ENOMEM;

	return len ;
}

static struct file_operations lm75_ops = {
	.read = lm75_read ,
};
static int major = 249, minor = 0;
static struct cdev lm75_cdev;
int new_probe (struct i2c_client *client, const struct i2c_device_id *new_id)
{
	dev_t devno = MKDEV(major, minor);
	int ret = 0;

	ret = register_chrdev_region (devno, 1, "lm75");
	if (ret) {
	
	}

	cdev_init (&lm75_cdev, &lm75_ops);
	ret = cdev_add (&lm75_cdev, devno, 1) ;
	if (ret) {

	}
	
	new_client = client;
	return 0;
}

int new_remove (struct i2c_client *client)
{
	cdev_del (&lm75_cdev);
	unregister_chrdev_region(MKDEV(major, minor), 1);
}

static struct i2c_device_id new_device_id = {
	"lm75", 0
};

static struct i2c_driver new_driver = {
	.probe  = new_probe,
	.remove = new_remove,
	.id_table = &new_device_id,
};
__init int i2c_new_driver_init (void)
{
	i2c_add_driver (&new_driver);
}

__exit void i2c_new_driver_exit (void)
{	
	i2c_del_driver (&new_driver);
}

MODULE_LICENSE ("GPL");
module_init (i2c_new_driver_init);
module_exit (i2c_new_driver_exit);
