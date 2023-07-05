## 01.[GPIO-点亮led1-led2-led3](https://github.com/zhuang-xd/demo-code/tree/master/linux%E9%A9%B1%E5%8A%A8/01.GPIO-%E7%82%B9%E4%BA%AEled1-led2-led3)

![](assets/Video_20230704_080057_159.gif)

## 02.[通过ioctl函数选择不同硬件的控制LED、蜂鸣器、马达、风扇](https://github.com/zhuang-xd/demo-code/tree/master/linux%E9%A9%B1%E5%8A%A8/02.%E9%80%9A%E8%BF%87ioctl%E5%87%BD%E6%95%B0%E9%80%89%E6%8B%A9%E4%B8%8D%E5%90%8C%E7%A1%AC%E4%BB%B6%E7%9A%84%E6%8E%A7%E5%88%B6LED%E3%80%81%E8%9C%82%E9%B8%A3%E5%99%A8%E3%80%81%E9%A9%AC%E8%BE%BE%E3%80%81%E9%A3%8E%E6%89%87)

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
