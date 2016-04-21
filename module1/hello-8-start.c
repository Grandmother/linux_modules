#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Kovtyukh <HelloDearGrandma@gmail.com>");

static int __init separated_init(void)
{
    printk(KERN_INFO "Separated start function is our future!!\n");
    return 0;
}

module_init(separated_init);
