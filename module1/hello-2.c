/*
        hello-2.c - Demonstrating the module_init() and module_exit() macros.
         This is preferred over using init_module() and cleanup_module().
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define MODULE_ATHR "Roman Kovtyukh <HelloDearGrandma@gmail.com>"
#define DRIVER_DESC "Just a module"

static int hello3_data __initdata = 22;

static int __init hello_init(void)
{
    printk(KERN_INFO "hello 3.0 data: %d", hello3_data);
    return 0;
}

static void __exit hello_bye(void)
{
    printk(KERN_INFO "See you in hello-3!");
}

module_init(hello_init);
module_exit(hello_bye);

MODULE_LICENSE("GPL");

MODULE_AUTHOR(MODULE_ATHR);
MODULE_DESCRIPTION(DRIVER_DESC);

MODULE_SUPPORTED_DEVICE("MITSUMI KFK-EA4XT");
