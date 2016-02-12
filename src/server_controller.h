#ifndef SERVER_CONTROLLER_H
 #define SERVER_CONTROLLER_H

#include <mutex>
#include <asio.hpp>

class server {
public:
	static std::mutex smutex;
	static void init(const short);
};

#endif