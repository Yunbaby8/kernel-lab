#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>   // ⭐ 新增：copy_from_user
#include <linux/jiffies.h>
#include <linux/utsname.h>

#define PROC_NAME "hello_module"

static int myvalue = 0;

/* ---------- read 路径 ---------- */
static int hello_module_show(struct seq_file *m, void *v)
{
    seq_printf(m, "myvalue: %d\n", myvalue);
    return 0;
}

static int hello_module_open(struct inode *inode, struct file *file)
{
    return single_open(file, hello_module_show, NULL);
}

/* ---------- write 路径（核心新增） ---------- */
static ssize_t hello_module_write(struct file *file,
                                  const char __user *buffer,
                                  size_t count,
                                  loff_t *ppos)
{
    char buf[32] = {0};
    int value;

    /* 1. 从用户态拷贝数据 */
    if (count >= sizeof(buf))
        return -EINVAL;

    if (copy_from_user(buf, buffer, count))
        return -EFAULT;

    /* 2. 字符串转整数 */
    if (kstrtoint(buf, 10, &value))
        return -EINVAL;

    /* 3. 更新内核变量 */
    myvalue = value;

    pr_info("hello_module: myvalue updated to %d\n", myvalue);

    return count;
}

/* ---------- proc 操作表 ---------- */
static const struct proc_ops hello_module_proc_ops = {
    .proc_open    = hello_module_open,
    .proc_read    = seq_read,
    .proc_write   = hello_module_write,   // ⭐ 新增
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

/* ---------- 模块加载 ---------- */
static int __init hello_module_init(void)
{
    if (!proc_create(PROC_NAME, 0666, NULL, &hello_module_proc_ops)) {
        pr_err("hello_module: create proc failed\n");
        return -ENOMEM;
    }

    pr_info("hello_module: loaded\n");
    return 0;
}

/* ---------- 模块卸载 ---------- */
static void __exit hello_module_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("hello_module: unloaded\n");
}

module_init(hello_module_init);
module_exit(hello_module_exit);

MODULE_LICENSE("GPL");
