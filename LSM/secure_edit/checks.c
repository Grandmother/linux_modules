#include "checks.h"

struct list_head blocked_users = LIST_HEAD_INIT( blocked_users );
struct list_head blocked_groups = LIST_HEAD_INIT( blocked_groups );
struct list_head blocked_globs = LIST_HEAD_INIT( blocked_globs );

bool check_perms( struct dentry* dir )
{
	char* path = NULL;
	char* buf = NULL;

    uid_t euid = current_euid().val;
    gid_t egid = current_egid().val;

    if ( !in_blocked_users(euid) && !in_blocked_groups(egid) )
    {
        printk( KERN_INFO "Mkdir security: User not blocked;\n" );
    	return 0;
    }

    path = dentry_to_path( dir, buf );
    if ( !path )
    {
    	return 0;
    }
    printk( KERN_INFO "Mkdir security: Path: %s\n", path );

    if ( !in_blocked_paths( path ) )
    {
        printk( KERN_INFO "Mkdir security: Path not blocked;\n" );
        kfree( buf );
        return 0;
    }

    printk( KERN_INFO "Mkdir security: Users in this group can't create any directories in this path.\n" );
    kfree( buf );
    return true;
}


char* dentry_to_path( struct dentry* dentry, char* buf )
{
    char* path = NULL;

    buf = (char*) kmalloc( MAX_PATH_LEN, GFP_KERNEL );
    if ( !buf )
    {
        printk( KERN_ALERT "kmalloc fail\n" );
        kfree( buf );
        return NULL;
    }
    path = dentry_path_raw( dentry, buf, MAX_PATH_LEN );
    if ( !path )
    {
        printk( KERN_INFO "Mkdir security: Can't get path of new dir.\n" );
        kfree( buf );
        return NULL;
    }
    return path;
}

void destroy_blocked_users(void)
{
	struct list_head* cur_head = NULL;
	struct list_head* temp_head = NULL;
	struct users_list* cur_user;

    list_for_each_safe(cur_head, temp_head, &blocked_users )
    {
        cur_user = list_entry(cur_head, struct users_list, list);
        list_del( cur_head );
        kfree( cur_user );
    }
}
void destroy_blocked_groups(void)
{
	struct list_head* cur_head = NULL;
	struct list_head* temp_head = NULL;
	struct groups_list* cur_group;

    list_for_each_safe(cur_head, temp_head, &blocked_groups )
    {
        cur_group = list_entry(cur_head, struct groups_list, list);
        list_del( cur_head );
        kfree( cur_group );
    }
}
void destroy_blocked_globs(void)
{
	struct list_head* cur_head = NULL;
	struct list_head* temp_head = NULL;
	struct globs_list* cur_glob;

    list_for_each_safe(cur_head, temp_head, &blocked_globs )
    {
        cur_glob = list_entry(cur_head, struct globs_list, list);
        list_del( cur_head );
        kfree( cur_glob );
    }
}

bool in_blocked_users( uid_t uid )
{
	bool res = false;

	struct users_list* tmp;

    list_for_each_entry( tmp, &blocked_users, list)
    {
        if ( tmp->uid == uid )
        {
            res = true;
            break;
        }
    }

    return res;
}

bool in_blocked_groups( gid_t gid )
{
	bool res = false;

	struct groups_list* tmp;

    list_for_each_entry( tmp, &blocked_groups, list )
    {
        if ( tmp->gid == gid )
        {
            res = true;
            break;
        }
    }

    return res;
}

bool in_blocked_paths( char* path )
{
    bool res = false;

    struct globs_list* tmp;

    list_for_each_entry( tmp, &blocked_globs, list )
    {
        if ( glob_match ( tmp->glob, path ) )
        {
            res = true;
            break;
        }
    }

    return res;
}


bool block_user( uid_t uid )
{
	struct users_list* tmp_user;

	// allocate memory for list entry
	tmp_user = ( struct users_list* ) kmalloc( sizeof(struct users_list), GFP_ATOMIC );
	if ( !tmp_user )
	{
		printk( KERN_ALERT "Mkdir security: kmalloc fail\n");
		return false;
	}

	// init list entry
	INIT_LIST_HEAD( &(tmp_user->list) );
	tmp_user->uid = uid;

	list_add( &(tmp_user->list), &blocked_users );
    return true;
}
// void unblock_user( uid_t uid )
// {}

bool block_group( gid_t gid )
{
	struct groups_list* tmp_group;

	// allocate memory for list entry
	tmp_group = ( struct groups_list* ) kmalloc( sizeof(struct groups_list), GFP_ATOMIC );
	if ( !tmp_group )
	{
		printk( KERN_ALERT "Mkdir security: kmalloc fail\n");
		return false;
	}

	// init list entry
	INIT_LIST_HEAD( &(tmp_group->list) );
	tmp_group->gid = gid;

	list_add( &(tmp_group->list), &blocked_groups );
    return true;
}
// void unblock_group( gid_t gid )
// {}

bool block_path( char* path )
{
	struct globs_list* tmp_glob;
    printk( KERN_INFO "Mkdir security: block_path - %s\n", path);

	tmp_glob = ( struct globs_list* ) kmalloc( sizeof(struct globs_list), GFP_ATOMIC );
	if ( !tmp_glob )
	{
		printk( KERN_ALERT "Mkdir security: kmalloc fail\n");
		return false;
	}


	INIT_LIST_HEAD( &(tmp_glob->list) );
	tmp_glob->glob = ( char* ) kmalloc( strlen(path) + 1 , GFP_ATOMIC );
	strncpy( tmp_glob->glob, path, strlen(path) + 1 );

	list_add( &(tmp_glob->list),  &blocked_globs );
    return true;
}
// void unblock_path_str( char* path )
// {}
