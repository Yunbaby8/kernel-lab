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
