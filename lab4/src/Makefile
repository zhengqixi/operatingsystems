all: lab4.m.cpp simulation.o scheduler.o request.o fifo.o
	g++ -o iosched lab4.m.cpp *.o
debug:
	g++ -ggdb -o iosched *.cpp *.h
fifo.o: fifo.h fifo.cpp simulation.h
	g++ -c fifo.h fifo.cpp
simulation.o: simulation.h simulation.cpp
	g++ -c simulation.h simulation.cpp
scheduler.o: scheduler.h scheduler.cpp
	g++ -c scheduler.h scheduler.cpp
request.o: request.h request.cpp
	g++ -c request.h request.cpp
zip: *.cpp *.h Makefile README.md
	tar -cvf lab4.tar *.cpp *.h Makefile README.md
clean:
	rm -f *.out *.o *.gch lab4.tar iosched