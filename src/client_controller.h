#ifndef CLIENT_CONTROLLER_H
 #define CLIENT_CONTROLLER_H

#include <asio.hpp>

class client {
public:
	static bool REQUEST_SERVER_DATA;
	static void init(const short);
	static void parse_game_data(const char*);
};

#endif