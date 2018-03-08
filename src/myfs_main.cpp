#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


static struct fuse_operations myfs_oper = {
	/*getattr :*/ NULL,
	/*readlink :*/ NULL,
	/*getdir :*/ NULL,
	/*mknod :*/ NULL,
	/*mkdir :*/ NULL,
	/*unlink :*/ NULL,
	/*rmdir :*/ NULL,
	/*symlink :*/ NULL,
	/*rename :*/ NULL,
	/*link :*/ NULL,
	/*chmod :*/ NULL,
	/*chown :*/ NULL,
	/*truncate :*/ NULL,
	/*utime :*/ NULL,
	/*open :*/ NULL,
	/*read :*/ NULL,
	/*write :*/ NULL,
	/*statfs :*/ NULL,
	/*flush :*/ NULL,
	/*release :*/ NULL,
	/*fsync :*/ NULL,
	/*setxattr :*/ NULL,
	/*getxattr :*/ NULL,
	/*listxattr :*/ NULL,
	/*removexattr :*/ NULL,
	/*opendir :*/ NULL,
	/*readdir :*/ NULL,
	/*releasedir :*/ NULL,
	/*fsyncdir :*/ NULL,
	/*init :*/ NULL,
	/*destroy :*/ NULL,
	/*access :*/ NULL,
	/*create :*/ NULL,
	/*ftruncate :*/ NULL,
	/*fgetattr :*/ NULL,
	/*lock :*/ NULL,
	/*utimens :*/ NULL,
	/*bmap :*/ NULL,
	/*flag_nullpath_ok :*/ 1,
	/*flag_nopath :*/ 1,
	/*flag_utime_omit_ok :*/ 1,
	/*flag_reserved :*/ 29,
	/*ioctl :*/ NULL,
	/*poll :*/ NULL,
	/*write_buf :*/ NULL,
	/*read_buf :*/ NULL,
	/*flock :*/ NULL,
	/*fallocate :*/ NULL,
};




int main(int argc, char *argv[]) {
	return fuse_main(argc, argv, &myfs_oper, NULL);
}
