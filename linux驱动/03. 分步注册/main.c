#include "head.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int dev_selected;
    int opt_selected;
    int led_fds[3];
    int back;
    int quit;

    // 开
    led_fds[0] = open("/dev/my_led0", O_RDWR);
    if (led_fds[0] < 0) {
        printf("打开设备文件失败\n");
        exit(-1);
    }
    led_fds[1] = open("/dev/my_led1", O_RDWR);
    if (led_fds[1] < 0) {
        printf("打开设备文件失败\n");
        exit(-1);
    }
    led_fds[2] = open("/dev/my_led2", O_RDWR);
    if (led_fds[2] < 0) {
        printf("打开设备文件失败\n");
        exit(-1);
    }

    while (1) {
        quit = 0;
        printf("请选择要控制的设备\n");
        printf("1.LED1\n");
        printf("2.LED2\n");
        printf("3.LED3\n");
        printf("4.退出\n");
        scanf("%d", &dev_selected);

        switch (dev_selected) {
        case 1:
        case 2:
        case 3:
            while (1) {
                back = 0;
                printf("请选择操作 1.开  2.关  3.返回\n");
                scanf("%d", &opt_selected);

                switch (opt_selected) {
                case 1:
                    ioctl(led_fds[dev_selected - 1], LED_ON);
                    break;
                case 2:
                    ioctl(led_fds[dev_selected - 1], LED_OFF);
                    break;
                case 3:
                    back = 1;
                    break;
                }

                if (back)
                    break;
            }
            break;
        case 4:
            quit = 1;
            break;
        default:
            printf("非法输入， 请重写输入\n");
            break;
        }

        if (quit)
            break;
    }

    // 关
    for (int i = 0; i < 3; i++)
        close(led_fds[i]);

    return 0;
}