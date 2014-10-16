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
#define NUM 10//前10个
static struct timer_list tmp_timer;/*内核定时器*/
static struct task_struct *task_p;
struct timeval tv;
struct rtc_time tm;
struct SORT{
		int state;
		unsigned long mem;
		long pid;
		char name[20];
	};//定义结构体
struct SORT temp[NUM];
struct SORT cpy;

static void my_function(void){
	printk("Hello World!\n");
	do_gettimeofday(&tv);
	rtc_time_to_tm(tv.tv_sec+8*3600,&tm);
	printk("The end time is :%d-%d-%d %d:%d:%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);	
}
static void cpyprocess(struct SORT *p1,struct SORT *p2){//匹配
	strcpy(p1->name,p2->name);
	p1->pid = p2->pid;
	p1->state = p2->state;
	p1->mem = p2->mem;
}
static void insertsort(struct SORT temp[],struct SORT cpy)
{
	int i,j;
	for(i = 0 ;i < NUM; i++)
	{
		if(temp[i].mem <= cpy.mem){
			for(j = NUM-1;j > i;j--)
				cpyprocess(&temp[j],&temp[j-1]);
				break;	//查找匹配后改变
			}	
	}
	cpyprocess(&temp[i],&cpy);//没有或是退出来直接插入
}
static void print(int para)
{	
	int i;
	for_each_process(task_p){//遍历进程
		if(task_p->mm != NULL){
			cpy.state = task_p->state;
			cpy.mem = task_p->mm->total_vm;
			cpy.pid = task_p->pid;
			strcpy(cpy.name,task_p->comm);			
			insertsort(temp,cpy);//排序			
		//printk("state=%d,mem=%lu,pid=%ld,name=%s\n",(int)task_p->state,task_p->mm->total_vm,(long)task_p->pid,task_p->comm);		
		}	
	}
	for(i = 0;i < NUM; i++)
		printk("state=%d,mem=%lu,pid=%ld,name=%s\n",temp[i].state,temp[i].mem,temp[i].pid,temp[i].name);//打印排序好的前10个
	my_function();
}
static int __init hello_proc_init(void)
{	
	int i;	
	for(i = 0;i < NUM; i++)	//初始化为0
		temp[i].mem = 0;
	init_timer(&tmp_timer);	//初始化定时器
	tmp_timer.expires = jiffies + (10*HZ);//延时10s打印
	tmp_timer.data = 0;//函数传参
	tmp_timer.function = print;//调用延时后的函数
	add_timer(&tmp_timer);//启动定时器
	my_function();
	return 0;
}
static void __exit hello_proc_exit(void)
{
	printk("GoodBye kernel\n");
	del_timer(&tmp_timer);//关闭定时器
}
MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
