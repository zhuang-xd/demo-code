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

// 开
int led_open(struct inode *inode, struct file *file);
// 关
int led_close(struct inode *inode, struct file *file);
// GPIO控制
long led_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

// 定义文件操作结构体
struct file_operations fops = {
    .open = led_open,
    .release = led_close,
    .unlocked_ioctl = led_ioctl,
};

#endif //_LED_H_