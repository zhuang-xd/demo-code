#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
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
unsigned int irqno;
unsigned int irqno2;
unsigned int irqno3;

static irqreturn_t interrupt_handler(int irqno, void *dev_id)
{
    printk("interrupt_handler! irqno = %d\n", irqno);
    return IRQ_HANDLED;
}

static int __init my_key_init(void)
{
    int ret;

    // 解析设备树节点
    dnode = of_find_node_by_name(NULL,"my_keys");
    if(!dnode){
        printk(KERN_ALERT "can't find my_keys\n");
        return -ENXIO;
    }
    printk("my_keys found\n");

    // 从设备树节点中获取获取软中断号
    irqno = irq_of_parse_and_map(dnode, 0);
    if(!irqno){
        printk(KERN_ALERT "can't find irq_3\n");
        return -ENOMEM;
    }
    printk("irq found! irqno = %d\n", irqno);

    irqno2 = irq_of_parse_and_map(dnode, 1);
    if(!irqno2){
        printk(KERN_ALERT "can't find irq_3\n");
        return -ENOMEM;
    }
    printk("irq_2 found irqno2 = %d\n", irqno2);

    irqno3 = irq_of_parse_and_map(dnode, 2);
    if(!irqno3){
        printk(KERN_ALERT "can't find irq_3\n");
        return -ENOMEM;
    }
    printk("irq_3 found irqno3 = %d\n", irqno3);

    // 注册中断号
    ret = request_irq(irqno, interrupt_handler, IRQF_TRIGGER_FALLING, "key1", NULL);
    ret = request_irq(irqno2, interrupt_handler, IRQF_TRIGGER_FALLING, "key2", NULL);
    ret = request_irq(irqno3, interrupt_handler, IRQF_TRIGGER_FALLING, "key3", NULL);
    if(ret){
        printk(KERN_ALERT "request irq failed\n");
        return ret;
    }
    printk("request irq success\n");
    
    return 0;
}

static void __exit my_key_exit(void)
{
    // 注销中断号
    free_irq(irqno, NULL);
    free_irq(irqno2, NULL);
    free_irq(irqno3, NULL);
}

module_init(my_key_init);
module_exit(my_key_exit);
MODULE_LICENSE("GPL");



