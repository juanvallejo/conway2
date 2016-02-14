#ifndef CONTROLLER_H
 #define CONTROLLER_H

const short DEFAULT_TCP_PORT = 8000;

class controller {
private:
	static int game_width;
	static int game_height;
	static bool* game_matrix;

	static struct timespec timer;
public:
	static std::mutex cmutex;

	// initialize server controller
	// handles game data transfer
	static void init_server(const int, const int);
	static void init_client();

	// initialize main game matrix
	// receives a width and height
	static void init_game(const int, const int);

	static void draw(bool*, int, int);

	static inline void set_game_width(int width) { game_width = width; }
	static inline void set_game_height(int height) { game_height = height; }
	static inline void set_game_matrix(bool* matrix) { game_matrix = matrix; }

	static inline int get_game_width() { return game_width; }
	static inline int get_game_height() { return game_height; }

	static inline bool* get_game_matrix() { return game_matrix; }

	static timespec* get_timer(int);
	static void sleep(int);

	static void sigint_handler(int);
};

#endif