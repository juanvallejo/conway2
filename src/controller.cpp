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

int controller::game_width = 0;
int controller::game_height = 0;

bool* controller::game_matrix = 0;

void sigint_handler(int sig) {
	endwin();
	exit(0);
}

void draw(bool* matrix, int width, int height) {

	const char LINEB[] = "\n";

	// setup nanosleep
	int sleep_m = 328;
	struct timespec sleep = {0};
	sleep.tv_sec = 0;
	sleep.tv_nsec = sleep_m * 1000000L;

	while(true) {
		clear();

		for(int x = 0; x < height; x++) {
			for(int y = 0; y < width; y++) {
				printw("%3s", matrix[x * width + y] ? " x " : "   ");

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
			printw(LINEB);
		}

		refresh();
		nanosleep(&sleep, (struct timespec *)NULL);
	}

}

void controller::init_game(const int width, const int height) {

	std::srand(std::time(0));

	int ns_width;
	int ns_height;

	// ncurses init
	signal(SIGINT, sigint_handler);
	initscr();
	getmaxyx(stdscr, ns_width, ns_height);

	controller::set_game_width(std::min(width, ns_width));
	controller::set_game_height(std::min(height, ns_height));

	controller::set_game_matrix(new bool[controller::get_game_width() * controller::get_game_height()]);
	memset(controller::get_game_matrix(), false, controller::get_game_width() * controller::get_game_height());

	// set random cells alive
	for(int i = 0; i < controller::get_game_width() * controller::get_game_height(); i++) {
		if(std::rand() % 100 < 15) { controller::get_game_matrix()[i] = true; }
	}

	draw(controller::get_game_matrix(), controller::get_game_width(), controller::get_game_height());
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
