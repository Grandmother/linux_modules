#ifndef __SECURE_EDIT_HOOKS__
#define __SECURE_EDIT_HOOKS__

// SECURITY_PATH is a kernel configuration variable that should be set to
// allow us to use security hooks for pathbased access control.
// More information here: linux/security/Kconfig:85
#ifndef CONFIG_SECURITY_PATH
#warning "WARNING: You should enable SECURITY_PATH option in kernel configuration to build this module"
#else

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/security.h> // for Security Linux Module functions
#include <linux/dcache.h>   // for struct dentry
#include <linux/path.h>     // struct path
#include <linux/types.h>    // umode_t

/*
 * Some structures definitions.
 */
// default security_operations structure
extern struct security_operations* security_ops;

struct task_struct;


/*
 * SECURITY HOOKS
 */

int secure_path_mkdir( struct path* dir, struct dentry* dentry, umode_t mask );
int secure_path_unlink( struct path* dir, struct dentry* dentry );
int secure_path_rmdir( struct path* dir, struct dentry* dentry );
// int secure_path_truncate( struct path* path );
int secure_path_symlink( struct path* dir, struct dentry* dentry, const char* old_name );
int secure_path_link( struct dentry* old_dentry, struct path* new_dir, struct dentry* new_dentry );
int secure_path_rename( struct path* old_dir, struct dentry* old_dentry,
                        struct path* new_dir, struct dentry* new_dentry);
// int secure_path_chmod( struct path* path, umode_t mode );
// int secure_path_chown( struct path* path, kuid_t uid, kgid_t gid );

#endif // CONFIG_SECURITY_PATH
#endif // __SECURE_EDIT_HOOKS__
