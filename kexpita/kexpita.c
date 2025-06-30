#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/bitops.h>
#include <linux/io.h>
#include <linux/goldfish.h>
#include <linux/mm.h>
#include <linux/acpi.h>
#include <linux/init.h>
#include <linux/printk.h>

#include "kexpita.h"


/********* Init Driver *********/
#define DEVICE_NAME "kexpita"
#define BUFFER_SIZE 0x400

char *g_buf = NULL;


MODULE_DESCRIPTION("Kernel Module EPITA");
MODULE_LICENSE("GPL");

static const struct file_operations kexpita_fops = {
    .owner      =   THIS_MODULE,
    .open       =   kexpita_open,
    .release    =   kexpita_release,
    .write      =   kexpita_write,
    .read       =   kexpita_read,
};

static struct miscdevice kexpita_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,    
    .fops = &kexpita_fops,
};

/*******************************/

static ssize_t kexpita_read(struct file *file,
  char __user *buf, size_t count,
  loff_t *f_pos)
{
  char kbuf[BUFFER_SIZE] = { 0 };

  printk(KERN_INFO "module_read called\n");

  memcpy(kbuf, g_buf, BUFFER_SIZE);
  if (_copy_to_user(buf, kbuf, count)) {
    printk(KERN_INFO "copy_to_user failed\n");
    return -EINVAL;
  }

  return count;
}

static ssize_t kexpita_write(struct file *file,
      const char __user *buf, size_t count,
      loff_t *f_pos)
{
  char kbuf[BUFFER_SIZE] = { 0 };

  printk(KERN_INFO "module_write called\n");

  if (_copy_from_user(kbuf, buf, count)) {
    printk(KERN_INFO "copy_from_user failed\n");
    return -EINVAL;
  }
  memcpy(g_buf, kbuf, BUFFER_SIZE);

  return count;
}



/********* Init Driver *********/
int kexpita_init(void) {
    int err = 0;

    pr_info("kexpita::kexpita_init \n");
    err = misc_register(&kexpita_miscdev);
    if(err) {
        pr_err("Unable to register kexpita driver\n");
        return err;
    }

    return 0;
}

void kexpita_exit(void) {
    pr_info("kexpita::exit\b");
    misc_deregister(&kexpita_miscdev);
}

int kexpita_open(struct inode *inode, struct file *file){
    pr_info("kexpita::open\n");
    g_buf = kzalloc(BUFFER_SIZE, GFP_KERNEL);
    printk("kzalloc(0x400, GFP_KERNEL) : 0x%px\n", g_buf);
    if (!g_buf) {
        printk(KERN_INFO "kmalloc failed");
        return -ENOMEM;
    }

    return 0;
}

int kexpita_release(struct inode *inode, struct file *file){
    pr_info("kexpita::release\n");
    kfree(g_buf);
    g_buf = NULL;
    return 0;
}

module_init(kexpita_init);
module_exit(kexpita_exit);