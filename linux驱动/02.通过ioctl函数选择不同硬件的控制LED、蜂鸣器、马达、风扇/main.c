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
    char buf[128] = { 0 };
    int select_dev;
    int select_opt;
    int quit = 0;

    // 开
    int led_fd = open("/dev/my_led0", O_RDWR);
    if (led_fd < 0) {
        printf("打开LED设备文件夹失败\n");
        exit(-1);
    }
    // int fan_fd;
    int fan_fd = open("/dev/fan0", O_RDWR);
    if (fan_fd < 0) {
        printf("打开风扇设备文件夹失败\n");
        exit(-1);
    }
    // int motor_fd;
    int motor_fd = open("/dev/motor0", O_RDWR);
    if (motor_fd < 0) {
        printf("打开马达设备文件夹失败\n");
        exit(-1);
    }
    // int beep_fd;
    int beep_fd = open("/dev/beep0", O_RDWR);
    if (beep_fd < 0) {
        printf("打开蜂鸣器设备文件夹失败\n");
        exit(-1);
    }

    while (1) {
        printf("请选择要控制的设备\n");
        printf("1.LED1\n");
        printf("2.LED2\n");
        printf("3.LED3\n");
        printf("4.风扇\n");
        printf("5.马达\n");
        printf("6.蜂鸣器\n");
        printf("7.退出\n");

        scanf("%d", &select_dev);
        if (select_dev == 7)
            exit(0);

        while (1) {
            int back = 0;
            printf("请选择操作 1.开 2.关 3.返回\n");
            scanf("%d", &select_opt);

            switch (select_opt) {
            case 1: // 开
                switch (select_dev) {
                case 1:
                case 2:
                case 3:
                    ioctl(led_fd, LED_ON, select_dev); // led
                    break;
                case 4:
                    ioctl(fan_fd, FAN_ON, select_dev); // 风扇
                    break;
                case 5:
                    ioctl(motor_fd, MOTOR_ON, select_dev); // 马达
                    break;
                case 6:
                    ioctl(beep_fd, BEEP_ON, select_dev); // 蜂鸣器
                    break;
                }
                break;
            case 2: // 关
                switch (select_dev) {
                case 1:
                case 2:
                case 3:
                    ioctl(led_fd, LED_OFF, select_dev); // led
                    break;
                case 4:
                    ioctl(fan_fd, FAN_OFF, select_dev); // 风扇
                    break;
                case 5:
                    ioctl(motor_fd, MOTOR_OFF, select_dev); // 马达
                    break;
                case 6:
                    ioctl(beep_fd, BEEP_OFF, select_dev); // 蜂鸣器
                    break;
                }
                break;
            case 3:
                back = 1;
                break;
            }

            if (back)
                break;
        }
    }

    // 关
    close(led_fd);
    close(fan_fd);

    return 0;
}