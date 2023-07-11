#include "beep.h"
#include "head.h"

static int major; // 保存主设备号
static char kbuf[128] = { 0 }; // 缓冲区

// 虚拟地址

static gpio_t *vir_beep;
static unsigned int *vir_rcc;

// 目录、设备信息
static struct class *cls;
static struct device *dev;

/**
 * @brief    初始化
 */
static int __init beep_init(void)
{
    int i;

    // 注册驱动
    major = register_chrdev(0, "beep", &fops);
    if (major < 0) {
        printk("字符设备驱动注册失败\n");
        return major;
    }
    printk("字符设备驱动注册成功%d\n", major);

    // 映射寄存器
    vir_beep = ioremap(PHY_BEEP_ADDR, sizeof(gpio_t));
    if (!vir_beep) {
        printk("蜂鸣器地址映射失败\n");
        return -EFAULT;
    }

    vir_rcc = ioremap(PHY_RCC, 4);
    if (vir_rcc == NULL) {
        printk("RCC 寄存器地址映射失败\n");
        return -EFAULT;
    }
    printk("寄存器地址映射成功\n");

    // RCC初始化
    (*vir_rcc) |= (0x1 << 1);
    
    // GPIO初始化 PB6
    (*vir_beep).moder &= (~(0X3 << 12));
    (*vir_beep).moder |= (0X1 << 12);
    (*vir_beep).otyper &= (~(0X1 << 6));
    (*vir_beep).ospeedr &= (~(0X3 << 12));
    (*vir_beep).pupdr &= (~(0X3 << 12));

    printk("寄存器初始化成功\n");

    // 自动注册设备
    cls = class_create(THIS_MODULE, "beep");
    if (!cls) {
        printk("向上提交目录失败\n");
        return -PTR_ERR(cls);
    }
    printk("向上提交目录成功\n");

    for (i = 0; i < 1; i++) {
        dev = device_create(cls, NULL, MKDEV(major, i), NULL, "beep%d", i);
        if (IS_ERR(dev)) {
            printk("向上设备节点失败\n");
            return -PTR_ERR(dev);
        }
    }
    printk("向上设备节点成功\n");

    printk(KERN_ALERT "beep mod installed\n");
    return 0;
}

/**
 * @brief    卸载
 */
static void __exit beep_exit(void)
{
    int i;

    // 注销驱动
    unregister_chrdev(major, "beep");
    printk("字符设备驱动 %d 注销成功\n", major);

    // 取消映射
    iounmap(vir_beep);
    iounmap(vir_rcc);
    printk("寄存器取消映射成功\n");

    // 自动卸载设备
    for (i = 0; i < 1; i++)
        device_destroy(cls, MKDEV(major, i));
    class_destroy(cls);

    printk(KERN_ALERT "beep mod removed\n");
}

/**
 * @brief    开
 */
int beep_open(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

/**
 * @brief    关
 */
int beep_close(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

/**
 * @brief    读
 */
ssize_t beep_read(struct file *file, char __user *ubuf, size_t size, loff_t *lof)
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
ssize_t beep_write(struct file *file, const char __user *ubuf, size_t size, loff_t *lof)
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

    return 0;
}

/**
 * @brief    GPIO控制
 */
long beep_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
    case MOTOR_ON: // 开
        vir_beep->odr |= (0x1 << 6);
        break;
    case MOTOR_OFF: // 关灯
        vir_beep->odr &= (~(0x1 << 6));
        break;
    }

    return 0;
}

module_init(beep_init);
module_exit(beep_exit);
MODULE_LICENSE("GPL");
