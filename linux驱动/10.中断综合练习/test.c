/**
 * 1. 应用程序通过阻塞的io模型来读取number变量的值
 * 2. number是内核驱动中的一个变量
 * 3. number的值随着按键按下而改变（按键中断）
 * 	    - 例如 number=o 按下按键 number=1，再次按下按键 number=0
 * 4. 在按下按键的时候需要同时将1ed1的状态取反
 * 5. 驱动中需要编写字符设备驱动
 * 6. 驱动中需要自动创建设备节点
 * 7. 这个驱动需要的所有设备信息放在设备树的同一个节点中
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/my_led"

int main(int argc, char const *argv[])
{
    unsigned int number = 0; 
    int fd = open(DEVICE_PATH, O_RDONLY);
    if (fd < 0) {
        printf("打开设备文件失败\n");
        exit(-1);
    }

    while (1) {
        read(fd, &number, sizeof(number)); // 读数据
        printf("number = %d\n", number);
    }

    close(fd);

    return 0;
}