CC = clang
CFLAGS = -Iinclude -Wall -Wextra -Werror
SRC = main.c src/file.c
OBJ = $(SRC:.c=.o)
TARGET = output

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
