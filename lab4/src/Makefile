all: lab4.m.cpp simulation.o scheduler.o request.o fifo.o srtf.o lookbase.o look.o flook.o clook.o
	g++ -o iosched lab4.m.cpp *.o
debug:
	g++ -ggdb -o iosched *.cpp *.h
clook.o: clook.h clook.cpp scheduler.h
	g++ -c clook.h clook.cpp
flook.o: flook.h flook.cpp lookbase.h
	g++ -c flook.h flook.cpp
look.o: look.h look.cpp lookbase.h
	g++ -c look.h look.cpp
lookbase.o: lookbase.h lookbase.cpp simulation.h
	g++ -c lookbase.h lookbase.cpp
srtf.o: srtf.h srtf.cpp simulation.h
	g++ -c srtf.h srtf.cpp
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