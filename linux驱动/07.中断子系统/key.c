#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/of_irq.h>
/**
 * 相关API
 * #include <linux/interrupt.h>
 * #include <linux/of_irq.h>
 * 1. 解析设备树节点 (名字、路径、compatible)
 *      static inline struct device_node *of_find_node_by_name(struct device_node *from,const char *name)
 * 2. 从设备树节点中获取获取软中断号
 *      static inline unsigned int irq_of_parse_and_map(struct device_node *dev,int index)
 * 3. 注册中断号
 *      request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags, const char *name, void *dev)
 * 4. 注销中断号
 *      extern const void *free_irq(unsigned int, void *);
 */

struct device_node *dnode;
unsigned int irqno[3];

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
    return IRQ_HANDLED;
}

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
        // ret = request_irq(irqno2, interrupt_handler, IRQF_TRIGGER_FALLING, "key2", NULL);
        ret = request_irq(irqno[i], interrupt_handler, IRQF_TRIGGER_FALLING, "key", (void *)i);
        if (ret) {
            printk(KERN_ALERT "request irq%d failed\n", i);
            return ret;
        }
        printk("request irq%d success\n", i);
    }

    return 0;
}

static void __exit my_key_exit(void)
{
    int i;
    // 注销中断号
    for (i = 0; i < 3; i++)
        free_irq(irqno[i], (void *)i);
}

module_init(my_key_init);
module_exit(my_key_exit);
MODULE_LICENSE("GPL");
