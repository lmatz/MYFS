#pragma once

#include <stdio.h>

#define log_struct(st, field, format, typecast) \
	    log_msg("    " #field " = " #format "\n", typecast st->field)

struct fuse_conn_info;
struct fuse_file_info;
struct fuse_context;
struct utimbuf;
struct stat;
struct statvfs;


namespace myfs {

	class Log {
	public:
		static FILE *log_open(void);
		
		static void log_msg(const char *format, ...);
		
		static void log_conn(struct ::fuse_conn_info *conn);
		
		static int log_error(char *func);
		
		static void log_fi(struct ::fuse_file_info *fi);
		
		static void log_fuse_context(struct ::fuse_context *context);
		
		static void log_retstat(char *func, int retstat);
		
		static void log_stat(struct ::stat *si);
		
		static void log_statvfs(struct ::statvfs *sv);
		
		static int  log_syscall(char *func, int retstat, int min_ret);
		
		static void log_utime(struct ::utimbuf *buf);
	};

}
