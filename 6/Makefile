CC = gcc
CFLAGS = -Wall 
SRC = part2.c utils.c running_time_counter.c
TARGET = part2

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) -lm

clean:
	rm -f $(TARGET)
