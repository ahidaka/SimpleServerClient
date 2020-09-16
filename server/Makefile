#
TARGET = server textserver makedata

CFLAGS = -Wall -g

all: $(TARGET)
	touch make.date

server: server.c
	gcc $(CFLAGS) -o server server.c

textserver: textserver.c
	gcc $(CFLAGS) -o textserver textserver.c

makedata: makedata.c
	gcc $(CFLAGS) -o makedata makedata.c

clean:
	rm -f make.date datafile *.o $(TARGET) *~ core
