frame.o: frame.h frame.cpp
	g++ -c frame.h frame.cpp
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