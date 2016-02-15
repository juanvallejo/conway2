#include <iostream>
#include "controller.h"

int main(int argc, char** argv) {

	if(argc > 1) {
		if(std::strcmp(argv[1], "--server") == 0 || std::strcmp(argv[1], "-s") == 0) {
			controller::init_server(16, 16);
			return 0;
		}

		// handle init_client for left part of grid
		controller::init_client(argv[1]);
		return 0;
	}

	controller::init_client(NULL);
	return 0;
}