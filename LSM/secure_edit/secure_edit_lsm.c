#include "hooks.h"
#include "checks.h"

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Roman Kovtyukh <HelloDearGrandma@gmail.com>" );

/*
 * Some structures definitions.
 */

struct task_struct;

// security_operations structure.
static struct security_operations hook_security_ops = {
    .name = "sec_edit",
    .path_mkdir = secure_path_mkdir,
    .path_unlink = secure_path_unlink,
    .path_rmdir = secure_path_rmdir,
    // .path_truncate = secure_path_truncate,
    .path_symlink = secure_path_symlink,
    .path_link = secure_path_link,
    .path_rename = secure_path_rename,
    // .path_chmod = secure_path_chmod,
    // .path_chown = secure_path_chown,
};


// hook REGISTRATOR
int mkdir_hook_register(void)
{
    int res = register_security( &hook_security_ops );

    if ( res )
    {
        printk( KERN_ALERT "Error while mkdir security hook registration. Error code: %d.\n", res );
        return res;
    }
    printk( KERN_INFO "Mkdir security hook registered.\n" );

    return 0;
}

// module INIT
static __init int mkdir_init(void)
{
    block_user(7343);
    block_group(100);
    block_path("/home/grandma/readexec/*");

    if ( mkdir_hook_register() )
    {
        printk( KERN_INFO "Error while mkdir security initialization.\n" );
    }
    return 0;
}

// module EXIT
static __exit void mkdir_exit(void)
{
    destroy_blocked_users();
    destroy_blocked_groups();
    destroy_blocked_globs();

	reset_security_ops();
    printk( KERN_INFO "Mkdir security exit.\n" );
    return;
}

module_init( mkdir_init );
module_exit( mkdir_exit );
