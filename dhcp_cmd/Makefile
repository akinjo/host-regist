CC=gcc
CFLAGS= -g -Wall
CLIBS= -ldhcpctl -lomapi -ldhcp
TARGET=dhcprequest

all:$(TARGET)
.c:
	$(CC) $(CFLAGS)  -o $@ $< $(CLIBS)
clean:
	rm -rf $(TARGET) *~
