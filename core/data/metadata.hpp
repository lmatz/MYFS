#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include <data/inode.hpp>
//#include <data/dir.hpp>
//#include <data/file.hpp>
#include <utils/constant.hpp>
#include <utils/lru_cache.hpp>
#include <utils/type.hpp>

#include <boost/iostreams/device/mapped_file.hpp>

namespace myfs {

	class Dir;
	class File;

	// all the struct myfs_inode in this class should not be directly modified
	// a inode should only be modified through class Dir or class File
	class GlobalMetadata {
	public:
		
		static GlobalMetadata& get_instance() {
			static GlobalMetadata metadata;
			return metadata; 
		}

		int initialize(const std::string &meta_filename, int cache_size, uint32_t num_bytes, uint32_t num_inodes);

		int make_directory(const char *path, mode_t mode);

		int change_mode(const char *path, mode_t mode);

		int change_owner(const char *path, uid_t uid, gid_t gid);

		int check_access(const char *path, int mask, uid_t uid, gid_t gid);

		int read_inode(const char *path, myfs_ino_t *, myfs_inode *);

		int write_inode(myfs_ino_t, myfs_inode *);

	private:

		// friends
		friend Dir;

		GlobalMetadata();

		~GlobalMetadata();

		void determine_layout(uint32_t num_bytes, uint32_t num_inodes);

		// get the pointer pointing to the start of inodes' bitmap
		char* inode_bitmap();

		// get the pointer pointing to the start of data blocks' bitmap
		char* data_block_bitmap();

		// get the inode pointer pointing to the start of inode with inode number 'ino_t'
		int get_inode(const myfs_ino_t ino_t, myfs_inode *res);

		// get the pointer pointing to the start of data block with 'index' 
		int get_data_block(uint32_t index, char *res);

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

		// convert data block to subdirs
		// int convert_data_block_to_subdirs(std::vector<std::string> &subdirs, char *data, int len);

		// find a new allocation for inode
		// since inode number serves as the index for inode struct
		int allocate_new_inode(myfs_ino_t *index);

		// fine a new allocation for data_block;
		int allocate_new_data_block(uint32_t *idx);

		// this function sohuld only be called when we are sure that this file exists
		int check_permission(uid_t ino_uid, gid_t ino_gid, mode_t mode, uid_t uid, gid_t gid, int mask);

		boost::iostreams::mapped_file file;

		// how many inodes we can store
		uint32_t num_bits_i;

		// how many inode bits are free
		uint32_t free_bits_i;
		
		// how many data blocks are free
		uint32_t free_bits_d;

		// how many data blocks we can hold
		uint32_t num_bits_d;

		// bytes for a inode struct
		uint32_t inode_size = INODE_SIZE;

		// bytes for a data block
		uint32_t data_block_size = BLOCK_SIZE;

		// whether it is initailized
		bool init;
	};

}
