## 01.[GPIO-点亮led1-led2-led3](https://github.com/zhuang-xd/demo-code/tree/master/linux%E9%A9%B1%E5%8A%A8/01.GPIO-%E7%82%B9%E4%BA%AEled1-led2-led3)

![](assets/Video_20230704_080057_159.gif)

## 02.[通过ioctl函数选择不同硬件的控制LED、蜂鸣器、马达、风扇](https://github.com/zhuang-xd/demo-code/tree/master/linux%E9%A9%B1%E5%8A%A8/02.%E9%80%9A%E8%BF%87ioctl%E5%87%BD%E6%95%B0%E9%80%89%E6%8B%A9%E4%B8%8D%E5%90%8C%E7%A1%AC%E4%BB%B6%E7%9A%84%E6%8E%A7%E5%88%B6LED%E3%80%81%E8%9C%82%E9%B8%A3%E5%99%A8%E3%80%81%E9%A9%AC%E8%BE%BE%E3%80%81%E9%A3%8E%E6%89%87)

`main.c` 是应用层的程序编译成 `a.out` 来测试驱动

```shell
arm-linux-gnueabihf-gcc main.c
```

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

卸载驱动

```shell
./rmmod.sh
```

## 03.[分步注册驱动](https://github.com/zhuang-xd/demo-code/tree/master/linux%E9%A9%B1%E5%8A%A8/03.%20%E5%88%86%E6%AD%A5%E6%B3%A8%E5%86%8C%EF%BC%8C%E6%AF%8F%E4%B8%AA%E8%AE%BE%E5%A4%87%E6%96%87%E4%BB%B6%E6%8E%A7%E5%88%B6%E4%B8%80%E4%B8%AALED)

> 创建三个设备文件，每个设备文件和一个LED灯绑定，当操作这个设备文件时只能控制设备文件对应的这盏灯。
