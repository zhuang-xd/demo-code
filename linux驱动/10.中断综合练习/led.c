#include "led.h"
#include "head.h"

// 创建文件IO对象
struct file_operations fops = {
    .read = led_read,
    .unlocked_ioctl = led_ioctl,
};

// 读取相关
unsigned int number = 0; // 变量
unsigned int condition = 0;
wait_queue_head_t wq_head; //定义等待队列头 

// 注册设备驱动
struct cdev *cdev_ptr; // 字符设备驱动对象
dev_t devno; // 设备号
int major = 255; // 主设备号
int minor = 0; // 次设备号
struct class *cls_ptr; // 提交目录
struct device *dev_prt; // 设备节点信息

// 设备树解析GPIO
struct device_node *dnode; // 设备树父节点
struct gpio_desc *gpiono; // gpio编号 led1

// 中断相关
unsigned int irqno;
struct tasklet_struct tasklet; // 中断底半部任务队列对象

/**
 * @brief    分步注册设备
 */
static int step_regist(void)
{
    int ret;
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
    ret = alloc_chrdev_region(&devno, minor, 0, "my_led");
    if (ret) {
        printk("动态指定设备号失败\n");
        goto out2;
    }

    // 4.注册字符设备驱动对象  cdev_add
    ret = cdev_add(cdev_ptr, MKDEV(major, minor), 1);
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
    dev_prt = device_create(cls_ptr, NULL, MKDEV(major, 0), NULL, "my_led");
    if (IS_ERR(dev_prt)) {
        printk("向上提交节点信息失败\n");
        ret = -PTR_ERR(dev_prt);
        goto out5;
    }

    return 0;

out5:
    device_destroy(cls_ptr, MKDEV(major, 0));
    class_destroy(cls_ptr);
out4:
    cdev_del(cdev_ptr);
out3:
    unregister_chrdev_region(MKDEV(major, minor), 1);
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
    device_destroy(cls_ptr, MKDEV(major, 0));
    // 2.销毁目录  class_destroy
    class_destroy(cls_ptr);
    // 3.注销对象  cdev_del()
    cdev_del(cdev_ptr);
    // 4.释放设备号   unregister_chrdev_region()
    unregister_chrdev_region(MKDEV(major, minor), 1);
    // 5.释放对象空间  kfree()
    kfree(cdev_ptr);

    return 0;
}

/**
 * @brief    中断回调函数
 */
static irqreturn_t interrupt_handler(int irqno, void *dev_id)
{
    int ret;

    printk("key1 on pressed\n");

    ret = gpiod_get_value(gpiono);
    switch (ret) {
    case 0:
        gpiod_set_value(gpiono, 1);
        break;
    case 1:
        gpiod_set_value(gpiono, 0);
        break;
    }

    // 开启中断底半部
    tasklet_schedule(&tasklet);

    return IRQ_HANDLED;
}


/**
 * @brief    中断底半部任务队列回调函数
 */
static void tasklet_handler(struct tasklet_struct *tasklet)
{
    number = !number;
    condition = 1;
    wake_up_interruptible(&wq_head); //将进程唤醒
}


/**
 * @brief    初始化设备驱动
 */
static int __init mydev_init(void)
{
    int ret;

    // 注册字符设备文件
    ret = step_regist();
    if (ret)
        return ret;
    printk("分步注册设备驱动成功\n");

    // 解析设备树文件
    dnode = of_find_node_by_path("/my_devs");
    if (!dnode) {
        printk(KERN_ALERT "dnode is null\n");
        return -ENXIO;
    }
    printk("设备树节点名:%s\n", dnode->name);

    gpiono = gpiod_get_from_of_node(dnode, "led1", 0, GPIOD_OUT_LOW, NULL);

     // 等待队列相关
    init_waitqueue_head(&wq_head); //初始化等待队列头

    // 初始化任务队列
    tasklet_setup(&tasklet, tasklet_handler);

    // 中断初始化  
    irqno = irq_of_parse_and_map(dnode, 0); // 从设备树节点中获取获取软中断号
    if (!irqno) {
        printk(KERN_ALERT "can't find irq\n");
        return -ENOMEM;
    }
    printk("irq found! irqno = %d\n", irqno);

    ret = request_irq(irqno, interrupt_handler, IRQF_TRIGGER_FALLING, "key", NULL); // 注册中断号
    if (ret) {
        printk(KERN_ALERT "request irq failed\n");
        return ret;
    }
    printk("request irq success\n");

    return 0;
}

/**
 * @brief    卸载驱动
 */
static void __exit mydev_exit(void)
{

    // 注销中断号
    free_irq(irqno, NULL);

    // 删除字符设备文件
    rm_regist();

    // 释放gpio编号
    gpiod_put(gpiono);
}

// 读
ssize_t led_read(struct file *file, char *ubuf, size_t size, loff_t *lof)
{
    int ret;

    if(sizeof(number)<size)
        size=sizeof(number);
    wait_event_interruptible(wq_head,condition);//将进程切换为休眠
    ret=copy_to_user(ubuf,&number,size);
    if(ret)
    {
        printk("copy_tO_user filed\n");
        return -EIO;
    }
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    condition=0;//表示下一次硬件数据没有准备好
    return 0;
}

// GPIO控制
long led_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    // unsigned int dev_selected = (int)(file->private_data + 1);

    if (cmd == LED_ON)
        gpiod_set_value(gpiono, 1);
    else if (cmd == LED_OFF)
        gpiod_set_value(gpiono, 0);

    return 0;
}

module_init(mydev_init);
module_exit(mydev_exit);
MODULE_LICENSE("GPL");