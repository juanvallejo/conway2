# ------------------------------------------------
# Generic Makefile
#
# Author: Juan Vallejo
# Date  : 2016-02-01
#
# Changelog :
#   2010-11-05 - first version
#   2011-08-10 - added structure : sources, objects, binaries
#                thanks to http://stackoverflow.com/users/128940/beta
#   2015-03-30 - CNU UAS Lab version
#   2015-11-02 - Project Pulusan version
# ------------------------------------------------

OS := $(shell uname)

# project name (generate executable with this name)
TARGET   = life2

CC       = g++
# compiling flags here
CXXFLAGS   += -Wall -I. -std=c++11 -g

# libs
LIBS = ncurses

# linking flags here
LFLAGS   = -Wall -I. -lm -L /usr/local/lib
LDLIBS  := $(addprefix -l,$(LIBS))

ifeq ($(OS), Darwin)
LFLAGS += -L/usr/local/opt/ncurses/lib
endif

# change these to set the proper directories where each files shoould be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)

ifeq ($(OS), Darwin)
INCLUDE_DIR := /usr/local/opt/ncurses/include
else
INCLUDE_DIR := /usr/include
endif

CFLAGS   += $(foreach includedir,$(INCLUDE_DIR),-I$(includedir))
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm       = rm -f

all: directories $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(LINK.cc) $^ -o $@ $(LFLAGS) $(LDLIBS)
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp | $(OBJDIR)
	@$(CC) $(CXXFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

directories:
	@mkdir -p $(BINDIR)
	@mkdir -p $(OBJDIR)

.PHONEY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONEY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

.PHONY: check-syntax
check-syntax:
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o nul -S ${CHK_SOURCES}