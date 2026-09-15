
CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinc

TARGET = threadpool

SRC = src/threadpool.c
OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)



