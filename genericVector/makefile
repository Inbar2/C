# makefile dynamicDS

TARGET= vectorGenFile #Name of a file(e.g. executable or object files).
#Program for compiling C programs.
CC=gcc
INC_PATH = ../includeGen
CFLAGS= -g -Wall -ansi -pedantic #Extra flags to give to the C compiler.
CPPFLAGS=$(addprefix -I,$(INC_PATH))

OBJS= vectorGenericMain.o vectorGeneric.o 


$(TARGET):  $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

vectorGeneric.o: vectorGeneric.c $(INC_PATH)/vectorGeneric.h
# no need to write recipe, since we are based on implicit rule
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

	
vectorGenericMain.o: vectorGenericMain.c $(INC_PATH)/vectorGeneric.h
# no need to write recipe, since we are based on implicit rule	
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

	
clean:
	rm -f $(TARGET) $(OBJS) #Command to remove a file.
	
run:
	./$(TARGET)

