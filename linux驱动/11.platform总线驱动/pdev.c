#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>

/** 
* @brief    release函数
*/
void pdev_release(struct device *dev) {
    printk("%s:%s:%d\n", __FILE__, __func__, __LINE__);
}

// 资源数组
struct resource res[] = {
    [0] = {
        .start = 0x12345678,
        .end = 0x12345678,
        .flags = IORESOURCE_MEM
    },
    [1] = {
        .start = 71,
        .end = 71,
        .flags = IORESOURCE_IRQ
    },
};


// 定义并初始化平台设备pdev
struct platform_device pdev = {
    .name = "mycdev",
    .id = PLATFORM_DEVID_AUTO,
    .dev = {
        .release = pdev_release,
    },
    .resource = res,
    .num_resources = ARRAY_SIZE(res),
};


static int __init mycdev_init(void)
{
    // 注册平台设备
    platform_device_register(&pdev);
    
    return 0;
}
static void __exit mycdev_exit(void)
{

    // 注销平台设备
    platform_device_unregister(&pdev);
}

module_init(mycdev_init);
module_exit(mycdev_exit);
MODULE_LICENSE("GPL");