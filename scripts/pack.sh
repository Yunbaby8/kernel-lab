#!/usr/bin/env bash
set -e

BASE_DIR="$(cd "$(dirname "$0")/.." && pwd)"

INITRAMFS_DIR="$BASE_DIR/initramfs"
OUTPUT_IMG="$BASE_DIR/initramfs.cpio"
echo "[*] Packing initramfs..."

cd "$INITRAMFS_DIR"
find . | cpio -o --format=newc > "$OUTPUT_IMG"

echo "[+] Done: $OUTPUT_IMG"
