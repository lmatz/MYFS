#include <data/metadata.hpp>
#include <op/op_myfs.hpp>
#include <utils/log.hpp>

#include <stddef.h>
#include <assert.h>
#include <string>

using namespace myfs;

static const struct fuse_operations myfs_oper = {
	/*getattr :*/ op_getattr,
	/*readlink :*/ op_readlink,
	/*getdir :*/ op_getdir,
	/*mknod :*/ op_mknod,
	/*mkdir :*/ op_mkdir,
	/*unlink :*/ op_unlink,
	/*rmdir :*/ op_rmdir,
	/*symlink :*/ op_symlink,
	/*rename :*/ op_rename,
	/*link :*/ op_link,
	/*chmod :*/ op_chmod,
	/*chown :*/ op_chown,
	/*truncate :*/ op_truncate,
	/*utime :*/ op_utime,
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
	/*readdir :*/ op_readdir,
	/*releasedir :*/ NULL,
	/*fsyncdir :*/ NULL,
	/*init :*/ op_init,
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

void myfs_usage() {
	fprintf(stderr, "usage: myfs [FUSE and mount options] mountPoint\n");
	abort();
}


static struct options {
	int debug;
	int show_help;
	int meta_file_size;
	uint32_t num_inodes;
	uint64_t theta;
	const char* log_filename;
	const char* meta_dir;
} options;

#define OPTION(t, p) \
	{ t, offsetof(struct options, p), 1 }

static const struct fuse_opt option_spec[] = {
	OPTION("-d", debug),
	OPTION("--debug", debug),
	OPTION("-h", show_help),
	OPTION("--help", show_help),
	OPTION("--theta=%lu", theta),
	OPTION("--log-filename=%s", log_filename),
	OPTION("--meta-dir=%s", meta_dir),
	OPTION("--meta-file-size=%d", meta_file_size),
	OPTION("--num-inodes=%u", num_inodes),
	FUSE_OPT_END
};


static void show_help(const char *progname)
{
	printf("usage: %s [options] <mountpoint>\n\n", progname);
	printf("File-system specific options:\n"
	       "    --log-filename=<s>          Name of the log file [Default] myfs.log\n"
		   "    --meta-dir=<s>              Name of the log file [Default] current directory\n"
		   "    --meta-file-size=<d>        The size of meta file[Default] 1GB\n"
		   "    --num-inodes=<u>            The number of inodes [Default] 1024*1024=1048576\n"
		   "    --debug, -d                 Enable debug         [Default] false\n"
		   "    --help, -h                  Show help            [Default] false\n"
		   "    --theta=<lu>                Set theta            [No Default]\n"
	       "\n");
}


int main(int argc, char *argv[]) {

	// get the current directory
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
    	fprintf(stdout, "Current working dir: %s\n", cwd);
	else
    	perror("getcwd() error");

	int res = check();
	if (res!=0) {
		return 1;
	}

	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	
	myfs::myfs_state *myfs_state = new myfs::myfs_state();
	/* Set defaults -- we have to use strdup so that
	   fuse_opt_parse can free the defaults if other
	   values are specified */
	options.debug = 0;
	options.show_help = 0;
	options.theta = 0;
	options.num_inodes = 1024*1024;
	options.log_filename = strdup("myfs.log");
	options.meta_dir = strdup(cwd);
	options.meta_file_size = 1024*1024*1024; // 1GB
		
	/* Parse options */
	if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1) {
		return 1;	
	}

	if (options.show_help) {
		show_help(argv[0]);
		assert(fuse_opt_add_arg(&args, "--help") == 0);
		args.argv[0] = (char*) "";
	}
	else {
		// theta must be set by the user
		if (!options.theta) {
			fprintf(stderr, "theta is %lu. theta must be set by user\n", options.theta);
			show_help(argv[0]);
			return 1;
		}
	}



	myfs_state->meta_dir = strdup(options.meta_dir);
	fprintf(stderr, "myfs_state->meta_dir: %s\n", myfs_state->meta_dir);
	myfs_state->meta_file_size = options.meta_file_size;
	fprintf(stderr, "myfs_state->meta_file_size: %d\n", myfs_state->meta_file_size);
	myfs_state->num_inodes = options.num_inodes;
	fprintf(stderr, "myfs_state->num_inodes: %u\n", myfs_state->num_inodes);
	myfs_state->metadata = new GlobalMetadata(myfs_state->meta_dir, 500, myfs_state->meta_file_size, myfs_state->num_inodes);

	myfs_state->debug = options.debug;
	myfs_state->theta = options.theta;
	myfs_state->log_filename = strdup(options.log_filename);
	fprintf(stderr, "mfs_state->log_filename: %s\n", myfs_state->meta_dir);
	myfs_state->logfile = Log::log_open(myfs_state->log_filename);
	fprintf(stderr, "logfile created.\n try to create metadata.\n");
	
	// turn over control to fuse
	fprintf(stderr, "about to call fuse_main\n");
	int fuse_stat = fuse_main(args.argc, args.argv, &myfs_oper, myfs_state);
	fprintf(stderr, "fuse_main returned %d\n", fuse_stat);

	return fuse_stat;
}
