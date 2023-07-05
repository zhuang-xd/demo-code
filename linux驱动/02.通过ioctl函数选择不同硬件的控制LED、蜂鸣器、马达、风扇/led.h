#ifndef _my_led_H_
#define _my_led_H_

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/ioctl.h>

// 开
int my_led_open(struct inode *inode, struct file *file);
// 关
int my_led_close(struct inode *inode, struct file *file);
// 读
ssize_t my_led_read(struct file *file, char __user *ubuf, size_t size, loff_t *lof);
// 写
ssize_t my_led_write(struct file *file, const char __user *ubuf, size_t size, loff_t *lof);
// GPIO控制
long my_led_ioctl (struct file *file, unsigned int cmd, unsigned long arg);

// 重写文件操作结构体
struct file_operations fops = {
    .open = my_led_open,
    .release = my_led_close,
    .read = my_led_read,
    .write = my_led_write,
    .unlocked_ioctl = my_led_ioctl,
};

#endif //_my_led_H_