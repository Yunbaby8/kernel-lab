#!/usr/bin/env bash
set -e

QEMU="/home/yunhe/qemu/build/qemu-system-x86_64"
KERNEL="linux-6.6.87/arch/x86/boot/bzImage"
INITRAMFS="initramfs.cpio"

"$QEMU" \
  -kernel "$KERNEL" \
  -initrd "$INITRAMFS" \
  -nographic \
  -append "console=ttyS0"
