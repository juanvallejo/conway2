#ifndef CONTROLLER_H
 #define CONTROLLER_H

const short DEFAULT_TCP_PORT = 8000;

#include <mutex>

class controller {
public:
	static std::mutex cmutex;
	static void init_server(const int, const int);
	static void init_client();
};

#endif