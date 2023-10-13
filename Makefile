TARGET=battleship
OBJS=battleship.o game.o rand.o
CFLAGS+=-std=c99 -Wall -Wextra -g

$(TARGET): $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)

.PHONY: clean
