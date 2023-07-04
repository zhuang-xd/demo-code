#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    // 开
    int fd = open("/dev/mycdev", O_RDWR);
    if (fd < 0) {
        printf("打开设备文件夹失败\n");
        exit(-1);
    }

    char buf[128] = { 0 };
    while (1) {
        // 键盘输入到buf，写入buf到内核
        printf("请选择 1.开灯 0.关灯\n");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = '\0';
        write(fd, buf, sizeof(buf));
    }

    // 关
    close(fd);

    return 0;
}
