#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of_irq.h>
#include <linux/workqueue.h>
/**
 * 相关API
 * #include <linux/workqueue.h>
 * 1. 分配工作队列对象
 *      struct work_struct{}
 * 2. 初始化对象
 *       INIT_WORK(_work, _func)
 * 3. 设置一个工作队列
 *       schedule_work(_work);
 * 4. 注销工作队列
 *       cancel_work_sync(_work);
 */

struct device_node *dnode;
unsigned int irqno[3];
struct work_struct work; // 工作队列对象

/**
 * @brief    按键中断底半部回调
 */
static void work_callback(struct work_struct *tasklet)
{
    int i, sum;

    sum = 0;
    for (i = 1; i <= 100; i++) {
        sum += i;
        printk("i = %d, sum = %d\n", i, sum);
    }
}

/**
 * @brief    按键中断回调
 */
static irqreturn_t interrupt_handler(int irqno, void *dev_id)
{
    // dev_id 接收的是 request_irq 传递的第五个参数
    unsigned int arg = (unsigned int)dev_id;

    switch (arg) {
    case 0:
        printk("key1 interrupt");
        break;
    case 1:
        printk("key2 interrupt");
        break;
    case 2:
        printk("key3 interrupt");
        break;
    }

    // 开启中断底半部
    schedule_work(&work);

    return IRQ_HANDLED;
}

/**
 * @brief    初始化设备驱动
 */
static int __init my_key_init(void)
{
    int ret, i;

    // 解析设备树节点
    dnode = of_find_node_by_name(NULL, "my_keys");
    if (!dnode) {
        printk(KERN_ALERT "can't find my_keys\n");
        return -ENXIO;
    }
    printk("my_keys found\n");

    for (i = 0; i < 3; i++) {

        // 从设备树节点中获取获取软中断号
        irqno[i] = irq_of_parse_and_map(dnode, i);
        if (!irqno[i]) {
            printk(KERN_ALERT "can't find irq%d\n", i);
            return -ENOMEM;
        }
        printk("irq%d found! irqno = %d\n", i, irqno[i]);

        // 注册中断号
        ret = request_irq(irqno[i], interrupt_handler, IRQF_TRIGGER_FALLING, "key", (void *)i);
        if (ret) {
            printk(KERN_ALERT "request irq%d failed\n", i);
            return ret;
        }
        printk("request irq%d success\n", i);
    }

    // 初始化任务队列
    INIT_WORK(&work, work_callback);

    return 0;
}

/**
 * @brief    移除设备驱动
 */
static void __exit my_key_exit(void)
{
    // 注销工作队列
    cancel_work_sync(&work);

    int i;
    // 注销中断号
    for (i = 0; i < 3; i++)
        free_irq(irqno[i], (void *)i);
}

module_init(my_key_init);
module_exit(my_key_exit);
MODULE_LICENSE("GPL");
