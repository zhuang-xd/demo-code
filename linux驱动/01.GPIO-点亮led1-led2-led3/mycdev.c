#include "mycdev.h"

static int major; // 保存主设备号
static char kbuf[128] = { 0 }; // 缓冲区

static gpio_t *vir_gpioe;
static gpio_t *vir_gpiof;
static unsigned int *vir_rcc;

/**
 * @brief    初始化
 */
static int __init mycdev_init(void)
{
    printk(KERN_ALERT "install demo mod\n");
    // 注册驱动
    major = register_chrdev(0, "mychrdev", &fops);
    if (major < 0) {
        printk("字符设备驱动注册失败\n");
        return major;
    }
    printk("字符设备驱动注册成功%d\n", major);

    // 映射寄存器
    vir_gpioe = ioremap(PHY_GPIOE, sizeof(gpio_t));
    if (!vir_gpioe) {
        printk("GPIOE 地址映射失败\n");
        return -EFAULT;
    }

    vir_gpiof = ioremap(PHY_GPIOF, sizeof(gpio_t));
    if (!vir_gpiof) {
        printk("GPIOF 地址映射失败\n");
        return -EFAULT;
    }

    vir_rcc = ioremap(PHY_RCC, 4);
    if (vir_rcc == NULL) {
        printk("RCC 寄存器地址映射失败\n");
        return -EFAULT;
    }
    printk("寄存器地址映射成功\n");

    // RCC初始化
    (*vir_rcc) |= (0x3 << 4);

    // led1
    (*vir_gpioe).moder &= (~(0X3 << 20));
    (*vir_gpioe).moder |= (0X1 << 20);
    (*vir_gpioe).otyper &= (~(0X1 << 10));
    (*vir_gpioe).ospeedr &= (~(0X3 << 20));
    (*vir_gpioe).pupdr &= (~(0X3 << 20));

    // led2
    (*vir_gpiof).moder &= (~(0X3 << 20));
    (*vir_gpiof).moder |= (0X1 << 20);
    (*vir_gpiof).otyper &= (~(0X1 << 10));
    (*vir_gpiof).ospeedr &= (~(0X3 << 20));
    (*vir_gpiof).pupdr &= (~(0X3 << 20));

    // led3
    (*vir_gpioe).moder &= (~(0X3 << 16));
    (*vir_gpioe).moder |= (0X1 << 16);
    (*vir_gpioe).otyper &= (~(0X1 << 8));
    (*vir_gpioe).ospeedr &= (~(0X3 << 16));
    (*vir_gpioe).pupdr &= (~(0X3 << 16));

    printk("寄存器初始化成功\n");

    return 0;
}

/**
 * @brief    卸载
 */
static void __exit mycdev_exit(void)
{
    // 注销驱动
    unregister_chrdev(major, "mychrdev");
    printk("字符设备驱动 %d 注销成功\n", major);
    printk(KERN_ALERT "remove demo mod\n");

    // 取消映射
    iounmap(vir_gpioe);
    iounmap(vir_gpiof);
    iounmap(vir_rcc);
    printk("寄存器取消映射成功\n");
}

/**
 * @brief    开
 */
int mycdev_open(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

/**
 * @brief    关
 */
int mycdev_close(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

/**
 * @brief    读
 */
ssize_t mycdev_read(struct file *file, char __user *ubuf, size_t size, loff_t *lof)
{
    // 拷贝内核缓冲区 到 用户空间缓冲区
    int ret;
    if (sizeof(kbuf) < size) // 同步内核的缓冲区大小
        size = sizeof(kbuf);
    ret = copy_to_user(ubuf, kbuf, size);

    if (ret) {
        printk("copy_to_user filed\n");
        return -EIO;
    }

    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

/**
 * @brief    写
 */
ssize_t mycdev_write(struct file *file, const char __user *ubuf, size_t size, loff_t *lof)
{
    // 拷贝用户空间缓冲区 到 内核空间
    int ret;
    if (sizeof(kbuf) < size)
        size = sizeof(kbuf);
    ret = copy_from_user(kbuf, ubuf, size);

    if (ret) {
        printk("copy_to_user filed\n");
        return -EIO;
    }
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);

    // LED亮灭逻辑
    if (kbuf[0] == '1') { // 亮
        (*vir_gpioe).odr |= (0x1 << 10);
        (*vir_gpiof).odr |= (0x1 << 10);
        (*vir_gpioe).odr |= (0x1 << 8);
    } else if (kbuf[0] == '0') { // 灭
        (*vir_gpioe).odr &= (~(0x1 << 10));
        (*vir_gpiof).odr &= (~(0x1 << 10));
        (*vir_gpioe).odr &= (~(0x1 << 8));
    }

    return 0;
}

module_init(mycdev_init);
module_exit(mycdev_exit);
MODULE_LICENSE("GPL");
