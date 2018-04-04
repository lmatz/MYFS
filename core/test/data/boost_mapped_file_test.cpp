#include <gtest/gtest.h>

#include <boost/iostreams/device/mapped_file.hpp>

#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

namespace test {


	class MappedFileTest : public testing::Test {
	protected:

		virtual void SetUp() {
			int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
			EXPECT_LT(0, fd);
			if (fd < 0) {
				perror("Cannot open file\n"); 
			}
			int res = ftruncate(fd, num_bytes);
			EXPECT_EQ(0, res);
			if (res < 0) {
				perror("Cannot ftruncate file\n");
			}

			fd = open(bigfilename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
			EXPECT_LT(0, fd);
			if (fd < 0) {
				perror("Cannot open big file\n");
			}
			res = ftruncate(fd, big_num_bytes);
			EXPECT_EQ(0, res);
			if (res < 0) {
				perror("Cannot ftruncate big file\n");
			}
		}

		virtual void TearDown() {
			int res = remove(filename);
			EXPECT_EQ(0, res);
			if (res < 0) {
				perror("Cannot remove file\n");
			}

			res = remove(bigfilename);
			EXPECT_EQ(0, res);
			if (res < 0) {
				perror("Cannot remove big file\n");
			}
		}


		int num_bytes = 1024*1024;
		long long int big_num_bytes = 1024LL*1024*1024*8;
		const char *filename = "./MappedFileTest";
		const char *bigfilename = "./BigMappedFileTest";
		boost::iostreams::mapped_file mapped_file;
	};

	TEST_F(MappedFileTest, Basic) {
		mapped_file.open(filename, boost::iostreams::mapped_file::readwrite, num_bytes);
		EXPECT_EQ(true, mapped_file.is_open());
		EXPECT_EQ(boost::iostreams::mapped_file::readwrite, mapped_file.flags());
		EXPECT_EQ(num_bytes, mapped_file.size());

		char* data = mapped_file.data();
		strcpy(data,"123456789");
		mapped_file.close();
		EXPECT_EQ(false, mapped_file.is_open());

		mapped_file.open(filename, boost::iostreams::mapped_file::readonly, num_bytes);
		EXPECT_EQ(true, mapped_file.is_open());
		EXPECT_EQ(boost::iostreams::mapped_file::readonly, mapped_file.flags());
		const char* const_data = mapped_file.const_data();
		EXPECT_EQ(0, memcmp("123456789", const_data, 9));
		mapped_file.close();
		EXPECT_EQ(false, mapped_file.is_open());
	}


	TEST_F(MappedFileTest, Large) {
		mapped_file.open(bigfilename, boost::iostreams::mapped_file::readwrite, big_num_bytes);
		EXPECT_EQ(true, mapped_file.is_open());
		EXPECT_EQ(boost::iostreams::mapped_file::readwrite, mapped_file.flags());
		EXPECT_EQ(big_num_bytes, mapped_file.size());

		char* data = mapped_file.data();
		strcpy(data,"123456789");
		data += big_num_bytes - 10;
		strcpy(data,"123456789");
		mapped_file.close();
		EXPECT_EQ(false, mapped_file.is_open());

		mapped_file.open(bigfilename, boost::iostreams::mapped_file::readonly, big_num_bytes);
		EXPECT_EQ(true, mapped_file.is_open());
		EXPECT_EQ(boost::iostreams::mapped_file::readonly, mapped_file.flags());
		const char* const_data = mapped_file.const_data();
		EXPECT_EQ(0, memcmp("123456789", const_data, 9));
		const_data += big_num_bytes - 10;
		EXPECT_EQ(0, memcmp("123456789", const_data, 9));
		mapped_file.close();
		EXPECT_EQ(false, mapped_file.is_open());
	}


}
