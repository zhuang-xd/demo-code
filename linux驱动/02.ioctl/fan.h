#ifndef _fan_H_
#define _fan_H_

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/ioctl.h>

// 开
int fan_open(struct inode *inode, struct file *file);
// 关
int fan_close(struct inode *inode, struct file *file);
// 读
ssize_t fan_read(struct file *file, char __user *ubuf, size_t size, loff_t *lof);
// 写
ssize_t fan_write(struct file *file, const char __user *ubuf, size_t size, loff_t *lof);
// GPIO控制
long fan_ioctl (struct file *file, unsigned int cmd, unsigned long arg);

// 重写文件操作结构体
struct file_operations fops = {
    .open = fan_open,
    .release = fan_close,
    .read = fan_read,
    .write = fan_write,
    .unlocked_ioctl = fan_ioctl,
};

#endif //_fan_H_