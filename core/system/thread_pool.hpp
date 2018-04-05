#pragma once

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <utils/constant.hpp>

namespace myfs {
  class ThreadPool {
  public:
    template<typename F>
    void execute(F f) {
      io_service_.post(f);
    }

    static ThreadPool &get_instance() {
      if (thread_pool_ == nullptr) {
        thread_pool_ = new ThreadPool(THREADPOOL_SIZE);
      }
      return *thread_pool_;
    }

  private:
    explicit ThreadPool(size_t size) : work_(io_service_) {
      for (size_t i = 0; i < size; i++) {
        workers_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
      }
    }

    ~ThreadPool() {
      io_service_.stop();
      workers_.join_all();
    }

    static ThreadPool *thread_pool_;

    boost::thread_group workers_;
    boost::asio::io_service io_service_;
    boost::asio::io_service::work work_;
  };
}
