all: lab2.m.cpp process.o randomgenerator.o simulation.o fifoscheduler.o lifoscheduler.o srtfscheduler.o prioscheduler.o preprioscheduler.o
	g++ -o sched process.o randomgenerator.o simulation.o lab2.m.cpp fifoscheduler.o lifoscheduler.o srtfscheduler.o prioscheduler.o preprioscheduler.o
debug: *.cpp *.h 
	g++ -ggdb -o sched lab2.m.cpp simulation.h simulation.cpp process.h process.cpp randomgenerator.h randomgenerator.cpp eventqueue.h abstractscheduler.h fifoscheduler.h fifoscheduler.cpp lifoscheduler.h lifoscheduler.cpp srtfscheduler.h srtfscheduler.cpp prioscheduler.h prioscheduler.cpp preprioscheduler.h preprioscheduler.cpp
simulation.o: simulation.h simulation.cpp eventqueue.h
	g++ -c simulation.h simulation.cpp
srtfscheduler.o: srtfscheduler.h srtfscheduler.cpp eventqueue.h abstractscheduler.h
	g++ -c srtfscheduler.h srtfscheduler.cpp
fifoscheduler.o: fifoscheduler.h fifoscheduler.cpp abstractscheduler.h
	g++ -c fifoscheduler.h fifoscheduler.cpp
lifoscheduler.o: lifoscheduler.h lifoscheduler.cpp abstractscheduler.h
	g++ -c lifoscheduler.h lifoscheduler.cpp
prioscheduler.o: prioscheduler.h prioscheduler.cpp abstractscheduler.h
	g++ -c prioscheduler.h prioscheduler.cpp
preprioscheduler.o: preprioscheduler.h preprioscheduler.cpp prioscheduler.h abstractscheduler.h
	g++ -c preprioscheduler.h preprioscheduler.cpp
randomgenerator.o: randomgenerator.cpp randomgenerator.h
	g++ -c randomgenerator.h randomgenerator.cpp
process.o: process.h process.cpp
	g++ -c process.h process.cpp
eventqueue.out: eventqueue.h eventqueue.m.cpp
	g++ -ggdb -o eventqueue.out eventqueue.h eventqueue.m.cpp
randomgenerator.out: randomgenerator.h randomgenerator.cpp randomgenerator.m.cpp
	g++ -ggdb -o randomgenerator.out randomgenerator.h randomgenerator.cpp randomgenerator.m.cpp
process.out: process.h process.cpp process.m.cpp
	g++ -ggdb -o process.out process.h process.cpp process.m.cpp
zip: *.cpp *.h Makefile README.md
	tar -cvf lab2.tar *.cpp *.h Makefile README.md
clean:
	rm -f *.out *.o *.gch lab2.tar sched