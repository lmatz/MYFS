#include <zmq.hpp>

#include <string.h>

#include <all_fuse.hpp>

#include <worker.hpp>

using namespace worker;

static struct options {
	int debug;
	int show_help;
	const char* log_filename;
	const char* worker_port;
	const char* master_addr;
	const char* master_port;
} options;

#define OPTION(t, p) \
	{ t, offsetof(struct options, p), 1 }

static const struct fuse_opt option_spec[] = {
	OPTION("-d", debug),
	OPTION("-h", show_help),
	OPTION("--help", show_help),
	OPTION("--log-filename=%s", log_filename),
	OPTION("--worker-port=%s", worker_port),
	OPTION("--master-addr=%s", master_addr),
	OPTION("--master-port=%s", master_port),
	FUSE_OPT_END
};

static void show_help(const char *progname) {
	printf("usage: %s [options]\n\n", progname);
	printf("Worker specific options:\n"
		   " --log-filename=<s>        Name of the log file [Default] worker.log\n"
		   " --debug, -d               Enable debug         [Default] Enable debug\n"
		   " --help, -h                Show help            [Default] Show help\n"
		   " --worker-port=<s>         Set worker port      [Default] 16667\n"
		   " --master-port=<s>         Set master port      [Default] 16666\n"
		   " --master-addr=<s>         Set master addr      [Default] 127.0.0.1\n"
		   "\n");

}

int main(int argc, char *argv[]) {

	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

	options.debug = 0;
	options.show_help = 0;
	options.log_filename = strdup("worker.log");
	options.worker_port = strdup("16667");
	options.master_addr = strdup("127.0.0.1");
	options.master_port = strdup("16666");
		
	/* Parse options */
	if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1) {
		return 1;
	}

	if (options.show_help) {
		show_help(argv[0]);
		return 0;
	}


	Worker &worker = Worker::get_instance();
	worker.set_worker_port(options.worker_port);
	worker.set_master_addr(options.master_addr);
	worker.set_master_port(options.master_port);
	worker.init_socket();
	// stuck in the loop
	worker.serve();
	

	return 0;
}
