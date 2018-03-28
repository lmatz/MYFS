#pragma once
#include <string>
#include <unordered_map>

#include <utils/lru_cache.hpp>
#include <utils/type.hpp>

#include <boost/iostreams/device/mapped_file.hpp>

namespace myfs {

	class GlobalMetadata {
	public:
		
		GlobalMetadata(const std::string &meta_dir, int cache_size, uint32_t num_bytes, uint32_t num_inodes);

		~GlobalMetadata();
		
		int read_inode(const char *path, myfs_ino_t *, myfs_inode *);

		int write_inode(myfs_ino_t, myfs_inode *);

	private:

		void determine_layout(uint32_t num_bytes, uint32_t num_inodes);

		// get the pointer pointing to the start of inodes' bitmap
		char* inode_bitmap();

		// get the pointer pointing to the start of data blocks' bitmap
		char* data_block_bitmap();

		// get the inode pointer pointing to the start of inode with inode number 'index+1'
		myfs_inode* get_inode(uint32_t index);

		// get the pointer pointing to the start of data block with 'index' 
		void* get_data_block(uint32_t index);

		// we search inode named by 'path', and return the inode number by 'ino'
		// if inode is found, then return 0, else not 0
		int search_inode(myfs_ino_t *ino, const char *path);

		// initialize the inode for root dir '/'
		int initialize_root_dir();

		// input ino_t represents the current directory
		// name represents the subdirectory or file we want to find
		// output ino_t represents the inode number of subdirectory
		// if subdir is found, then return 0, else not 0
		int find_file_in_dir(myfs_ino_t *ino_t, const char *name);

		//std::unordered_map<myfs_ino_t, myfs_inode*> ino_t_to_inode;

		// cache so that we can easily find the inode number by path
		lru_cache<std::string, myfs_ino_t> path_to_ino_t;

		boost::iostreams::mapped_file file;

		// inode_size should be <= 128 bytes
		const static size_t inode_size = sizeof(myfs_inode);

		// how many inodes we can store
		uint32_t num_bits_i;

		// how many inode bits are free
		uint32_t free_bits_i;
		
		// how many data blocks are free
		uint32_t free_bits_d;

		// how many data blocks we can hold
		uint32_t num_bits_d;
	};

}
