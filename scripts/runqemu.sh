#!/usr/bin/env bash
set -e

QEMU="qemu-system-x86_64"
KERNEL="/home/eric-zhou/linux-6.6.87-lab/arch/x86/boot/bzImage"
INITRAMFS="/home/eric-zhou/kernel-lab/initramfs.cpio"

"$QEMU" \
  -kernel "$KERNEL" \
  -initrd "$INITRAMFS" \
  -nographic \
  -append "console=ttyS0"
