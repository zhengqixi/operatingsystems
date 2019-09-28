all: symboltable.o parser.o linker.o
	g++ -o linker lab1.m.cpp symboltable.o parser.o linker.o
debug:
	g++ -ggdb -o linker lab1.m.cpp symboltable.cpp symboltable.h parser.cpp parser.h linker.cpp linker.h
symboltable.out:
	g++ -ggdb -o symboltable.out symboltable.m.cpp symboltable.h symboltable.cpp
symboltable.o:
	g++ -c symboltable.h symboltable.cpp
parser.out:
	g++ -ggdb -o parser.out parser.m.cpp parser.cpp parser.h
parser.o:
	g++ -c parser.h parser.cpp
linker.o:
	g++ -c linker.h linker.cpp
clean:
	rm *.out *.o *.gch linker
lab1.tar:
	tar -cvf lab1.tar *.cpp *.h Makefile