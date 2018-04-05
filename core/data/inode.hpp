#include <stdint.h>

#include <utils/constant.hpp>

namespace myfs {

#define S_IFMT      0170000
#define S_IFSOCK    0140000
#define S_IFLNK     0120000
#define S_IFREG     0100000
#define S_IFBLK     0060000
#define S_IFDIR     0040000
#define S_IFCHR     0020000
#define S_IFIFO     0010000

#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISLNK(m)	(((m) & S_IFMT) == S_ISLNK)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)
	struct myfs_inode {
		uint16_t	i_mode; 	/* File mode */
		uint16_t	i_uid;  	/* Owner Uid */
		uint32_t	i_size;		/* Size in bytes */
		uint32_t	i_atime;	/* Access time */
		uint32_t	i_ctime;	/* Creation time */
		uint32_t	i_mtime;	/* Modification time */
		uint32_t	i_dtime;	/* Deletion Time */
		uint16_t	i_gid;		/* Group Id */
		uint16_t	i_links_count;	/* Links count */
		uint32_t	i_blocks;	/* Blocks count */
		uint32_t	i_flags;	/* File flags */
		union {
			struct {
				uint32_t  l_i_reserved1;
			} linux1;
			struct {
				uint32_t  h_i_translator;
			} hurd1;
			struct {
				uint32_t  m_i_reserved1;
			} masix1;
		} osd1;				/* OS dependent 1 */
		uint32_t	i_block[NUM_BLOCKS];/* Pointers to blocks */
		uint32_t	i_version;	/* File version (for NFS) */
		uint32_t	i_file_acl;	/* File ACL */
		uint32_t	i_dir_acl;	/* Directory ACL */
		uint32_t	i_faddr;	/* Fragment address */
	};

}
