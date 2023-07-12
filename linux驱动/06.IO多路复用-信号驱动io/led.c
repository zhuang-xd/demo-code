#include "led.h"
#include "head.h"

// 设备树解析GPIO
struct device_node *dnode; // 设备树节点
struct gpio_desc *gpiono; // gpio编号 led1
struct gpio_desc *gpiono2; // gpio编号 led2
struct gpio_desc *gpiono3; // gpio编号 led3

// 注册设备驱动
struct cdev *cdev_ptr; // 字符设备驱动对象
dev_t devno; // 设备号
int major = 255; // 主设备号
int minor = 0; // 次设备号
struct class *cls_ptr; // 提交目录
struct device *dev_prt; // 设备节点信息

// 定时器相关
struct timer_list my_timer; // 创建定时器对象

/**
 * @brief    分步注册设备
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
 * @brief    删除设备注册
 */
static int rm_regist(void)
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

    return 0;
}

/**
 * @brief    定时器回调函数
 */
static void timer_callback(struct timer_list *timer){
    int ret;
    
    ret = gpiod_get_value(gpiono);
    switch (ret)
    {
    case 0:
        gpiod_set_value(gpiono, 1);
        break;
    case 1:
        gpiod_set_value(gpiono, 0);
        break;
    }

    // 重新启用定时器
    mod_timer(&my_timer, jiffies+HZ);
}

/**
 * @brief    初始化设备驱动
 */
static int __init led_init(void)
{
    int ret;

    // 注册程序
    ret = step_regist();
    if (ret)
        return ret;
    printk("分步注册设备驱动成功\n");

    // 解析设备树文件
    dnode = of_find_node_by_path("/my_leds");
    if (!dnode) {
        printk(KERN_ALERT "dnode is null\n");
        return -ENXIO;
    }
    printk("设备树节点名:%s\n", dnode->name);

    gpiono = gpiod_get_from_of_node(dnode, "led1", 0, GPIOD_OUT_LOW, NULL);
    gpiono2 = gpiod_get_from_of_node(dnode, "led2", 0, GPIOD_OUT_LOW, NULL);
    gpiono3 = gpiod_get_from_of_node(dnode, "led3", 0, GPIOD_OUT_LOW, NULL);

    // 初始化定时器
    my_timer.expires = jiffies + HZ; // 1秒定时
    timer_setup(&my_timer, timer_callback, 0); // 启动定时
    add_timer(&my_timer); // 添加定时

    return 0;
}

/**
 * @brief    卸载驱动
 */
static void __exit led_exit(void)
{
    // 删除设备驱动注册
    rm_regist();

    // 关闭定时器
    del_timer(&my_timer);

    // 释放gpio编号
    gpiod_put(gpiono);
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
    case 1: // led1
        if (cmd == LED_ON) {
            gpiod_set_value(gpiono, 1);
        } else if ( cmd == LED_OFF) {
            gpiod_set_value(gpiono, 0);
        }
        break;
    case 2: // led2
        if (cmd == LED_ON) {
           gpiod_set_value(gpiono2, 1);
        } else if ( cmd == LED_OFF) {
            gpiod_set_value(gpiono2, 0);
        }
        break;
    case 3: // led3
        if (cmd == LED_ON) {
           gpiod_set_value(gpiono3, 1);
        } else if ( cmd == LED_OFF) {
           gpiod_set_value(gpiono3, 0);
        }
        break;
    }

    return 0;
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");