# makefile LoggerProject

LIB_DDS= libLOG.so
TARGET= logger # Name of a file(e.g. executable or object files).
CC= gcc # Program for compiling C programs.
INCLUDES =
CFLAGS= -g -Wall -ansi -pedantic -fPIC# Extra flags to give to the C compiler.
OBJS= zlog4c.o

$(TARGET): $(OBJS)
	$(CC) -o $(LIB_DDS) -shared $(OBJS)
	export LD_LIBRARY_PATH=.
	
zlog4c.o: zlog4c.c zlog4c.h
	$(CC) $(CFLAGS) -c zlog4c.c

clean:
	rm -f $(TARGET) $(OBJS) # Command to remove a file.


