#ifndef _LED_H_
#define _LED_H_

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

// 开
int led_open(struct inode *inode, struct file *file);
// 关
int led_close(struct inode *inode, struct file *file);
// GPIO控制
long led_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

// 重写文件操作结构体
struct file_operations fops = {
    .open = led_open,
    .release = led_close,
    .unlocked_ioctl = led_ioctl,
};

#endif //_LED_H_