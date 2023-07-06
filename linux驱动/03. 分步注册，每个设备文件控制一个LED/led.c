#include "led.h"
#include "head.h"

static int step_regist(void);
static int mapping_register(void);
static int all_led_init(void);

// 虚拟地址
static gpio_t *vir_led1;
static gpio_t *vir_led2;
static gpio_t *vir_led3;
static unsigned int *vir_rcc;

struct cdev *cdev_ptr; // 字符设备驱动对象
dev_t devno; // 设备号
int major = 500; // 主设备号
int minor = 0; // 次设备号
struct class *cls_ptr; // 提交目录
struct device *dev_prt; // 设备节点信息

/**
 * @brief    安装驱动
 */
static int __init led_init(void)
{
    int ret;
    // 映射 LED 寄存器地址
    if (!mapping_register())
        printk("寄存器地址映射成功\n");

    // 初始化 LED 寄存器
    if (!all_led_init())
        printk("寄存器初始化成功\n");

    // 分步注册设备驱动
    ret = step_regist();
    if (!ret)
        printk("分步注册设备驱动成功\n");

    return ret;
}

/**
 * @brief    卸载驱动
 */
static void __exit led_exit(void)
{
    // 1.销毁设备信息  device_destroy
    int i;
    for (i = 0; i < 3; i++) {
        device_destroy(cls_ptr, MKDEV(major, i));
    }
    // 2.销毁目录  class_destroy
    class_destroy(cls_ptr);
    // 3.注销对象  cdev_del()
    cdev_del(cdev_ptr);
    // 4.释放设备号   unregister_chrdev_region()
    unregister_chrdev_region(MKDEV(major, minor), 3);
    // 5.释放对象空间  kfree()
    kfree(cdev_ptr);
}

// 开
int led_open(struct inode *inode, struct file *file)
{
    // 传递次设备号到file的private_data成员
    file->private_data = (void *)MINOR(inode->i_rdev);
    return 0;
}
// 关
int led_close(struct inode *inode, struct file *file)
{
    return 0;
}
// GPIO控制
long led_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int dev_selected = (int)(file->private_data + 1);

    switch (dev_selected) {
    case 1:
        (*vir_led1).odr ^= (0x1 << 10);
        break;
    case 2:
        (*vir_led2).odr ^= (0x1 << 10);
        break;
    case 3:
        (*vir_led3).odr ^= (0x1 << 8);
        break;
    }

    return 0;
}

/**
 * @brief    分步注册
 */
static int step_regist(void)
{
    int ret, i;
    // 1.分配字符设备驱动对象空间
    cdev_ptr = cdev_alloc();
    if (cdev_ptr == NULL) {
        printk("申请字符设备驱动对象空间失败\n");
        ret = -EFAULT;
        goto out;
    }

    // 2.字符设备驱动对象部分初始化
    cdev_init(cdev_ptr, &fops);

    // 3.申请设备号
    if (major > 0) { // 指定设备号
        ret = register_chrdev_region(MKDEV(major, minor), 3, "my_led");
        if (ret) {
            printk("静态指定设备号失败\n");
            goto out2;
        }
    } else { // 自动分配可用的设备号
        for (i = 0; i < 3; i++) {
            ret = alloc_chrdev_region(&devno, minor, i, "my_led");
            if (ret) {
                printk("动态指定设备号失败\n");
                goto out2;
            }
        }
        major = MAJOR(devno);
        minor = MINOR(devno);
    }

    // 4.注册字符设备驱动对象  cdev_add
    ret = cdev_add(cdev_ptr, MKDEV(major, minor), 3);
    if (ret) {
        printk("注册字符设备驱动对象失败\n");
        goto out3;
    }

    // 5.向上提交目录 class_create
    cls_ptr = class_create(THIS_MODULE, "my_led");
    if (IS_ERR(cls_ptr)) {
        printk("向上提交目录失败\n");
        ret = -PTR_ERR(cls_ptr);
        goto out4;
    }

    // 6.向上提交设备节点
    for (i = 0; i < 3; i++) {
        dev_prt = device_create(cls_ptr, NULL, MKDEV(major, i), NULL, "my_led%d", i);
        if (IS_ERR(dev_prt)) {
            printk("向上提交节点信息失败\n");
            ret = -PTR_ERR(dev_prt);
            goto out5;
        }
    }

    return 0;

out5:
    for (--i; i < 3; i--)
        device_destroy(cls_ptr, MKDEV(major, i));
    class_destroy(cls_ptr);
out4:
    cdev_del(cdev_ptr);
out3:
    unregister_chrdev_region(MKDEV(major, minor), 3);
out2:
    kfree(cdev_ptr);
out:
    return ret;
}

/**
 * @brief    寄存器地址映射
 */
static int mapping_register(void)
{
    // 映射寄存器
    vir_led1 = ioremap(PHY_LED1_ADDR, sizeof(gpio_t));
    if (!vir_led1) {
        printk("LED1 地址映射失败\n");
        return -EFAULT;
    }

    vir_led2 = ioremap(PHY_LED2_ADDR, sizeof(gpio_t));
    if (!vir_led2) {
        printk("LED2 地址映射失败\n");
        return -EFAULT;
    }

    vir_led3 = ioremap(PHY_LED3_ADDR, sizeof(gpio_t));
    if (!vir_led3) {
        printk("LED3 地址映射失败\n");
        return -EFAULT;
    }

    vir_rcc = ioremap(PHY_RCC, 4);
    if (vir_rcc == NULL) {
        printk("RCC 寄存器地址映射失败\n");
        return -EFAULT;
    }
    return 0;
}

/**
 * @brief    led初始化
 */
static int all_led_init(void)
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

    return 0;
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");