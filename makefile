CC = gcc
CFLAGS = -std=gnu99 -Wall -g -DTDS_MEMORY_DEBUG
LDFLAGS = -ltds -lGL -ldl -lm -lglfw -lopenal -lSOIL -lcairo

SOURCES = $(wildcard src/*.c src/tds_game/*.c src/objects/*.c)
OBJECTS = $(SOURCES:.c=.o)

OUTPUT = hunter-game

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(OUTPUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

cleanbin: clean
	rm -f $(OUTPUT)

clean:
	rm -f $(OBJECTS)
