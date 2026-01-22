CC = gcc
CFLAGS = -Wall -Wextra -O2

TARGET = minesweeper
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
