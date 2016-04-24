// Module is based on description from http://www.okbsapr.ru/kanner_2012_5.html


// SECURITY_PATH is a kernel configuration variable that should be set to
// allow us to use security hooks for pathbased access control.
// More information here: linux/security/Kconfig:85
#ifndef CONFIG_SECURITY_PATH

#endif

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/security.h> // for Security Linux Module functions
#include <linux/dcache.h>   // for struct dentry
#include <linux/errno.h>    //for error codes
#include <linux/cred.h>     // for current_uid/gid
#include <linux/sched.h>    // struct task_struct
#include <linux/path.h>     // struct path
#include <linux/types.h>    // umode_t

#define MKDIRSEC_GID_NO 4273   // Group ID of group with denied access.
#define MKDIRSEC_UID_NO 7342   // User ID of user with denied access.

#define MKDIRSEC_GID_YES 100   // Group ID of group with allowed access.
#define MKDIRSEC_UID_YES 1000  // User ID of user with allowed access.

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Roman Kovtyukh <HelloDearGrandma@gmail.com>" );

/*
 * Some structures definitions.
 */
// default security_operations structure
extern struct security_operations security_ops;

struct task_struct;

/*
 * SECURITY HOOKS
 */

// MKDIR hook
static int secure_path_mkdir( struct path* dir, struct dentry* dentry, umode_t mask )
{
    uid_t uid  = current_uid().val;
    uid_t euid = current_euid().val;
    gid_t gid  = current_gid().val;
    gid_t egid = current_egid().val;

    printk( KERN_INFO "Mkdir security: uid:%d euid:%d gid:%d egid:%d\n", uid, euid, gid, egid);

    // GID of mkdirse
    if ( egid == MKDIRSEC_GID_NO || euid == MKDIRSEC_UID_NO )
    {
        printk( KERN_ALERT "Mkdir security: Users in this group can't create any directories." );
        return EACCES;
    }

    if ( egid == MKDIRSEC_GID_YES || euid == MKDIRSEC_UID_YES )
    {
        // May be is not safe to print the dir name here, but it is just for debug now.
        printk( KERN_INFO "Mkdir security: Creating dir with name: %s\n", dentry->d_name.name );
    }
    return security_ops.path_mkdir(dir, dentry, mask);
}

int secure_path_unlink( struct path* dir, struct dentry* dentry ){}
int secure_path_rmdir( struct path* dir, struct dentry* dentry ){}
int secure_path_truncate( struct path* path ){}
int secure_path_symlink( struct path* dir, struct dentry* dentry, const char* old_name ){}
int secure_path_link( struct dentry* old_dentry, struct path* new_dir, struct dentry* new_dentry ){}
int secure_path_rename( struct path* old_dir, struct dentry* old_dentry,
                        struct path* new_dir, struct dentry* new_dentry){}
int secure_path_chmod( struct path* path, umode_t mode ){}
int secure_path_chown( struct path* path, kuid_t uid, kgid_t gid ){}



// security_operations structure.
static struct security_operations hook_security_ops = {
    .name = "mkdir_sec",
    .inode_mkdir = secure_inode_mkdir,
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
    if ( mkdir_hook_register() )
    {
        printk( KERN_INFO "Error while mkdir security initialization.\n" );
    }
    return 0;
}

// module EXIT
static __exit void mkdir_exit(void)
{
    printk( KERN_INFO "Mkdir security exit.\n" );
    return;
}

module_init( mkdir_init );
module_exit( mkdir_exit );
