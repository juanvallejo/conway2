#ifndef CLIENT_CONTROLLER_H
 #define CLIENT_CONTROLLER_H

#include <asio.hpp>

class client {
public:

	enum Grid {
		GRID_DISPLAY_ALL,
		GRID_DISPLAY_TL,
		GRID_DISPLAY_TR,
		GRID_DISPLAY_BL,
		GRID_DISPLAY_BR
	};

	static Grid grid_display;

	static bool REQUEST_SERVER_DATA;
	static void init(const short, enum client::Grid);
	static void parse_game_data(const char*);
};

#endif