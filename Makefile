CC = g++
INCLUDE	= -I/usr/local/include
CFLAGS	= -Wall $(INCLUDE) -Winline -pipe
LDFLAGS	= -L/usr/local/lib
LDLIBS  = -lwiringPi -lwiringPiDev -lpthread -lm

SRC = main.cpp PCD8544.cpp
OBJ = main.o PCD8544.o
BIN = orangepipc-lcd-bandwidth-monitor

%.o: src/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(OBJ) $(BIN)
