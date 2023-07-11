#ifndef _BEEP_H_
#define _BEEP_H_

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/ioctl.h>

// 开
int beep_open(struct inode *inode, struct file *file);
// 关
int beep_close(struct inode *inode, struct file *file);
// 读
ssize_t beep_read(struct file *file, char __user *ubuf, size_t size, loff_t *lof);
// 写
ssize_t beep_write(struct file *file, const char __user *ubuf, size_t size, loff_t *lof);
// GPIO控制
long beep_ioctl (struct file *file, unsigned int cmd, unsigned long arg);

// 重写文件操作结构体
struct file_operations fops = {
    .open = beep_open,
    .release = beep_close,
    .read = beep_read,
    .write = beep_write,
    .unlocked_ioctl = beep_ioctl,
};

#endif //_BEEP_H_