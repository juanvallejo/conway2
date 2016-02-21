#include <iostream>
#include <ctime>
#include <ncurses.h>
#include <signal.h>
#include <time.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include <asio.hpp>
#include "controller.h"
#include "server_controller.h"
#include "client_controller.h"

std::mutex controller::cmutex;

timespec controller::timer = {0};
controller::Grid controller::grid_display = controller::Grid::GRID_DISPLAY_ALL;
bool* controller::temp_game_matrix = NULL;

int controller::game_width = 0;
int controller::game_height = 0;

bool* controller::game_matrix = 0;

void controller::sigint_handler(int sig) {
	endwin();
	exit(0);
}

/**
 * Draw 2d grid to screen given a one-dimensional boolean
 * array, a width, and height
 * TODO: decouple partial matrix draw from function
 */
void controller::draw(bool* matrix, int width, int height) {

	clear();

	for(int i = 0; i < width * height; i++) {
		printw("%3s", matrix[i] ? " x " : "   ");
		if((i + 1) % width == 0) printw("\n");
	}

	refresh();

}

// replaces old draw, renamed to calc_grid, which should be run on its own thread
// takes full matrix, transforms into new 
void controller::calc_grid(bool* matrix, int width, int height) {

	for(int x = 0; x < height; x++) {
		for(int y = 0; y < width; y++) {

			int live_n = 0;

			// check top neighbor
			if(x > 1) {
				if(matrix[(x - 1) * width + y]) { live_n++; }
				// check top left neighbor
				if(y > 1) { if(matrix[(x - 1) * width + (y - 1)]) { live_n++; } }
				// check top right neighbor
				if(y < width - 1) { if(matrix[(x - 1) * width + (y + 1)]) { live_n++; } }
			}

			// check bottom neighbor
			if(x < height - 1) {
				if(matrix[(x + 1) * width + y]) { live_n++; }
				// check bottom left neighbor
				if(y > 1) { if(matrix[(x + 1) * width + (y - 1)]) { live_n++; } }
				// check bottom right neighbor
				if(y < width - 1) { if(matrix[(x + 1) * width + (y + 1)]) { live_n++; } }
			}

			// check left neighbor
			if(y > 1) { if(matrix[x * width + (y - 1)]) { live_n++; } }
			// check right neighbor
			if(y < width - 1) { if(matrix[x * width + (y + 1)]) { live_n++; } }

			if(live_n < 2 || live_n > 3) { matrix[x * width + y] = false; }
			if(live_n == 3) { matrix[x * width + y] = true; }
		}
	}

}

// returns full game matrix, or a partial
// game matrix depending on the value of grid_display
bool* controller::get_partial_game_matrix(bool* matrix, int width, int height) {

	if(controller::grid_display == controller::Grid::GRID_DISPLAY_ALL)
		return matrix;

	int off_x = 0;
	int off_y = 0;

	int matrix_cell = 0;
	
	int n_width = width / 2;
	int n_height = height / 2;

	bool* n_matrix = controller::set_temp_game_matrix(new bool[n_width * n_height]);
	memset(n_matrix, false, n_width * n_height);

	if(controller::grid_display == controller::Grid::GRID_DISPLAY_TR)
		off_y = n_width;
	if(controller::grid_display == controller::Grid::GRID_DISPLAY_BL)
		off_x = n_height;
	if(controller::grid_display == controller::Grid::GRID_DISPLAY_BR) {
		off_y = n_width;
		off_x = n_height;
	}

	for(int i = 0; i < (width * height); i++) {		
		if(i % width >= off_y && i % width < off_y + n_width) {
			if(std::floor(i / width) >= off_x && std::floor(i / width) < off_x + n_height) {
				matrix_cell = ((std::floor(i / width) - off_x) * n_width) + (i % n_width);
				n_matrix[matrix_cell] = matrix[i];
			}
		}
	}

	return n_matrix;

}

int controller::get_partial_game_width() {
	if(controller::grid_display == controller::Grid::GRID_DISPLAY_ALL)
		return controller::get_game_width();
	return controller::get_game_width() / 2;
}

int controller::get_partial_game_height() {
	if(controller::grid_display == controller::Grid::GRID_DISPLAY_ALL)
		return controller::get_game_height();
	return controller::get_game_height() / 2;
}

bool* controller::set_temp_game_matrix(bool* n_matrix) {
	if(controller::temp_game_matrix && controller::temp_game_matrix != NULL) 
		delete temp_game_matrix;
	controller::temp_game_matrix = n_matrix;
	return controller::temp_game_matrix;
}

void controller::init_game(const int width, const int height) {

	std::srand(std::time(0));

	int ns_width = width;
	int ns_height = height;

	// ncurses init
	signal(SIGINT, sigint_handler);
	initscr();

	if(CONSTRAIN_GRID) {
		getmaxyx(stdscr, ns_width, ns_height);
	}
	
	controller::set_game_width(std::min(width, ns_width));
	controller::set_game_height(std::min(height, ns_height));

	controller::set_game_matrix(new bool[controller::get_game_width() * controller::get_game_height()]);
	memset(controller::get_game_matrix(), false, controller::get_game_width() * controller::get_game_height());

	// set random cells alive
	for(int i = 0; i < controller::get_game_width() * controller::get_game_height(); i++) {
		if(std::rand() % 100 < 15) { controller::get_game_matrix()[i] = true; }
	}

	while(true) {
		if(controller::grid_display != controller::Grid::GRID_DISPLAY_NO)
			controller::draw(controller::get_partial_game_matrix(), controller::get_partial_game_width(), controller::get_partial_game_height());
		controller::calc_grid(controller::get_game_matrix(), controller::get_game_width(), controller::get_game_height());
		controller::sleep(229);
	}

	endwin();

}

void controller::init_server(const int width, const int height) {
	std::thread tserver(server::init, DEFAULT_TCP_PORT);
	std::thread tgame(controller::init_game, width, height);
	tserver.join();
	tgame.join();
}

void controller::init_client() {
	client::init(DEFAULT_TCP_PORT);
}

timespec* controller::get_timer(int timeout) {
	if(timer.tv_nsec == timeout * 1000000L) { return &timer; }
	timer.tv_sec = 0;
	timer.tv_nsec = timeout * 1000000L;
	return &timer;
}

void controller::sleep(int timeout) {
	nanosleep(get_timer(timeout), (struct timespec *)NULL);
}