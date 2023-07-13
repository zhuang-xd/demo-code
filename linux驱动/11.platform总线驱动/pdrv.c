#include <linux/init.h>
#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>

struct resource *res;
unsigned int irqno;

/**
 * @brief   probe函数，初始化设备
 */
int pdrev_probe(struct platform_device *pdev)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);

    // 获取设备信息
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (res == NULL) {
        printk("获取资源失败\n");
        return -ENXIO;
    }

    printk("获取资源信息成功 %llx\n", res->start);
    irqno = platform_get_irq(pdev, 0);
    if (irqno < 0) {
        printk("获取中断资源失败\n");
        return irqno;
    }
    printk("中断类型资源为%d\n", irqno);

    return 0;
}

/**
 * @brief    remove函数，销毁设备
 */
int pdrev_remove(struct platform_device *pdev)
{
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
    return 0;
}

// 构建设备树匹配的表
struct of_device_id of_table[] = {
    {
        .compatible = "hqyj,my_devs",
    },
    {},
};

// 定义平台驱动对象
struct platform_driver pdrev_driver = {
    .driver = {
        .name = "mycdev",
        .owner = THIS_MODULE,
    },
    .probe = pdrev_probe,
    .remove = pdrev_remove,
};

// static int __init mycdev_init(void)
// {
//     // 注册驱动信号对象
//     platform_driver_register(&pdrev_driver);
//     return 0;
// }

// static void __exit mycdev_exit(void)
// {
//     // 注销驱动信号对象
//     platform_driver_unregister(&pdrev_driver);
// }

// module_init(mycdev_init);
// module_exit(mycdev_exit);

// 一键注册宏
module_platform_driver(pdrev_driver);
MODULE_LICENSE("GPL");