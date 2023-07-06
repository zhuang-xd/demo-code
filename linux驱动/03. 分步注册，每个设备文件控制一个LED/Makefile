ARCH?=x86
MODNAME?=demo

ifeq ($(ARCH),arm)
KERNELDIR=/home/shelton/Workspaces/FSMP1A/stm32mp1-openstlinux-5.10-dunfell-mp1-21-11-17/sources/arm-ostl-linux-gnueabi/linux-stm32mp-5.10.61-stm32mp-r2-r0/linux-5.10.61
else
KERNELDIR=/lib/modules/$(shell uname -r)/build
endif

PWD:=$(shell pwd)

KBUILD_EXTRA_SYMBOLS+=/home/shelton/Workspaces/nfs/rootfs/study/day02/Module.symvers

all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean

obj-m=$(MODNAME).o