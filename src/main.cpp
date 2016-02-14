#include <iostream>
#include "controller.h"

int main(int argc, char** argv) {

	if(argc > 1 && (std::strcmp(argv[1], "--server") || std::strcmp(argv[1], "-s"))) {
		controller::init_server(16, 16);
		return 0;
	}

	controller::init_client();
	return 0;
}