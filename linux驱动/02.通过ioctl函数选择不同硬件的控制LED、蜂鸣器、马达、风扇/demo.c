#include <stdio.h>
#include <stdlib.h>

int main()
{
    int select_dev;
    int select_opt;

    while (1) {
        printf("请选择要控制的设备\n");
        printf("1.灯\n");
        printf("2.风扇\n");
        printf("3.马达\n");
        printf("4.蜂鸣器\n");
        printf("5.退出\n");

        scanf("%d", &select_dev);
        if (select_dev == 5)
            exit(0);

        while (1) {
            printf("请选择操作 1.开 2.关 3.返回\n");
            scanf("%d", &select_opt);
            if (select_opt == 1) {
                printf("开\n");
            } else if (select_opt == 2) {
                printf("关\n");
            } else if (select_opt == 3) {
                break;
            }
        }
    }

    return 0;
}