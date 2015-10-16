# Kernel_Timer
####内核中定时器的使用
关键函数:
```
init_timer(&tmp_timer);	//初始化定时器  
add_timer(&tmp_timer);//启动定时器  
del_timer(&tmp_timer);//关闭定时器  
tmp_timer.expires = jiffies + (10*HZ);//延时10s打印
tmp_timer.data = 0;//函数传参
tmp_timer.function = print;//调用延时后的函数
for_each_process(task_p){}//遍历进程
static struct task_struct *task_p; //task_struct结构体
