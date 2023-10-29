#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

// We are going to crete two minor devices in this exercise
#define MAX_DEVICE_NUM 2

static int chdev_open(struct inode *inode, struct file *file);
static int chdev_release(struct inode *inode, struct file *file);
static long chdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t chdev_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t chdev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);

// 1. TODO: initialize file operations by replacing ... with the appropriate functions.
// hint: you have to use the functions written in this code
static const struct file_operations chdev_file_ops = {
    .owner      = THIS_MODULE,
    .open       = chdev_open,
    .release    = chdev_release, 
    .unlocked_ioctl = chdev_ioctl,
    .read       = chdev_read,
    .write       = chdev_write
};

// device data holder, this structure may be extended to hold additional data
struct mychar_device_data {
    struct cdev cdev;
    int owner;
};


// global storage for device major number
static int dev_major = 0;

// sysfs class structure
static struct class *chdev_class = NULL;

// array of mychar_device_data for our minor devices
static struct mychar_device_data chdev_data[MAX_DEVICE_NUM];

// configure UDEV variables to set correct permissions to the character device
static int chdev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

static int __init chdev_init(void)
{
    // declare variables
    int err, i;
    dev_t dev;

    // 2. TODO: allocate chardev region and assign Major number
    // hint: search for alloc_chrdev_region function. Use 0 as the base and "chdev" as the name.
    err = alloc_chrdev_region(&dev, 0, MAX_DEVICE_NUM, "chdev");

    // 3. TODO: generate and set the major number
    // hint: use and search for MAJOR macro
    dev_major = MAJOR(dev);

    // 4. TODO: create sysfs class
    // hint: search for class_create function. Use "chdev" for the name.
    chdev_class = class_create(THIS_MODULE, "chdev");
 
    // setting permissions
    chdev_class->dev_uevent = chdev_uevent;

    // create each minor device
    for (i = 0; i < MAX_DEVICE_NUM; i++) {
        
        // 5. TODO: initialize new device
        // hint: search for cdev_init function.
        cdev_init(&chdev_data[i].cdev, &chdev_file_ops);

        // 6. TODO: set the owner of the new device
        // hint: use the chdev array
        chdev_data[i].owner = dev_major;

        // 7. TODO: add device to the system where "i" is the minor number of the new device
        // hint: search for cdev_add function and MKDEV macro.
        cdev_add(&chdev_data[i].cdev, &dev, i);
        MKDEV(dev_major, i);

        // 8. TODO: create device node /dev/chdev-x where "x" should be equal to the minor number
        // hint: search for device_create function
        device_create(chdev_class, NULL, dev, NULL, "chdev-%d", i);
    }
    return 0;
}


static void __exit chdev_exit(void)
{
    int i;
 
    // delete each minor device
    for (i = 0; i < MAX_DEVICE_NUM; i++) {
        // 9. TODO: add the right parameters to the device_destroy function. 
        // hint: search for device_destroy function
        device_destroy(chdev_class, i);
    }

    class_unregister(chdev_class);
    class_destroy(chdev_class);

    unregister_chrdev_region(MKDEV(dev_major, 0), MINORMASK);
}

static int chdev_open(struct inode *inode, struct file *file)
{
    printk("Aaltodriver: CHDEV: Device open\n");
    return 0;
}

static int chdev_release(struct inode *inode, struct file *file)
{
    printk("Aaltodriver: CHDEV: Device close\n");
    return 0;
}

static long chdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    printk("Aaltodriver: CHDEV: Device ioctl\n");
    return 0;
}

static ssize_t chdev_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    uint8_t *data = "Aaltodriver: Hello from the kernel world!\n";
    size_t datalen = strlen(data);

    printk("Aaltodriver: Reading device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));

    if (count > datalen) {
        count = datalen;
    }

    if (copy_to_user(buf, data, count)) {
        return -EFAULT;
    }

    return count;
}

static ssize_t chdev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
    size_t maxdatalen = 15, ncopied;
    uint8_t databuf[maxdatalen];

    printk("Aaltodriver: Writing device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));

    if (count < maxdatalen) {
        maxdatalen = count;
    }
 
    ncopied = copy_from_user(databuf, buf, maxdatalen);

    if (ncopied == 0) {
        printk("Aaltodriver: Copied %zd bytes from the user\n", maxdatalen);
    } else {
        printk("Aaltodriver: Couldn't copy %zd bytes from the user\n", ncopied);
    }

    databuf[maxdatalen] = 0;

    printk("Aaltodriver: Data from the user: %s\n", databuf);

    return count;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Po-Sheng Cheng");

module_init(chdev_init);
module_exit(chdev_exit);

