# makefile periodic executor project


TARGET= ExecutorTests #Name of a file(e.g. executable or object files).
#Program for compiling C programs.
CC= gcc
INC_PATH= ../includeGen
LOGGER_PATH= ../Logger
VECTOR_PATH= ../genericVector
HEAP_PATH= ../genericHeap
#add in class computer the flag -m32 to CFLAGS
CFLAGS= -g -Wall -ansi -pedantic -m32 -D_BSD_SOURCE#Extra flags to give to the C compiler.
CPPFLAGS=$(addprefix -I,$(INC_PATH))
OBJS= vectorGenericFunc.o genHeapFunc.o ExecutorTestsMain.o ExecutorTestsFunctions.o Executor.o Task.o  Timer.o zlog4c.o 

.PHONY: all clean

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET)

zlog4c.o: $(LOGGER_PATH)/zlog4c.c $(LOGGER_PATH)/zlog4c.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -I$(LOGGER_PATH) $<

Executor.o: Executor.c Executor.h Task.h Timer.h $(LOGGER_PATH)/zlog4c.h $(INC_PATH)/vectorGenericHead.h $(INC_PATH)/vectorGenericHead.h 
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -I$(LOGGER_PATH) $<

Task.o: Task.c Task.h Timer.h $(LOGGER_PATH)/zlog4c.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -I$(LOGGER_PATH) $<

Timer.o: Timer.c Timer.h  $(LOGGER_PATH)/zlog4c.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS)  -I$(LOGGER_PATH) $<
	
vectorGenericFunc.o: $(VECTOR_PATH)/vectorGenericFunc.c $(INC_PATH)/vectorGenericHead.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

genHeapFunc.o: $(HEAP_PATH)/genHeapFunc.c $(INC_PATH)/genHeapHead.h $(INC_PATH)/vectorGenericHead.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

ExecutorTestsMain.o: ExecutorTestsMain.c ExecutorTestsHead.h $(LOGGER_PATH)/zlog4c.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -I$(LOGGER_PATH) $<
	
ExecutorTestsFunctions.o: ExecutorTestsFunctions.c ExecutorTestsHead.h $(LOGGER_PATH)/zlog4c.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -I$(LOGGER_PATH) $<


clean:
	rm -f $(TARGET) $(OBJS) #Command to remove a file.


run:
	./$(TARGET)
