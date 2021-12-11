# makefile for mmn 12 - dynamic DS

#Name of the executable file.
TARGET= myset

#Program for compiling C programs.
CC= gcc

#Extra flags to give to the C compiler. I added the -m32 because my computer is 64bit. -g flag is for debugging.
CFLAGS= -g -Wall -ansi -pedantic -m32

#Name of a file (e.g. object files).
OBJS= myset.o set.o syntax.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

myset.o: myset.c set.h syntax.h
	$(CC) -c $(CFLAGS) $<
	
set.o: set.c set.h
	$(CC) -c $(CFLAGS) $<

syntax.o: syntax.c set.h syntax.h
	$(CC) -c $(CFLAGS) $<


clean:
	rm -f $(TARGET) $(OBJS) #Command to remove a file/s.

run:
	./$(TARGET) < checkProgInput.txt > progOutput.txt
	
run2:
	./$(TARGET) < testFailInput.txt > FailOutput.txt #run the program

