#include <linux/kernel.h>
#include <linux/module.h>

static void __exit separated_exit(void)
{
    printk(KERN_INFO "Separated exit says goodbye to you.\n");
}

module_exit(separated_exit);
