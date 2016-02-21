#include <iostream>
#include "controller.h"

void set_grid_display(char* g_disp) {
	if(std::strcmp(g_disp, "--tl") == 0)
		controller::grid_display = controller::Grid::GRID_DISPLAY_TL;
	if(std::strcmp(g_disp, "--tr") == 0)
		controller::grid_display = controller::Grid::GRID_DISPLAY_TR;
	if(std::strcmp(g_disp, "--bl") == 0)
		controller::grid_display = controller::Grid::GRID_DISPLAY_BL;
	if(std::strcmp(g_disp, "--br") == 0)
		controller::grid_display = controller::Grid::GRID_DISPLAY_BR;
}

int main(int argc, char** argv) {
	
	if(argc > 1) {
		if(std::strcmp(argv[1], "--server") == 0 || std::strcmp(argv[1], "-s") == 0) {
			if(argc > 2) set_grid_display(argv[2]);
			controller::init_server(16, 16);
			return 0;
		}

		set_grid_display(argv[1]);
	}

	controller::init_client();
	return 0;
}