#pragma once

#include <utils/param.hpp>
#include <data/metadata.hpp>
#include <io/io_manager.hpp>

namespace myfs {

#define MYFS_METADATA ((GlobalMetadata*)(((struct myfs_state *) fuse_get_context()->private_data)->metadata))

#define MYFS_IO ((IOManager*)(((struct myfs_state *) fuse_get_context()->private_data)->io_manager))

}
