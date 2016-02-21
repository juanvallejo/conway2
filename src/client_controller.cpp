#include <iostream>
#include <ncurses.h>
#include "controller.h"
#include "client_controller.h"

bool client::REQUEST_SERVER_DATA = true;

void client::init(const short port, enum controller::Grid g_disp) {

	signal(SIGINT, controller::sigint_handler);
	initscr();

	controller::grid_display = g_disp;

	try {
		asio::io_service io_service;
		asio::ip::tcp::socket socket(io_service);
		asio::ip::tcp::resolver resolver(io_service);
        asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(asio::ip::tcp::resolver::query("127.0.0.1", std::to_string(port)));
        
		std::error_code error;
		size_t stream_size;

		std::cout << "Client ready." << (controller::grid_display == controller::Grid::GRID_DISPLAY_ALL ? "" : " Displaying partial grid.") << std::endl;

		char server_data[1024];

		while(REQUEST_SERVER_DATA) {

			memset(server_data, 0, stream_size);
			
			asio::connect(socket, endpoint);
			socket.send(asio::buffer("HEARTBEAT"));
			stream_size = socket.read_some(asio::buffer(server_data), error);
			socket.close();

			client::parse_game_data(server_data);

			if(error)
				if(error != asio::error::eof)
					std::cout << "status: " << error.message() << std::endl;

			controller::sleep(229);
		}

	} catch(std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		endwin();
	}

	endwin();

}

void client::parse_game_data(const char* game_data) {

	char char_w[4];
	char char_h[4];

	int width;
	int height;
	int n_width;
	int n_height;

	strncpy(char_w, game_data, 2);
	strncpy(char_h, &game_data[2], 2);

	width = atoi(char_w);
	height = atoi(char_h);
	n_width = width;
	n_height = height;

	if(controller::grid_display != controller::Grid::GRID_DISPLAY_ALL) {
		n_width = width / 2;
		n_height = height / 2;
	}

	bool* matrix = new bool[width * height];
	memset(matrix, false, width * height);

	for(int i = 4; i < (width * height) + 4; i++) {
		matrix[i - 4] = (game_data[i] == '1');
	}

	controller::draw(controller::get_partial_game_matrix(matrix, width, height), n_width, n_height);
	delete matrix;
}