CC=gcc
CFLAGS= -Wall -Wextra -g -O3 -std=c99
SFLAGS= -Wall -Wextra -g -O3 -std=c99 -fsanitize=address,undefined
OBJS = *.c
TARGET = migrafo

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

sanitario:
	$(CC) $(SFLAGS) $(OBJS) -o $(TARGET)

.PHONY: clean
clean:
	@rm -f *.o $(TARGET)
