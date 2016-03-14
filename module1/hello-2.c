/*
        hello-2.c - Demonstrating the module_init() and module_exit() macros.
         This is preferred over using init_module() and cleanup_module().
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Roman Kovtyukh <HelloDearGrandma@gmail.com>");
MODULE_DESCRIPTION("Just a module");
MODULE_SUPPORTED_DEVICE("MITSUMI KFK-EA4XT");


static short int myshort = 1;
static int myint = 420;
static long int mylong = 9999;
static char* mystring = "blah-blah...";
static int myarray[2] = {-1, -3};
static int arr_argc = 0;

/*
    module_param(foo, int, 0000);
    the first param is the parameters name
    the second param is it's data type
    the final argument is the permissions bits,
    for exposing parameters in sysfs (if non-zero) at a later stage
*/

module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP );
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
MODULE_PARM_DESC(myint, "An integer parameter");
module_param(mylong, long, S_IRUSR );
MODULE_PARM_DESC(mylong, "A long integer");
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A string parameter");

/*
    module_param_array(name, type, num, perm)
    The first param is the parameter's (in this case array's) name
    the second param is the data type of the elements of the array
    the third argument is a pointer to the variable that will store the number
    of the elements of the array initialized by the user at module loading time
    The fourth argument is the permissions bits
 */
module_param_array(myarray, int, &arr_argc, 0000 );
MODULE_PARM_DESC(myarray, "an array of integers");

static int hello3_data __initdata = 22;

static int __init hello_init(void)
{
    int i = 0;
    printk(KERN_INFO "Hello world 5\n");
    printk(KERN_INFO "myshort is a short integer: %hd\n", myshort);
    printk(KERN_INFO "myint is an integer: %d\n", myint);
    printk(KERN_INFO "mylong is a long integer: %ld\n", mylong);
    printk(KERN_INFO "mystring is a string: %s\n", mystring);
    for (i = 0; i < (sizeof myarray / sizeof (int)); i++)
    {
        printk(KERN_INFO "myintArray[%d] = %d\n", i, myarray[i]);
    }
    printk(KERN_INFO "got %d arguments for myintArray.\n", arr_argc);
    printk(KERN_INFO "hello 3.0 data: %d", hello3_data);
    return 0;
}

static void __exit hello_bye(void)
{
    printk(KERN_INFO "See you in hello-3!");
}

module_init(hello_init);
module_exit(hello_bye);

