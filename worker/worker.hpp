#pragma once

#include <string>
#include <utility>

#include <zmq.hpp>

namespace worker {

	class Worker {
	public:
		static Worker& get_instance() {
			static Worker worker;
			return worker;
		}

		void set_worker_port(std::string port) {
			this->worker_port = port;
		}

		void set_master_addr(std::string addr) {
			this->master_addr = addr;
		}

		void set_master_port(std::string port) {
			this->master_port = port;
		}

		void init_socket() {
			this->worker_socket.reset(new zmq::socket_t(this->zmq_context, ZMQ_ROUTER));
			std::string master_point = "tcp://" + this->master_addr + ":" + this->master_port;	
			std::string worker_point = "localhost:" + this->worker_port;
			this->worker_socket->bind(master_point + ";" + worker_point);

		}

		void serve() {
			char buf[4096];
			while(this->running) {

			}	
		}

		bool is_running() {
			return this->running;
		}
	
	protected:
		Worker() {}


	private:
		bool running = false;

		std::string worker_port;
		std::string master_addr;
		std::string master_port;

		zmq::context_t zmq_context;
    	std::unique_ptr<zmq::socket_t> worker_socket;


	};
}
