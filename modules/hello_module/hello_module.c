#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/jiffies.h>
#include <linux/utsname.h>
#include <linux/errno.h>

#define PROC_NAME "hello_module"

static int myvalue = 0;
static char *name = "default";

module_param(myvalue, int, 0644);
MODULE_PARM_DESC(myvalue, "An integer value passed to hello_module");

module_param(name, charp, 0644);
MODULE_PARM_DESC(name, "A string name passed to hello_module");

static int hello_module_show(struct seq_file *m, void *v)
{
    seq_printf(m, "Hello from hello_module\n");
    seq_printf(m, "myvalue: %d\n", myvalue);
    seq_printf(m, "name: %s\n", name);
    seq_printf(m, "jiffies: %lu\n", jiffies);
    seq_printf(m, "release: %s\n", init_uts_ns.name.release);

    return 0;
}

static int hello_module_open(struct inode *inode, struct file *file)
{
    return single_open(file, hello_module_show, NULL);
}

static const struct proc_ops hello_module_proc_ops = {
    .proc_open    = hello_module_open,
    .proc_read    = seq_read,
    .proc_lseek   = seq_lseek,
    .proc_release = single_release,
};

static int __init hello_module_init(void)
{
    if (myvalue < 0) {
        pr_err("hello_module: invalid myvalue=%d, must be >= 0\n", myvalue);
        return -EINVAL;
    }

    if (!proc_create(PROC_NAME, 0444, NULL, &hello_module_proc_ops)) {
        pr_err("hello_module: failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    pr_info("hello_module: loaded\n");
    pr_info("hello_module: myvalue=%d\n", myvalue);
    pr_info("hello_module: name=%s\n", name);
    pr_info("hello_module: /proc/%s created\n", PROC_NAME);

    return 0;
}

static void __exit hello_module_exit(void)
{
    remove_proc_entry(PROC_NAME, NULL);
    pr_info("hello_module: /proc/%s removed\n", PROC_NAME);
    pr_info("hello_module: unloaded\n");
}

module_init(hello_module_init);
module_exit(hello_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eric Zhou");
MODULE_DESCRIPTION("A hello module with procfs entry and parameters");
