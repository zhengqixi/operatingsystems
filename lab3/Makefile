all: simulation.o pagehandler.o frame.o process.o page.o
	g++ -o mmu lab3.m.cpp *.o
simulation.o: simulation.h simulation.cpp page.h pagehandler.h process.h
	g++ -c simulation.h simulation.cpp
pagehandler.o: pagehandler.h pagehandler.cpp frame.h
	g++ -c pagehandler.h pagehandler.cpp
frame.o: frame.h frame.cpp
	g++ -c frame.h frame.cpp
process.o: process.h process.cpp
	g++ -c process.h process.cpp
process.out: page.o process.h process.cpp process.t.cpp
	g++ -g -o process.out process.h process.cpp process.t.cpp page.o
page.o: page.h page.cpp
	g++ -c page.h page.cpp
page.out: page.h page.cpp page.t.cpp
	g++ -g -o page.out page.h page.cpp page.t.cpp
zip: *.cpp *.h Makefile README.md
	tar -cvf lab3.tar *.cpp *.h Makefile README.md
clean:
	rm -f *.out *.o *.gch lab3.tar mmu