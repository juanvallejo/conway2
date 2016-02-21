# Conway2

Conway's Game of Life implementation in C++. Removes overhead by using a one-dimensional boolean array. Builds in OSX and Linux (Tested on Ubuntu).

Requires a terminal with at least `90x20` columns / rows to display properly.

## Ubuntu

- Requires the following dependencies:

```
sudo apt-get install libncurses-dev
```

## OSX

- Requires the following dependencies:

```
brew install ncurses
```

## Building

```
make
```

## Running

### Server

The game server is the main `module`. It must be run first, before attempting to connect more `modules` to it.
The example below runs the main game server, displaying the entire grid:

```
./bin/life2 --server
```

The example below runs the main game server, displaying the `top left` part of the grid:

```
./bin/life2 --server --tl
```

### Client

Game clients connect to a `server module`, retrieve grid data from it, and display all, or part of it.
Running a game client with no arguments, makes that client display the entire grid from the `server module`.
The example below runs a game client, displaying the `top right` part of the grid:

```
./bin/life2 --tr
```

### Options

Available grid display options are:

- `--tl` to display the top left part of the grid
- `--tr` to display the top right part of the grid
- `--bl` to display the bottom left part of the grid
- `--br` to display the bottom right part of the grid
- `--no` to prevent the grid from being displayed