#!/usr/bin/env bash
set -e

QEMU="qemu-system-x86_64"
BASE_DIR="$(cd "$(dirname "$0")/.." && pwd)"

KERNEL="$BASE_DIR/../linux-6.6.87-build/arch/x86/boot/bzImage"
INITRAMFS="$BASE_DIR/initramfs.cpio"

"$QEMU" \
  -kernel "$KERNEL" \
  -initrd "$INITRAMFS" \
  -nographic \
  -append "console=ttyS0"
