#ifndef SERVER_CONTROLLER_H
 #define SERVER_CONTROLLER_H

#include <mutex>
#include <asio.hpp>

class server {
public:
	static bool ACCEPT_CLIENT_CONN;
	static std::mutex smutex;
	static void init(const short);

	/**
	 * Loads conway's array values into a string buffer
	 */
	static void load_game_data(std::ostream&);
};

#endif