#

TARGET = tcpread

CFLAGS = -Wall -g

all: $(TARGET)
	touch make.date

tcpread: tcpread.c
	gcc $(CFLAGS) -o tcpread tcpread.c

clean:
	rm -f make.date *.o $(TARGET) *~ core
