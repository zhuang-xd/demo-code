#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/timer.h>

/**
 *  相关API
 *  #include <linux/timer.h> 
 *  1. 配置定时器对象 
 *       struct timer_list *timer_create(struct clocksource *clock, 
 *  2. 初始化定时器   
 *       timer_setup(timer, callback, flags);
 *  3. 将定时器对象注册进内核，并启用
 *       extern void add_timer(struct timer_list *timer);
 *  4. 再次启用定时器
 *       extern int mod_timer(struct timer_list *timer, unsigned long expires); 
 *  5. 删除定时器 
 *       extern int del_timer(struct timer_list *timer);
 */

struct timer_list my_timer; // 创建定时器对象

// 定时器回调
void callback(struct timer_list *timer)
{
    printk(KERN_ALERT "show\n");
    // 4. 再次启用这个定时器
    mod_timer(&my_timer, jiffies+HZ);
}

static int __init mycdev_init(void)
{
    // 1. 配置定时器对象 
    my_timer.expires = jiffies+HZ; // 1秒定时

    // 2. 初始化定时器   
    timer_setup(&my_timer,callback,0);

    // 3. 将定时器对象注册进内核并启用
    add_timer(&my_timer);

    return 0;
}

static void __exit mycdev_exit(void)
{
    // 5. 删除定时器
    del_timer(&my_timer);
}

module_init(mycdev_init);
module_exit(mycdev_exit);
MODULE_LICENSE("GPL");