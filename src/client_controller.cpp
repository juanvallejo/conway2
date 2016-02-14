#include <iostream>
#include "client_controller.h"

bool client::REQUEST_SERVER_DATA = true;

void client::init(const short port) {

	try {
		asio::io_service io_service;
		asio::ip::tcp::socket socket(io_service);
		asio::ip::tcp::resolver resolver(io_service);
        asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(asio::ip::tcp::resolver::query("127.0.0.1", std::to_string(port)));
        
		std::error_code error;
		size_t stream_size;

        asio::connect(socket, endpoint);
		std::cout << "Client ready." <<std::endl;

		char server_data[1024];

		// while(REQUEST_SERVER_DATA) {
			socket.send(asio::buffer("HEARTBEAT"));
			stream_size = socket.read_some(asio::buffer(server_data), error);

			std::cout << "Server data: " << server_data << std::endl;

			if(error)
				if(error != asio::error::eof)
					std::cout << "status: " << error.message() << std::endl;

			socket.close();
		// }

	} catch(std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

}