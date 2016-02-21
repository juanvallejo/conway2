#ifndef CONTROLLER_H
 #define CONTROLLER_H

const short DEFAULT_TCP_PORT = 8000;
const bool CONSTRAIN_GRID = false;

class controller {
private:
	static int game_width;
	static int game_height;
	static bool* game_matrix;
	static bool* temp_game_matrix;

	static struct timespec timer;
public:
	static std::mutex cmutex;

	enum Grid {
		GRID_DISPLAY_ALL,
		GRID_DISPLAY_TL,
		GRID_DISPLAY_TR,
		GRID_DISPLAY_BL,
		GRID_DISPLAY_BR
	};

	static Grid grid_display;

	// initialize server controller
	// handles game data transfer
	static void init_server(const int, const int);
	static void init_client();

	// initialize main game matrix
	// receives a width and height
	static void init_game(const int, const int);

	// calculate the state of each cell
	// in the grid based on conway's rules
	static void calc_grid(bool*, int, int);
	static void draw(bool*, int, int);

	static inline void set_game_width(int width) { game_width = width; }
	static inline void set_game_height(int height) { game_height = height; }

	static inline void set_game_matrix(bool* matrix) { game_matrix = matrix; }
	static bool* set_temp_game_matrix(bool* matrix);

	static inline int get_game_width() { return game_width; }
	static inline int get_game_height() { return game_height; }
	
	static inline bool* get_game_matrix() { return game_matrix; }
	static inline bool* get_temp_game_matrix() { return temp_game_matrix; }

	static bool* get_partial_game_matrix(bool* matrix = game_matrix, int width = game_width, int height = game_height);
	static int get_partial_game_width();
	static int get_partial_game_height();

	static timespec* get_timer(int);
	static void sleep(int);

	static void sigint_handler(int);
};

#endif