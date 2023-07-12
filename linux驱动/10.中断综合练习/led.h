#ifndef _LED_H_
#define _LED_H_

#include <linux/cdev.h> // 注册设备头文件
#include <linux/device.h>
#include <linux/fs.h> //定义文件操作结构体头文件
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/slab.h>
#include <linux/timer.h> // 定时器头文件
#include <linux/uaccess.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/poll.h>

// 读
ssize_t led_read(struct file *file, char *ubuf, size_t size, loff_t *lof);
// GPIO控制
long led_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#endif //_LED_H_