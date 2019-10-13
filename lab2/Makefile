all: event.h eventqueue.h process.o randomgenerator.o simulation.o lab2.m.cpp
	g++ -o sched event.h eventqueue.h process.o randomgenerator.o simulation.o lab2.m.cpp
simulation.o: simulation.h simulation.cpp
	g++ -c simulation.h simulation.cpp
randomgenerator.o: randomgenerator.cpp randomgenerator.h
	g++ -c randomgenerator.h randomgenerator.cpp
process.o: process.h process.cpp
	g++ -c process.h process.cpp
eventqueue.out: eventqueue.h event.h eventqueue.m.cpp
	g++ -ggdb -o eventqueue.out event.h eventqueue.h eventqueue.m.cpp
randomgenerator.out: randomgenerator.h randomgenerator.cpp randomgenerator.m.cpp
	g++ -ggdb -o randomgenerator.out randomgenerator.h randomgenerator.cpp randomgenerator.m.cpp
process.out: process.h process.cpp process.m.cpp
	g++ -ggdb -o process.out process.h process.cpp process.m.cpp
clean:
	rm -f *.out *.o *.gch