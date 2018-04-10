#pragma once
#include <stdint.h>

#include <data/dir_entry.hpp>
#include <data/metadata.hpp>
#include <utils/type.hpp>

#include <vector>
#include <unordered_map>

namespace myfs {

	class Dir {
	public:
		// construct a Dir class by myfs_inode 
		Dir(myfs_ino_t ino, myfs_inode *inode);

	private:
		// initialize
		void init();

		// get all the names of sub directory in this data block
		int get_dir_names_from_data_block(char *, std::unordered_map<std::string, myfs_ino_t>&);

		// check whether a sub directory is in this data block
		int get_ino_by_name(const char *name, myfs_ino_t *ino_ret);

		// insert sub directory
		int insert_subdir(myfs_ino_t ino, const char *name);


	private:
		// data_block is only added when necessary
		// so after adding the data block, the data_block must store something on it
		// so it is a private function
		// return 0 indicates that we successfully add the block and the block stores actual data
		// return 1 indicates that we successfully add the block and the block stores pointers to actual data block
		int add_new_data_block(uint32_t block_idx);

		int find_space_in_data_block(uint32_t *space, char *loc, DirEntry *prev_dir_entry, DirEntry *next_dir_entry, uint32_t block_idx);

	private:
		myfs_ino_t dir_ino;
		myfs_inode *dir_inode;

		std::unordered_map<std::string, myfs_ino_t> name_to_ino;
	};

}
