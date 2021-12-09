# makefile for mmn 14 - final project

#Name of the executable file.
TARGET= assembler

#Program for compiling C programs.
CC= gcc

#Extra flags to give to the C compiler. I added the -m32 because my computer is 64bit. -g flag is for debugging.
CFLAGS= -g -Wall -ansi -pedantic -m32 -lm#Extra flags to give to the C compiler. I added the -m32 because my computer is 64bit. -g flag is for debugging. -lm is for using library math.h.

#Name of a file (e.g. object files).
OBJS= assembler.o fileProcess.o firstPass.o errors.o labelAnalysis.o symbolTable.o instructions.o directives.o generalFuncs.o genericListDS.o secondPass.o outputFiles.o

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET)
	
assembler.o: assembler.c constants.h fileProcess.h
	$(CC) -c $(CFLAGS) $<

fileProcess.o: fileProcess.c fileProcess.h constants.h generalFuncs.h errors.h symbolTable.h firstPass.h genericListDS.h directives.h instructions.h outputFiles.h
	$(CC) -c $(CFLAGS) $<
		
firstPass.o: firstPass.c firstPass.h generalFuncs.h errors.h constants.h symbolTable.h labelAnalysis.h directives.h instructions.h
	$(CC) -c $(CFLAGS) $<
	
errors.o: errors.c errors.h
	$(CC) -c $(CFLAGS) $<

labelAnalysis.o: labelAnalysis.c labelAnalysis.h constants.h errors.h symbolTable.h generalFuncs.h directives.h instructions.h
	$(CC) -c $(CFLAGS) $<

symbolTable.o: symbolTable.c symbolTable.h constants.h
	$(CC) -c $(CFLAGS) $<
	
directives.o: directives.c directives.h constants.h labelAnalysis.h symbolTable.h errors.h generalFuncs.h genericListDS.h
	$(CC) -c $(CFLAGS) $<

instructions.o: instructions.c instructions.h constants.h errors.h #include generalFuncs.h labelAnalysis.h genericListDS.h
	$(CC) -c $(CFLAGS) $<

generalFuncs.o: generalFuncs.c generalFuncs.h constants.h errors.h
	$(CC) -c $(CFLAGS) $<

genericListDS.o: genericListDS.c genericListDS.h 
	$(CC) -c $(CFLAGS) $<

secondPass.o: secondPass.c secondPass.h constants.h labelAnalysis.h instructions.h symbolTable.h errors.h generalFuncs.h genericListDS.h
	$(CC) -c $(CFLAGS) $<
	
outputFiles.o: outputFiles.c outputFiles.h constants.h instructions.h symbolTable.h genericListDS.h
	$(CC) -c $(CFLAGS) $<

	
clean:
	rm -f $(TARGET) $(OBJS) #Command to remove obj file/s.
	
run:
	./$(TARGET) InputMmn14.as
run2:
	./$(TARGET) instTesting.as 2ndPassErrors.as myErrorTest.as InputMmn14.as InputMmn14.as testMe.as notExist.as EmptyFile.as noSUFFIX wrongSuff.ob
