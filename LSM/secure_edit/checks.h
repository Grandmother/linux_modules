#ifndef __SECURE_EDIT_CHECKS__
#define __SECURE_EDIT_CHECKS__


#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/glob.h>
#include <linux/types.h>		// umode_t and other types
#include <linux/dcache.h>		// struct dentry
#include <linux/errno.h>		//for error codes
#include <linux/sched.h>    	// struct task_struct
#include <linux/cred.h>     	// for current_uid/gid

#define MAX_PATH_LEN 512

struct users blocked_users;
struct groups blocked_groups;
struct globs blocked_globs;

struct users
{
	struct list_head users_list;
	struct kobject kobj;
};

struct groups
{
	struct list_head groups_list;
	struct kobject kobj;
};

struct globs
{
	struct list_head globs_list;
	struct kobject kobj;
};



struct users_list
{
	struct list_head list;
	uid_t uid;
};

struct groups_list
{
	struct list_head list;
	gid_t gid;
};

struct globs_list
{
	struct list_head list;
	char* glob;
};



bool check_perms( struct dentry* dir );

/**
*	This function will allocate memory for directory path.
* and you should free it yourself.
* It returns the pointer to path string, but it can be
* different with buf pointer.
*/
char* dentry_to_path( struct dentry* dentry, char* buf );

void init_blocked_users(void);
void init_blocked_groups(void);
void init_blocked_globs(void);

void destroy_blocked_users(void);
void destroy_blocked_groups(void);
void destroy_blocked_globs(void);

bool in_blocked_users( uid_t uid );
bool in_blocked_groups( gid_t gid );
bool in_blocked_paths( char* path );

bool block_user( uid_t uid );
// void unblock_user( uid_t uid );

bool block_group( gid_t gid );
// void unblock_group( gid_t gid );

bool block_path( char* path );
// void unblock_path_str( char* path );
//	void unblock_path_int( int* path_num );

#endif //__SECURE_EDIT_CHECKS__
