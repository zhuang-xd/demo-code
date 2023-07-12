#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>

/**
 *  相关API（新版）
 *  #include <linux/gpio.h>
 *  1. of_find_node_by_path     解析设备树节点名 myled
 *  2. struct gpio_desc *gpiod_get_from_of_node
 *                              根据设备树解析gpio编号
 *                                      1. 申请GPIO编号
 *                                      2. 设置gpio输出高低电平
 *  3. gpiod_set_value          设置gpio输出高低电平
 *  4. gpiod_put                释放gpio编号
 */

struct device_node *dnode; // 设备树节点
struct gpio_desc *gpiono; // gpio编号

static int __init mycdev_init(void)
{
    // 1. 解析设备树节点名 myled
    printk(KERN_ALERT "mycdev_init\n");

    dnode = of_find_node_by_path("/my_leds");
    if (!dnode) {
        printk(KERN_ALERT "dnode is null\n");
        return -ENXIO;
    }
    printk("设备树节点名:%s\n", dnode->name);

    gpiono = gpiod_get_from_of_node(
        dnode,
        "led1",
        0,
        GPIOD_OUT_LOW,
        NULL);

    gpiod_set_value(gpiono, 1);

    return 0;
}

static void __exit mycdev_exit(void)
{
    // 灭灯
    gpiod_set_value(gpiono, 0);

    // 释放gpio编号
    gpiod_put(gpiono);
}
module_init(mycdev_init);
module_exit(mycdev_exit);
MODULE_LICENSE("GPL");