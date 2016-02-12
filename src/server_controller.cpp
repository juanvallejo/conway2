#include <iostream>
#include <mutex>
#include "server_controller.h"

std::mutex server::smutex;

void server::init(const short port) {

	std::lock_guard<std::mutex> lock(server::smutex);

	std::cout << "Initialized as server. Listening on port " << port << std::endl;

	try {
		asio::io_service io_service;
		asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
		asio::ip::tcp::acceptor acceptor(io_service, endpoint);
		asio::ip::tcp::socket socket(io_service);

		asio::streambuf sbuf;
		std::error_code ecode;

		std::cout << "Server ready." <<std::endl;

		{
			acceptor.accept(socket);

			while(asio::read(socket, sbuf, ecode)) {
				socket.send(asio::buffer("This is a test"));
				if(ecode) {
					std::cout << "status: " << ecode.message() << std::endl;
					break;
				}
			}

			socket.close();
		}

	} catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

}