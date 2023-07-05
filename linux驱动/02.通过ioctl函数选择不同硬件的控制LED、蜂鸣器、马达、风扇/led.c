#include "led.h"
#include "head.h"

static void all_led_init(void);

static int major; // 保存主设备号
static char kbuf[128] = { 0 }; // 缓冲区

// 虚拟地址
static gpio_t *vir_led1;
static gpio_t *vir_led2;
static gpio_t *vir_led3;
static unsigned int *vir_rcc;

// 目录、设备信息
static struct class *cls;
static struct device *dev;

/**
 * @brief    初始化
 */
static int __init my_led_init(void)
{
    int i;

    // 注册驱动
    major = register_chrdev(0, "my_led", &fops);
    if (major < 0) {
        printk("字符设备驱动注册失败\n");
        return major;
    }
    printk("字符设备驱动注册成功%d\n", major);

    // 映射寄存器
    vir_led1 = ioremap(PHY_GPIOE, sizeof(gpio_t));
    if (!vir_led1) {
        printk("LED1 地址映射失败\n");
        return -EFAULT;
    }

    vir_led2 = ioremap(PHY_GPIOF, sizeof(gpio_t));
    if (!vir_led2) {
        printk("LED2 地址映射失败\n");
        return -EFAULT;
    }

    vir_led3 = vir_led1;
    if (!vir_led3) {
        printk("LED3 地址映射失败\n");
        return -EFAULT;
    }

    vir_rcc = ioremap(PHY_RCC, 4);
    if (vir_rcc == NULL) {
        printk("RCC 寄存器地址映射失败\n");
        return -EFAULT;
    }
    printk("寄存器地址映射成功\n");

    // 初始化湖村桥
    all_led_init();
    printk("寄存器初始化成功\n");

    // 自动注册设备
    cls = class_create(THIS_MODULE, "my_led");
    if (!cls) {
        printk("向上提交目录失败\n");
        return -PTR_ERR(cls);
    }
    printk("向上提交目录成功\n");

    for (i = 0; i < 3; i++) {
        dev = device_create(cls, NULL, MKDEV(major, i), NULL, "my_led%d", i);
        if (IS_ERR(dev)) {
            printk("向上设备节点失败\n");
            return -PTR_ERR(dev);
        }
    }
    printk("向上设备节点成功\n");

    printk(KERN_ALERT "led mod installed\n");
    return 0;
}

/**
 * @brief    卸载
 */
static void __exit my_led_exit(void)
{
    int i;

    // 注销驱动
    unregister_chrdev(major, "my_led");
    printk("字符设备驱动 %d 注销成功\n", major);

    // 取消映射
    iounmap(vir_led1);
    iounmap(vir_led2);
    iounmap(vir_rcc);
    printk("寄存器取消映射成功\n");

    // 自动卸载设备
    for (i = 0; i < 3; i++)
        device_destroy(cls, MKDEV(major, i));
    class_destroy(cls);

    printk(KERN_ALERT "led mod removed\n");
}

/**
 * @brief    开
 */
int my_led_open(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

/**
 * @brief    关
 */
int my_led_close(struct inode *inode, struct file *file)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

/**
 * @brief    读
 */
ssize_t my_led_read(struct file *file, char __user *ubuf, size_t size, loff_t *lof)
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
ssize_t my_led_write(struct file *file, const char __user *ubuf, size_t size, loff_t *lof)
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

    // // LED亮灭逻辑
    // if (kbuf[0] == '1') // led1
    //     (*vir_led1).odr ^= (0x1 << 10);
    // else if (kbuf[0] == '2') // led2
    //     (*vir_led2).odr ^= (0x1 << 10);
    // else if (kbuf[0] == '3') // led3
    //     (*vir_led1).odr ^= (0x1 << 8);

    return 0;
}

/**
 * @brief    GPIO控制
 */
long my_led_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
    case LED_ON: // 开灯
        switch (arg) {
        case 1: // led1
            (*vir_led1).odr |= (0x1 << 10);
            break;
        case 2: // led2
            (*vir_led2).odr |= (0x1 << 10);
            break;
        case 3: // led3
            (*vir_led3).odr |= (0x1 << 8);
            break;
        }
        break;
    case LED_OFF: // 关灯
        (*vir_led1).odr &= (~(0x1 << 10));
        (*vir_led2).odr &= (~(0x1 << 10));
        (*vir_led3).odr &= (~(0x1 << 8));
        break;
    }

    return 0;
}

/**
 * @brief    led初始化
 */
static void all_led_init(void)
{
    // RCC初始化
    (*vir_rcc) |= (0x3 << 4);

    // led1
    (*vir_led1).moder &= (~(0X3 << 20));
    (*vir_led1).moder |= (0X1 << 20);
    (*vir_led1).otyper &= (~(0X1 << 10));
    (*vir_led1).ospeedr &= (~(0X3 << 20));
    (*vir_led1).pupdr &= (~(0X3 << 20));

    // led2
    (*vir_led2).moder &= (~(0X3 << 20));
    (*vir_led2).moder |= (0X1 << 20);
    (*vir_led2).otyper &= (~(0X1 << 10));
    (*vir_led2).ospeedr &= (~(0X3 << 20));
    (*vir_led2).pupdr &= (~(0X3 << 20));

    // led3
    (*vir_led3).moder &= (~(0X3 << 16));
    (*vir_led3).moder |= (0X1 << 16);
    (*vir_led3).otyper &= (~(0X1 << 8));
    (*vir_led3).ospeedr &= (~(0X3 << 16));
    (*vir_led3).pupdr &= (~(0X3 << 16));
}

module_init(my_led_init);
module_exit(my_led_exit);
MODULE_LICENSE("GPL");
