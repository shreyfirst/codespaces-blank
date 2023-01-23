CC = g++
CFLAGS = -Wall -Werror
TARGET = SearchNewBooks

all: $(TARGET)

$(TARGET): SearchNewBooks.cc
	$(CC) $(CFLAGS) -o $(TARGET) SearchNewBooks.cc

clean:
	rm -f $(TARGET)
