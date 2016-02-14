#include <iostream>
#include <mutex>
#include "controller.h"
#include "server_controller.h"

bool server::ACCEPT_CLIENT_CONN = true;
std::mutex server::smutex;

void server::load_game_data(std::ostream& stream) {
	if(!controller::get_game_matrix()) return;
	for(int i = 0; i < controller::get_game_width() * controller::get_game_height(); i++)
		stream << controller::get_game_matrix()[i];
}

void server::init(const short port) {

	std::lock_guard<std::mutex> lock(server::smutex);

	std::cout << "Initialized as server. Listening on port " << port << std::endl;

	try {
		asio::io_service io_service;
		asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port);
		asio::ip::tcp::acceptor acceptor(io_service, endpoint);
		asio::ip::tcp::socket socket(io_service);

		char client_data[1024];

		asio::streambuf sbuf;
		asio::streambuf buf_conway;
		std::error_code error;

		std::cout << "Server ready." <<std::endl;
		
		size_t stream_size;
		std::ostream buf_conway_stream(&buf_conway);

		{
			while(server::ACCEPT_CLIENT_CONN) {
				acceptor.accept(socket);
				stream_size = socket.read_some(asio::buffer(client_data), error);

				memset(client_data, 0, stream_size);

				// socket.send(asio::buffer("Message from server"));
				server::load_game_data(buf_conway_stream);
				stream_size = socket.send(buf_conway.data());
				buf_conway.consume(stream_size);

				socket.close();

				if(error)
					if(error != asio::error::eof)
						std::cout << "status: " << error.message() << std::endl;
			}
		}

	} catch(std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}

}