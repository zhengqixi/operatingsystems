all: symboltable.o parser.o linker.o
	g++ -o linker.out lab1.m.cpp symboltable.o parser.o linker.o
symboltable.out: symboltable.o
	g++ -o symboltable.out symboltable.m.cpp symboltable.o 
symboltable.o:
	g++ -c symboltable.h symboltable.cpp
parser.out: parser.o
	g++ -o parser.out parser.m.cpp parser.o 
parser.o:
	g++ -c parser.h parser.cpp
linker.o:
	g++ -c linker.h linker.cpp
clean:
	rm *.out *.o *.gch