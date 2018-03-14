#include <myfs/myfs.hpp>


namespace myfs {



	// return 0 means success
	// otherwise, master and worker should abort instead of calling fuse_main
	int check() {
		int res = 0;
		int size = sizeof(struct myfs_inode);
		if (size != 116) {
			fprintf(stderr, "sizeof struct myfs_inode is not 112 but %d\n", size);
			return -1;
		}

		return 0;	
	}

	void strmode(mode_t mode, char * buf) {
  		const char chars[] = "rwxrwxrwx";
  		for (size_t i = 0; i < 9; i++) {
    		buf[i] = (mode & (1 << (8-i))) ? chars[i] : '-';
 	 	}
  		buf[9] = '\0';
	}

}
