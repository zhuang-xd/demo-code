#include "fan.h"
#include "head.h"

static int major; // 保存主设备号
static char kbuf[128] = { 0 }; // 缓冲区

// 虚拟地址

static gpio_t *vir_fan;
static unsigned int *vir_rcc;

// 目录、设备信息
static struct class *cls;
static struct device *dev;

/**
 * @brief    初始化
 */
static int __init fan_init(void)
{
    int i;

    // 注册驱动
    major = register_chrdev(0, "fan", &fops);
    if (major < 0) {
        printk("字符设备驱动注册失败\n");
        return major;
    }
    printk("字符设备驱动注册成功%d\n", major);

    // 映射寄存器
    vir_fan = ioremap(PHY_FAN_ADDR, sizeof(gpio_t));
    if (!vir_fan) {
        printk("风扇地址映射失败\n");
        return -EFAULT;
    }

    vir_rcc = ioremap(PHY_RCC, 4);
    if (vir_rcc == NULL) {
        printk("RCC 寄存器地址映射失败\n");
        return -EFAULT;
    }
    printk("寄存器地址映射成功\n");

    // RCC初始化
    (*vir_rcc) |= (0x1 << 4);

    // GPIOE初始化
    (*vir_fan).moder &= (~(0X3 << 18));
    (*vir_fan).moder |= (0X1 << 18);
    (*vir_fan).otyper &= (~(0X1 << 9));
    (*vir_fan).ospeedr &= (~(0X3 << 18));
    (*vir_fan).pupdr &= (~(0X3 << 18));
    printk("寄存器初始化成功\n");

    // 自动注册设备
    cls = class_create(THIS_MODULE, "fan");
    if (!cls) {
        printk("向上提交目录失败\n");
        return -PTR_ERR(cls);
    }
    printk("向上提交目录成功\n");

    for (i = 0; i < 1; i++) {
        dev = device_create(cls, NULL, MKDEV(major, i), NULL, "fan%d", i);
        if (IS_ERR(dev)) {
            printk("向上设备节点失败\n");
            return -PTR_ERR(dev);
        }
    }
    printk("向上设备节点成功\n");

    printk(KERN_ALERT "fan mod installed\n");
    return 0;
}

/**
 * @brief    卸载
 */
static void __exit fan_exit(void)
{
    int i;

    // 注销驱动
    unregister_chrdev(major, "fan");
    printk("字符设备驱动 %d 注销成功\n", major);

    // 取消映射
    iounmap(vir_fan);
    iounmap(vir_rcc);
    printk("寄存器取消映射成功\n");

    // 自动卸载设备
    for (i = 0; i < 1; i++)
        device_destroy(cls, MKDEV(major, i));
    class_destroy(cls);

    printk(KERN_ALERT "fan mod removed\n");
}

/**
 * @brief    开
 */
int fan_open(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

/**
 * @brief    关
 */
int fan_close(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

/**
 * @brief    读
 */
ssize_t fan_read(struct file *file, char __user *ubuf, size_t size, loff_t *lof)
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
ssize_t fan_write(struct file *file, const char __user *ubuf, size_t size, loff_t *lof)
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
long fan_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
    case FAN_ON: // 开
        vir_fan->odr |= (0x1 << 9);
        break;
    case FAN_OFF: // 关灯
        vir_fan->odr &= (~(0x1 << 9));
        break;
    }

    return 0;
}

module_init(fan_init);
module_exit(fan_exit);
MODULE_LICENSE("GPL");
