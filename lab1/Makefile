linker.out: symboltable.o parser.o linker.o
	g++ -o lab1.m.cpp linker.out symboltable.o parser.o linker.o
symboltable.o:
	g++ -c symboltable.h symboltable.cpp
parser.o:
	g++ -c parser.h parser.cpp
linker.o:
	g++ -c linker.h linker.cpp
clean:
	rm *.out *.o *.gch