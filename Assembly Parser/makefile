# makefile for mmn 14 - final project

#Name of the executable file.
TARGET= assembler

#Program for compiling C programs.
CC= gcc
INC_PATH= ./include
INPUT_PATH= ./InputFiles

#Extra flags to give to the C compiler. I added the -m32 because my computer is 64bit. -g flag is for debugging.
CFLAGS= -g -Wall -ansi -pedantic -m32 -lm#Extra flags to give to the C compiler. I added the -m32 because my computer is 64bit. -g flag is for debugging. -lm is for using library math.h.
CPPFLAGS=$(addprefix -I,$(INC_PATH))
#Name of a file (e.g. object files).
OBJS= assembler.o fileProcess.o firstPass.o errors.o labelAnalysis.o symbolTable.o instructions.o directives.o generalFuncs.o genericListDS.o secondPass.o outputFiles.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET)
	
assembler.o: assembler.c $(INC_PATH)/constants.h $(INC_PATH)/fileProcess.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

fileProcess.o: fileProcess.c $(INC_PATH)/fileProcess.h $(INC_PATH)/constants.h $(INC_PATH)/generalFuncs.h $(INC_PATH)/errors.h $(INC_PATH)/symbolTable.h $(INC_PATH)/firstPass.h $(INC_PATH)/genericListDS.h $(INC_PATH)/directives.h $(INC_PATH)/instructions.h $(INC_PATH)/outputFiles.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<
		
firstPass.o: firstPass.c $(INC_PATH)/firstPass.h $(INC_PATH)/generalFuncs.h $(INC_PATH)/errors.h $(INC_PATH)/constants.h $(INC_PATH)/symbolTable.h $(INC_PATH)/labelAnalysis.h $(INC_PATH)/directives.h $(INC_PATH)/instructions.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<
	
errors.o: errors.c $(INC_PATH)/errors.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

labelAnalysis.o: labelAnalysis.c $(INC_PATH)/labelAnalysis.h $(INC_PATH)/constants.h $(INC_PATH)/errors.h $(INC_PATH)/symbolTable.h $(INC_PATH)/generalFuncs.h $(INC_PATH)/directives.h $(INC_PATH)/instructions.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

symbolTable.o: symbolTable.c $(INC_PATH)/symbolTable.h $(INC_PATH)/constants.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<
	
directives.o: directives.c $(INC_PATH)/directives.h $(INC_PATH)/constants.h $(INC_PATH)/labelAnalysis.h $(INC_PATH)/symbolTable.h $(INC_PATH)/errors.h $(INC_PATH)/generalFuncs.h $(INC_PATH)/genericListDS.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

instructions.o: instructions.c $(INC_PATH)/instructions.h $(INC_PATH)/constants.h $(INC_PATH)/errors.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

generalFuncs.o: generalFuncs.c $(INC_PATH)/generalFuncs.h $(INC_PATH)/constants.h $(INC_PATH)/errors.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

genericListDS.o: genericListDS.c $(INC_PATH)/genericListDS.h 
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

secondPass.o: secondPass.c $(INC_PATH)/secondPass.h $(INC_PATH)/constants.h $(INC_PATH)/labelAnalysis.h $(INC_PATH)/instructions.h $(INC_PATH)/symbolTable.h $(INC_PATH)/errors.h $(INC_PATH)/generalFuncs.h $(INC_PATH)/genericListDS.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<
	
outputFiles.o: outputFiles.c $(INC_PATH)/outputFiles.h $(INC_PATH)/constants.h $(INC_PATH)/instructions.h $(INC_PATH)/symbolTable.h $(INC_PATH)/genericListDS.h
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

	
clean:
	rm -f $(TARGET) $(OBJS) #Command to remove obj file/s.
	
run:
	./$(TARGET) ./$(INPUT_PATH)/InputMmn14.as ./$(INPUT_PATH)/EmptyFile.as notExist.as noSUFFIX wrongSuff.ob
run2:
	./$(TARGET) ./$(INPUT_PATH)/instTesting.as ./$(INPUT_PATH)/2ndPassErrors.as ./$(INPUT_PATH)/myErrorTest.as ./$(INPUT_PATH)/InputMmn14.as ./$(INPUT_PATH)/testMe.as ./$(INPUT_PATH)/EmptyFile.as 
	
