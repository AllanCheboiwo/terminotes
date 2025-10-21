.PHONY: all install clean uninstall

CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lsqlite3
TARGET = notesapp
SOURCES = main.c database.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: $(TARGET)
	sudo ln -sf $(shell pwd)/$(TARGET) /usr/local/bin/$(TARGET)
	@echo "Installed $(TARGET) to /usr/local/bin/$(TARGET)"

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Cleaned up build artifacts"

uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstalled $(TARGET) from /usr/local/bin"

help:
	@echo "Available targets:"
	@echo "  make           - Build the application"
	@echo "  make install   - Install to /usr/local/bin (requires sudo)"
	@echo "  make clean     - Remove build artifacts"
	@echo "  make uninstall - Remove from /usr/local/bin (requires sudo)"
	@echo "  make help      - Show this message"
