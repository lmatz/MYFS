#include <stdlib.h>

#include <data/metadata.hpp>
#include <utils/log.hpp>
#include <myfs/myfs.hpp>


namespace myfs {

	int op_getattr(const char *path, struct stat *stbuf) {
		Log::log_msg("op_getattr path:%s\n", path);	
		int res;
		myfs_ino_t ino;
		myfs_inode inode;


		res = check_path(path);
		if (res != 0) {
			char *msg = strdup("op_getattr check_path fails.");
			Log::log_error(msg);
			free(msg);
			return res;	
		}

		res = GlobalMetadata::get_instance().read_inode(path, &ino, &inode);
		if (res != 0 ) {
			char *msg = strdup("op_getattr read_inode fails.");
			Log::log_error(msg);
			free(msg);
			return res;
		}

		res = do_fill_stat(stbuf, ino, &inode);

		Log::log_msg("op_getattr success\n");
		return 0;
	} 	

}
