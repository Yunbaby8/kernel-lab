# kernel-lab

This repository contains helper scripts and a minimal initramfs used for my Linux kernel experiments.

## Contents

- `scripts/pack.sh`: pack initramfs into `initramfs.cpio`
- `scripts/runqemu.sh`: run QEMU with the built kernel
- `initramfs/`: minimal rootfs used for boot testing

## Completed experiments

- Built Linux 6.6.87 into a bootable bzImage
- Booted self-built kernel with QEMU
- Built a minimal initramfs and entered shell
- Modified kernel version suffix
- Added printk in start_kernel()

SRC_DIR：只放源码
BUILD_DIR：放 .config 和所有构建产物
源码目录里不要留 .config

/home/yunhe/allcode/
├── linux-6.6.87-lab/        # 内核源码仓库
├── linux-6.6.87-build/      # 内核构建输出目录
└── kernel-lab/
    └── scripts/             # 你自己的辅助脚本
