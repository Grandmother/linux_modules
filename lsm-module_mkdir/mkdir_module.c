// Module is based on description from http://www.okbsapr.ru/kanner_2012_5.html

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/security.h>
#include <linux/dcache.h>

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Roman Kovtyukh <HelloDearGrandma@gmail.com>" );


// security_operations structure definition.
extern struct security_operations security_ops;


// MKDIR hook
static int secure_inode_mkdir( struct inode* dir, struct dentry* dentry, unsigned short mask )
{
    // May be is not safe to print the dir name here, but it is just for debug now.
    printk( KERN_INFO "Mkdir security create. Dirname: %s\n", dentry->d_name.name );

    return 0;
}

// security_operations structure.
static struct security_operations hook_security_ops = {
    .name = "mkdir_secure",
    .inode_mkdir = secure_inode_mkdir,
};

// hook REGISTRATOR
int mkdir_hook_register(void)
{
    int res = register_security( &hook_security_ops );

    if ( res )
    {
        printk( KERN_ERR "Error while mkdir security hook registration. Error code: %d.\n", res );
        return res;
    }
    printk( KERN_INFO "Mkdir security hook registered.\n" );

    return 0;
}

// hook UNREGISTRATOR
void mkdir_hook_unregister(void)
{
    //int res = unregister_security( &hook_security_ops );
    int res = 0;

    if ( res )
    {
        printk( KERN_ERR "Error while mkdir security hook unregistration. Error code: %d.\n", res );
        return;
    }
    printk( KERN_INFO "Mkdir security hook unregistered.\n" );
    return;

}

// module INIT
static __init int mkdir_init(void)
{
    if ( mkdir_hook_register() )
    {
        printk( KERN_INFO "Error while mkdir security initialization.\n" );
    }
}

// module EXIT
static __exit void mkdir_exit(void)
{
    mkdir_hook_unregister();
    printk( KERN_INFO "Mkdir security exit.\n" );
    return;
}


module_init( mkdir_init );
module_exit( mkdir_exit );
