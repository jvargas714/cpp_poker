vpath %.cpp sources
vpath %.h headers

# include directories
DIR1 = -I./headers
DIRS = $(DIR1)

#compiler and linker options
CC = g++ 
DEBUG = -g
CFLAGS = $(DIRS) -Wall -c $(DEBUG) -std=c++11
LFLAGS = $(DIRS) -Wall $(DEBUG) -std=c++11

#specify source files and target files 
SOURCES = player.cpp Poker.cpp TexasHoldem.cpp card.cpp deck.cpp poker_error.cpp\
	jsoncpp.cpp poker_fsm.cpp bot_player.cpp
OBJS = $(subst .cpp,.o, $(SOURCES))
LIBFILE = libpoker.a
INSTALLDIR = ~/Documents/Dev/Poker/lib
EXEC = test_suite.cpp #test_cfg.cpp #test_cfg.cpp #main2.cpp
LIBS = -L./lib/

.SUFFIXES: .cpp

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(LIBFILE) exe
	
$(LIBFILE): $(OBJS)
	ar rcvs $(LIBFILE) $(OBJS)
	mv $(LIBFILE) ./lib

exe:
	g++ -std=c++11 $(DIRS) -static ./sources/$(EXEC) $(LIBS) -o poker_exe -lpoker

.PHONY: install
install:
	make
	mv $(LIBFILE) $(INSTALLDIR)

.PHONY: clean
clean:
	rm --verbose *.o ./lib/*.a poker_exe

.PHONY: help
help:
	@clear
	@ls -R -latrh
	@echo "Sources:: $(SOURCES)"
	@echo "Objects:: $(OBJS)"