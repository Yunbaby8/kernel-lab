#!/usr/bin/env bash
set -e

INITRAMFS_DIR="/home/eric-zhou/kernel-lab/initramfs"
OUTPUT_IMG="/home/eric-zhou/kernel-lab/initramfs.cpio"

echo "[*] Packing initramfs..."

cd "$INITRAMFS_DIR"
find . | cpio -o --format=newc > "$OUTPUT_IMG"

echo "[+] Done: $OUTPUT_IMG"
