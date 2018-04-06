#pragma once

namespace myfs {

	// return 0 means success
	// otherwise, master and worker should abort instead of calling fuse_main
	int check();

}
