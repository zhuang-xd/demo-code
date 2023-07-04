#ifndef _MYCDEV_H_
#define _MYCDEV_H_

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/uaccess.h>

// 开
int mycdev_open(struct inode *inode, struct file *file);
// 关
int mycdev_close(struct inode *inode, struct file *file);
// 读
ssize_t mycdev_read(struct file *file, char __user *ubuf, size_t size, loff_t *lof);
// 写
ssize_t mycdev_write(struct file *file, const char __user *ubuf, size_t size, loff_t *lof);

// 重写文件操作结构体
struct file_operations fops = {
    .open = mycdev_open,
    .release = mycdev_close,
    .read = mycdev_read,
    .write = mycdev_write,
};

// 定义寄存器组织结构体
typedef struct {
    unsigned int moder;
    unsigned int otyper;
    unsigned int ospeedr;
    unsigned int pupdr;
    unsigned int idr;
    unsigned int odr;
} gpio_t;

// 定义GPIOE和GPIOF
#define PHY_GPIOE 0X50006000
#define PHY_GPIOF 0X50007000
#define PHY_RCC   0x50000A28

#endif //_MYCDEV_H_