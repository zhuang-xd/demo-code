## 01.GPIO-点亮led1-led2-led3

![](assets/Video_20230704_080057_159.gif)

## 02.通过ioctl函数选择不同硬件的控制LED、蜂鸣器、马达、风扇

生成 `.ko` 驱动文件

```shell
make ARCH=arm MODNAME=led
make ARCH=arm MODNAME=fan
make ARCH=arm MODNAME=motor
make ARCH=arm MODNAME=beep
```

安装驱动

```shell
./insmod.sh
```

`main.c` 是应用层的程序编译成 `a.out` 来测试驱动

```shell
arm-linux-gnueabihf-gcc main.c
```

卸载驱动

```shell
./rmmod.sh
```


