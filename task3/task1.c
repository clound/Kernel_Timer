#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/utsname.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/rtc.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#define TIMER_OVER 3/*调用时间间隔*/
static struct timer_list tmp_timer;/*内核定时器*/
struct timeval tv;
struct rtc_time tm;
int flag = 0;
static void my_function(void){
	printk("Hello World!\n");
	do_gettimeofday(&tv);
	rtc_time_to_tm(tv.tv_sec+8*3600,&tm);
	printk("The end time is :%d-%d-%d %d:%d:%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);	
}
static void print(int para)
{	
	flag = 1;
	my_function();
}
static int __init hello_proc_init(void)
{
	init_timer(&tmp_timer);	
	tmp_timer.expires = jiffies + (10*HZ);
	tmp_timer.data = 0;
	tmp_timer.function = print;
	add_timer(&tmp_timer);
	my_function();
	del_timer_sync(&tmp_timer);
	msleep(11000);
	my_function();
	return 0;
}
static void __exit hello_proc_exit(void)
{
	printk("GoodBye kernel\n");
	del_timer(&tmp_timer);
}
MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
