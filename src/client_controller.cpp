#include <iostream>
#include "client_controller.h"

void client::init(const short port) {

	try {
		asio::io_service io_service;
		asio::ip::tcp::socket socket(io_service);
		asio::ip::tcp::resolver resolver(io_service);
        asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(asio::ip::tcp::resolver::query("127.0.0.1", std::to_string(port)));
        
        asio::connect(socket, endpoint);
		std::cout << "Client ready." <<std::endl;
		socket.send(asio::buffer("This is a test"));
		socket.close();

	} catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

}