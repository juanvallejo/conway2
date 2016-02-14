Conway2
======

Conway's Game of Life implementation in C++. Removes overhead by using a one-dimensional boolean array. Builds in OSX and Linux (Tested on Ubuntu).

Requires a terminal with at least `90x20` columns / rows to display properly.

###Ubuntu

- Requires the following dependencies:

```
sudo apt-get install libncurses-dev
```

###OSX

- Requires the following dependencies:

```
brew install ncurses
```

###Building

```
make
```

###Running

```
./bin/life2
```

##TODO

Make each client render part of its grid
