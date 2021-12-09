# makefile


TARGET= HeapGenTests #Name of a file(e.g. executable or object files).
#Program for compiling C programs.
CC= gcc
INC_PATH= ../includeGen
HEAP_PATH= ../genericHeap
VECTOR_PATH= ../genericVector
CFLAGS= -g -Wall -ansi -pedantic#Extra flags to give to the C compiler.
CPPFLAGS=$(addprefix -I,$(INC_PATH))
OBJS= vectorGeneric.o genHeap.o heapGenMainTests.o heapGenTest.o

.PHONY: all clean

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET)


vectorGeneric.o: $(VECTOR_PATH)/vectorGeneric.c $(INC_PATH)/vectorGeneric.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

genHeap.o: $(HEAP_PATH)/genHeap.c $(INC_PATH)/genHeap.h $(INC_PATH)/vectorGeneric.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

heapGenMainTests.o: heapGenMainTests.c heapGenTest.h $(INC_PATH)/AutoTestsEnum.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<
	
heapGenTest.o: heapGenTest.c heapGenTest.h $(INC_PATH)/AutoTestsEnum.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<


clean:
	rm -f $(TARGET) $(OBJS) #Command to remove a file.


run:
	./$(TARGET)
