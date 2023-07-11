#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_gpio.h>

/**
 *  相关API
 *  #include <linux/gpio.h>
 *  1. of_find_node_by_path     解析设备树节点名 myled
 *  2. of_get_named_gpio        根据设备树解析gpio编号
 *  3. gpio_request             申请GPIO编号
 *  4. gpio_direction_output    设置gpio编号对应的gpio管脚为输出方向
 *     gpio_direction_input     设置gpio编号对应的gpio管脚为输入方向
 *  5. gpio_set_value           设置gpio输出高低电平
 *     gpio_get_value           获取gpio输出高低电平
 *  6. gpio_free                释放gpio编号
 */

struct device_node *dnode; // 设备树节点
unsigned int gpiono; // gpio编号

static int __init mycdev_init(void)
{
    int ret; // 返回结果

    printk(KERN_ALERT "mycdev_init\n");

    // 1. 解析设备树节点名 myled
    dnode = of_find_node_by_path("/my_leds");
    if (!dnode) {
        printk(KERN_ALERT "dnode is null\n");
        return -ENXIO;
    }
    printk("设备树节点名:%s\n", dnode->name);

    // 2. 解析gpio编号
    gpiono = of_get_named_gpio(dnode, "led1", 0);
    if (!gpiono) {
        printk(KERN_ALERT "gpio is null\n");
        return -ENXIO;
    }
    printk("gpio编号:%d\n", gpiono);

    // 3. 申请GPIO编号
    ret = gpio_request(gpiono, NULL);
    if (ret < 0) {
        printk("申请GPIO编号失败\n");
        return -ENXIO;
    }
    printk("申请GPIO编号成功\n");

    // 4. 设置gpio编号对应的gpio管脚为输出方向
    gpio_direction_output(gpiono, 1);

    // 5. 设置gpio输出高电平, 亮灯
    gpio_set_value(gpiono, 1);

    return 0;
}

static void __exit mycdev_exit(void)
{
    // 灭灯
    gpio_set_value(gpiono, 0);

    // 释放gpio编号
    gpio_free(gpiono);
}
module_init(mycdev_init);
module_exit(mycdev_exit);
MODULE_LICENSE("GPL");