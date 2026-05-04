#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "mydev"
#define BUF_SIZE 128

static dev_t mydev_num;
static struct cdev mydev_cdev;

static char mydev_buf[BUF_SIZE];
static size_t mydev_len;

static int mydev_open(struct inode *inode, struct file *file)
{
    pr_info("mydev: open\n");
    return 0;
}

static ssize_t mydev_read(struct file *file,
                          char __user *user_buf,
                          size_t count,
                          loff_t *ppos)
{
    size_t len;

    pr_info("mydev: read called, count=%zu, ppos=%lld\n", count, *ppos);

    if (*ppos >= mydev_len)
        return 0;

    len = min(count, mydev_len - (size_t)*ppos);

    if (copy_to_user(user_buf, mydev_buf + *ppos, len))
        return -EFAULT;

    *ppos += len;

    pr_info("mydev: sent %zu bytes\n", len);

    return len;
}

static ssize_t mydev_write(struct file *file,
                           const char __user *user_buf,
                           size_t count,
                           loff_t *ppos)
{
    size_t len;

    pr_info("mydev: write called, count=%zu\n", count);

    len = min(count, (size_t)(BUF_SIZE - 1));

    if (copy_from_user(mydev_buf, user_buf, len))
        return -EFAULT;

    mydev_buf[len] = '\0';
    mydev_len = len;

    pr_info("mydev: received: %s\n", mydev_buf);

    return count;
}

static const struct file_operations mydev_fops = {
    .owner = THIS_MODULE,
    .open  = mydev_open,
    .read  = mydev_read,
    .write = mydev_write,
};

static int __init mydev_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&mydev_num, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("mydev: failed to allocate device number\n");
        return ret;
    }

    cdev_init(&mydev_cdev, &mydev_fops);
    mydev_cdev.owner = THIS_MODULE;

    ret = cdev_add(&mydev_cdev, mydev_num, 1);
    if (ret < 0) {
        pr_err("mydev: failed to add cdev\n");
        unregister_chrdev_region(mydev_num, 1);
        return ret;
    }

    pr_info("mydev: init\n");
    pr_info("mydev: major=%d minor=%d\n", MAJOR(mydev_num), MINOR(mydev_num));

    return 0;
}

static void __exit mydev_exit(void)
{
    cdev_del(&mydev_cdev);
    unregister_chrdev_region(mydev_num, 1);
    pr_info("mydev: exit\n");
}

module_init(mydev_init);
module_exit(mydev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eric Zhou");
MODULE_DESCRIPTION("Minimal character device read/write lab");
