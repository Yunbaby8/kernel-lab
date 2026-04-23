#!/usr/bin/env python3
import os
import subprocess
import sys
import time
from pathlib import Path


# =========================
# 1. 路径与参数配置
# =========================
SRC_DIR = Path("/home/yunhe/allcode/linux-6.6.87-lab").resolve()
BUILD_DIR = Path("/home/yunhe/allcode/linux-6.6.87-build").resolve()
CONFIG_BACKUP = Path("/tmp/kernel_build_backup.config")
JOBS = os.cpu_count() or 1

step_times = {}
script_start_time = time.time()


# =========================
# 2. 通用输出与退出
# =========================
def format_elapsed(seconds: float) -> str:
    minutes = int(seconds // 60)
    secs = seconds % 60
    if minutes > 0:
        return f"{minutes}m {secs:.2f}s"
    return f"{secs:.2f}s"


def log(msg: str) -> None:
    print(f"[INFO] {msg}")


def error_exit(msg: str, code: int = 1) -> None:
    print(f"[ERROR] {msg}", file=sys.stderr)
    total_elapsed = time.time() - script_start_time
    print(f"[INFO] Total elapsed before exit: {format_elapsed(total_elapsed)}")
    sys.exit(code)


# =========================
# 3. 统一命令执行函数
# =========================
def run_command(step_name: str, cmd: list[str], cwd: Path | None = None) -> None:
    """
    统一执行命令：
    - 打印步骤名
    - 打印命令
    - 计时
    - 检查返回值
    - 出错即退出
    """
    print()
    print(f"========== {step_name} ==========")
    print("+", " ".join(str(x) for x in cmd))

    start = time.time()
    try:
        subprocess.run(
            cmd,
            cwd=str(cwd) if cwd else None,
            check=True
        )
    except subprocess.CalledProcessError as e:
        elapsed = time.time() - start
        step_times[step_name] = elapsed
        error_exit(f"{step_name} failed (exit code {e.returncode})")

    elapsed = time.time() - start
    step_times[step_name] = elapsed
    log(f"{step_name} finished in {format_elapsed(elapsed)}")


# =========================
# 4. 前置检查
# =========================
def check_source_tree() -> None:
    if not SRC_DIR.is_dir():
        error_exit(f"Source directory not found: {SRC_DIR}")

    makefile_path = SRC_DIR / "Makefile"
    if not makefile_path.is_file():
        error_exit(f"Makefile not found in source directory: {makefile_path}")


# =========================
# 5. 构建准备
# =========================
def backup_existing_build_config() -> None:
    step_name = "Step 0 Backup existing build .config"
    build_config = BUILD_DIR / ".config"

    if build_config.is_file():
        run_command(step_name, ["cp", str(build_config), str(CONFIG_BACKUP)])
    else:
        step_times[step_name] = 0.0
        log(f"{step_name} skipped: no existing build .config found")


def remove_old_build_dir() -> None:
    step_name = "Step 1 Remove old build directory"

    if BUILD_DIR.exists():
        run_command(step_name, ["rm", "-rf", str(BUILD_DIR)])
    else:
        step_times[step_name] = 0.0
        log(f"{step_name} skipped: build directory does not exist: {BUILD_DIR}")


def create_new_build_dir() -> None:
    run_command(
        "Step 2 Create new build directory",
        ["mkdir", "-p", str(BUILD_DIR)]
    )


def prepare_config() -> None:
    build_config = BUILD_DIR / ".config"

    if CONFIG_BACKUP.is_file():
        run_command(
            "Step 3.1 Restore .config from backup to build directory",
            ["cp", str(CONFIG_BACKUP), str(build_config)]
        )
    else:
        error_exit(
            "No .config available.\n"
            f"Checked backup: {CONFIG_BACKUP}"
        )

    run_command(
        "Step 3.2 Run olddefconfig",
        ["make", f"O={BUILD_DIR}", "-C", str(SRC_DIR), "olddefconfig"]
    )


# =========================
# 6. 正式编译
# =========================
def build_kernel() -> None:
    run_command(
        "Step 4 Build kernel",
        ["make", f"O={BUILD_DIR}", "-C", str(SRC_DIR), f"-j{JOBS}"]
    )


# =========================
# 7. 构建结果检查
# =========================
def check_build_outputs() -> tuple[Path, Path]:
    bzimage = BUILD_DIR / "arch" / "x86" / "boot" / "bzImage"
    vmlinux = BUILD_DIR / "vmlinux"

    if not bzimage.is_file():
        error_exit(f"Build finished but bzImage not found: {bzimage}")

    if not vmlinux.is_file():
        error_exit(f"Build finished but vmlinux not found: {vmlinux}")

    return bzimage, vmlinux


# =========================
# 8. 总结输出
# =========================
def print_summary(bzimage: Path, vmlinux: Path) -> None:
    total_elapsed = time.time() - script_start_time

    print()
    print("========== Build Summary ==========")
    for step_name, elapsed in step_times.items():
        print(f"{step_name}: {format_elapsed(elapsed)}")

    print(f"Total elapsed: {format_elapsed(total_elapsed)}")
    print(f"Build output directory: {BUILD_DIR}")
    print(f"bzImage: {bzimage}")
    print(f"vmlinux: {vmlinux}")
    print("===================================")


# =========================
# 9. 主流程
# =========================
def main() -> None:
    print("DEBUG: minute-format script loaded")
    log(f"Source directory: {SRC_DIR}")
    log(f"Build directory : {BUILD_DIR}")
    log(f"Parallel jobs   : {JOBS}")

    check_source_tree()
    backup_existing_build_config()
    remove_old_build_dir()
    create_new_build_dir()
    prepare_config()
    build_kernel()

    bzimage, vmlinux = check_build_outputs()
    print_summary(bzimage, vmlinux)


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        error_exit("Interrupted by user", code=130)