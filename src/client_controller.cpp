#include <iostream>
#include <ncurses.h>
#include "controller.h"
#include "client_controller.h"

bool client::REQUEST_SERVER_DATA = true;
client::Grid client::grid_display = client::Grid::GRID_DISPLAY_ALL;

void client::init(const short port, enum client::Grid g_disp) {

	signal(SIGINT, controller::sigint_handler);
	initscr();

	client::grid_display = g_disp;

	try {
		asio::io_service io_service;
		asio::ip::tcp::socket socket(io_service);
		asio::ip::tcp::resolver resolver(io_service);
        asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(asio::ip::tcp::resolver::query("127.0.0.1", std::to_string(port)));
        
		std::error_code error;
		size_t stream_size;

		std::cout << "Client ready." << (client::grid_display == client::Grid::GRID_DISPLAY_ALL ? "" : " Displaying partial grid.") << std::endl;

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

	bool partial = false;

	char char_w[4];
	char char_h[4];

	int width;
	int height;
	int n_width;
	int n_height;
	int matrix_cell;

	int off_x = 0;
	int off_y = 0;

	strncpy(char_w, game_data, 2);
	strncpy(char_h, &game_data[2], 2);

	width = atoi(char_w);
	height = atoi(char_h);
	n_width = width;
	n_height = height;

	if(client::grid_display != client::Grid::GRID_DISPLAY_ALL) {
		partial = true;
		n_width = width / 2;
		n_height = height / 2;
	}

	bool* matrix = new bool[n_width * n_height];
	memset(matrix, false, n_width * n_height);

	if(client::grid_display == client::Grid::GRID_DISPLAY_TR)
		off_y = n_width;
	if(client::grid_display == client::Grid::GRID_DISPLAY_BL)
		off_x = n_height;
	if(client::grid_display == client::Grid::GRID_DISPLAY_BR) {
		off_y = n_width;
		off_x = n_height;
	}

	for(int i = 4; i < (width * height) + 4; i++) {		
		if(partial) {
			if((i - 4) % width >= off_y && (i - 4) % width < off_y + n_width) {
				if(std::floor((i - 4) / width) >= off_x && std::floor((i - 4) / width) < off_x + n_height) {
					matrix_cell = ((std::floor((i - 4) / width) - off_x) * n_width) + ((i - 4) % n_width);
					matrix[matrix_cell] = (game_data[i] == '1');
				}
			}
		} else
			matrix[i - 4] = (game_data[i] == '1');
	}

	// std::cout << "off_x " << off_x << " off_y" << off_y << std::endl;
	controller::draw(matrix, n_width, n_height);
	delete matrix;
}