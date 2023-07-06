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
    int led_fd;
    int led2_fd;
    int led3_fd;
    int led_fds[] = { led_fd, led2_fd, led3_fd };
    int num_leds = sizeof(led_fds) / sizeof(led_fds[0]);

    while (1) {
        printf("请选择要控制的设备\n");
        printf("1.LED1\n");
        printf("2.LED2\n");
        printf("3.LED3\n");
        printf("4.退出\n");
        scanf("%d", &dev_selected);
        if (dev_selected == 4)
            break;

        if (dev_selected >= 1 && dev_selected <= num_leds) {
            printf("请选择操作 1.开  2.关  3.返回\n");
            scanf("%d", &opt_selected);
            if (opt_selected == 1)
                ioctl(led_fds[dev_selected - 1], LED_ON);
            else if (opt_selected == 2)
                ioctl(led_fds[dev_selected - 1], LED_OFF);
            else if (opt_selected == 3)
                continue;
        }
    }
}
