EXEC := yafm
CC := gcc
CFLAGS := -std=gnu99 -Wall -Wextra -g -pedantic -O2
LDLIBS := -lncurses

INCDIR := inc
SRCDIR := src
EDITDIR := editor

SOURCES := $(wildcard $(SRCDIR)/*.c)
INCLUDES := -I $(INCDIR)

all: clean $(EXEC)

$(EXEC):
	cd $(EDITDIR) && make && cd ../
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) $(LDLIBS) -o $(EXEC)

valrun:
	valgrind -v --leak-check=full --show-leak-kinds=all ./$(EXEC)

clean:
	cd $(EDITDIR) && make clean && cd ../ && rm -f $(EXEC)
