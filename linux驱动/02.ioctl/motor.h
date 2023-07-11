#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/ioctl.h>

// 开
int motor_open(struct inode *inode, struct file *file);
// 关
int motor_close(struct inode *inode, struct file *file);
// 读
ssize_t motor_read(struct file *file, char __user *ubuf, size_t size, loff_t *lof);
// 写
ssize_t motor_write(struct file *file, const char __user *ubuf, size_t size, loff_t *lof);
// GPIO控制
long motor_ioctl (struct file *file, unsigned int cmd, unsigned long arg);

// 重写文件操作结构体
struct file_operations fops = {
    .open = motor_open,
    .release = motor_close,
    .read = motor_read,
    .write = motor_write,
    .unlocked_ioctl = motor_ioctl,
};

#endif //_MOTOR_H_