#include <linux/string.h>
#include <linux/glob.h>

#include "hooks.h"
#include "checks.h"

// MKDIR hook
int secure_path_mkdir( struct path* dir, struct dentry* dentry, umode_t mask )
{
	printk( KERN_INFO "secure_path_mkdir\n" );
    return check_perms(dentry);
}

int secure_path_unlink( struct path* dir, struct dentry* dentry )
{
	printk( KERN_INFO "secure_path_unlink\n" );
    return check_perms(dentry);
}
int secure_path_rmdir( struct path* dir, struct dentry* dentry )
{
	printk( KERN_INFO "secure_path_rmdir\n" );
    return check_perms(dentry);
}
// int secure_path_truncate( struct path* path )
// {
// }
int secure_path_symlink( struct path* dir, struct dentry* dentry, const char* old_name )
{
	printk( KERN_INFO "secure_path_symlink\n" );
    return check_perms(dentry);
}
int secure_path_link( struct dentry* old_dentry, struct path* new_dir, struct dentry* new_dentry )
{
	printk( KERN_INFO "secure_path_link\n" );
    return check_perms(old_dentry);
}
int secure_path_rename( struct path* old_dir, struct dentry* old_dentry,
                        struct path* new_dir, struct dentry* new_dentry)
{
	printk( KERN_INFO "secure_path_rename\n" );
    return check_perms(old_dentry);
}
// int secure_path_chmod( struct path* path, umode_t mode ){}
// int secure_path_chown( struct path* path, kuid_t uid, kgid_t gid ){}
