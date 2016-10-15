#include <linux/module.h>
#include <linux/kernel.h>
#include<linux/init.h>
#include<asm/io.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include<linux/errno.h>
#include<linux/ioport.h>
#include<linux/device.h>
#include<linux/cdev.h>

dev_t  devid;
static int devid_major;
struct cdev keyled_cdev;

#define LED_CTROL	1

unsigned long *iobase = NULL;
unsigned long gpj2con, gpj2dat, gph2con, gph2dat;

unsigned long kdata;


void led_key_init(void)
{
	unsigned int val;
	val = ioread32(gpj2con);
	val &=~(0xffff<<0);
	val |=(0x1111<<0);
	iowrite32(val, gpj2con);

	val = ioread32(gpj2dat);
	val &=~(0xf<<0);
	iowrite32(val, gpj2dat);

	val = ioread32(gph2con);
	val &=~(0xf<<0);
	iowrite32(val, gph2con);
	
}

int keyled_open (struct inode *inode, struct file *file)
{
	printk("keyled_open!\n");
	
	led_key_init();
	
	return 0;
}


int keyled_close(struct inode *inode, struct file *file)
{
	return 0;
}


int keyled_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg )
{
	unsigned int val;
	switch(cmd)
	{
		case LED_CTROL:
			val = ioread32(gpj2dat);
			val ^=(0x1<<0);
			iowrite32(val, gpj2dat);
			break;
			
	}
	return 0;
}

ssize_t keyled_read(struct file *file, char __user *buf, size_t size, loff_t * offest)
{
	printk("keyled read...\n");

	kdata = ioread32(gph2dat);

	if(copy_to_user(buf, &kdata, size))
	{
		printk("copy data to user fail!\n");
		return -1;
	}
	return 0;
}


struct file_operations keyled_fops = {
	.open = keyled_open,
	.release = keyled_close,
	.ioctl		= keyled_ioctl,
	.read	= keyled_read,
};

int __init keyled_module_init(void)
{
	int err;
	kdata = 0;

/*old*/	
	//register_chrdev(88, "keyled", &keyled_fops);
	err = alloc_chrdev_region(&devid,  0,  1, "keyled");
	if(err<=0)
	{
		printk("alloc_chrdev_region fail!\n");
		return -EBUSY;
	}

	devid_major = MAJOR(devid);

	cdev_init(&keyled_cdev, &keyled_fops);
	err = cdev_add(&keyled_cdev, devid, 1);
	if(err<0)
	{
		printk("cdev_add fail !\n");
		return err;
	}

	
	err = request_mem_region(0xe0200000, 0x1000, "keyled-mem");
	if(!err)
	{
		printk("keyed request mem region fail!\n");
		return -EBUSY;
	}

	iobase = (unsigned long *)ioremap(0xe0200000, 0x1000);
	if(iobase == NULL)
	{
		printk("ioremap fail!\n");
		return -EBUSY;
	}

	gpj2con		 = (unsigned long)iobase + 0x280;
	gpj2dat 		 = (unsigned long)iobase + 0x284;
	gph2con		 = (unsigned long)iobase +0xc40;
	gph2dat 	 = (unsigned long)iobase + 0xc44;

	
	return 0;	
	
}

 void keyled_moudle_exit(void)
{
	printk("----------keyled exit----------!\n");

	iounmap(iobase);
	release_mem_region(0xe0200000, 0x1000);

	cdev_del(&keyled_cdev);
/*old*/
	//unregister_chrdev(88, "keyled");
	unregister_chrdev_region(devid, "keyled");
	
}

module_init(keyled_module_init);
module_exit(keyled_moudle_exit);


MODULE_AUTHOR("ZJH");
MODULE_DESCRIPTION("a simple keyled dirver");
MODULE_LICENSE("GPL");







