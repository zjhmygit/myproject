#include <linux/kernel.h>		// include/linux/xxx.h 
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <linux/workqueue.h>

//struct tasklet_struct mytask;
struct work_struct mywork; 


int kdata = 88;
int udata;


void led_on(void)
{
	printk("led on\n");
}

void led_off(void)
{
	printk("led off\n");
}

irqreturn_t key2_isr(int irq, void *dev_id)
{
	printk("key2 put down\n");

	//tasklet_schedule(&mytask);
	schedule_work(&mywork);

	return IRQ_HANDLED;
}

//void led_tasklet(unsigned long data)
void led_work(struct work_struct *wk)
{
	int cnt;
	cnt = 20;
	while(cnt)
	{
		printk("intr loop...\n");
		led_on();
		mdelay(1000);
		led_off();
		cnt--;
	}
}

int __init intr_init(void)		// init_intr
{
	printk(KERN_NOTICE "++++++> intr init\n");

	int kdata = 88;

	//зЂВс
	int err;
	err = request_irq(IRQ_EINT(16) ,key2_isr, IRQF_TRIGGER_FALLING | IRQF_DISABLED, "key2_isr", &kdata);
	if(err)
	{
		printk("request irq fail\n");
		return -1;
	}
	printk("eint16: %d\n", IRQ_EINT(16));

	//tasklet_init(&mytask, led_tasklet, 88);
	INIT_WORK(&mywork, led_work);
	

	while(1)
	{
		printk("main thread loop...\n");
		ssleep(1);
	}


	
	return 0;
}


void __exit intr_exit(void)
{
	printk(KERN_NOTICE "-----> intr exit\n");
//	tasklet_kill(&mytask);
	free_irq(IRQ_EINT(16), &kdata);
}

module_init(intr_init);	
module_exit(intr_exit);

MODULE_AUTHOR("jo6nfi3h");
MODULE_DESCRIPTION("a simple intr driver");
MODULE_LICENSE("GPL");
