#include <op/op_myfs.hpp>

#include <utils/constant.hpp>


namespace myfs {

	int do_fill_stat(struct stat *stbuf, myfs_ino_t ino, myfs_inode *inode) {
		
		stbuf->st_ino = ino;
		stbuf->st_mode = inode->i_mode;
		stbuf->st_nlink = inode->i_links_count;
		stbuf->st_uid = inode->i_uid;
		stbuf->st_gid = inode->i_gid;
		stbuf->st_size = inode->i_size;
		stbuf->st_blksize = BLOCK_SIZE;
		stbuf->st_blocks = NUM_BLOCKS;

		// time
		stbuf->st_atim.tv_sec = inode->i_atime;
		stbuf->st_mtim.tv_sec = inode->i_mtime;
		
		// time of last status change?

		
		return 0;
	}

}
