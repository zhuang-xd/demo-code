#!/bin/sh

module_names=(
    "led.ko"
)

# 检查是否为root用户
if [ "$(id -u)" != "0" ]; then
    echo "需要以root权限运行此脚本，请使用sudo或切换到root用户。"
    exit 1
fi

# 遍历模块数组
for module_name in "${module_names[@]}"; do
    # 检查模块文件是否存在
    if [ ! -e "$module_name" ]; then
        echo "找不到模块文件 $module_name"
        continue
    fi

    # 执行insmod命令加载内核模块
    insmod "$module_name"

    # 检查insmod命令是否执行成功
    if [ $? -ne 0 ]; then
        echo "加载内核模块 $module_name 失败。"
        continue
    fi

    echo "内核模块 $module_name 已成功加载。"
done

exit 0