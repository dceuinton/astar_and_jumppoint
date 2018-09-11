Main.exe		: 	Main.o  transform.o AstarSearch.o LPAstar.o dstar.o gridworld.o graphics.o finaldstar.o priorityQueue.o
	g++ -o Main.exe Main.o transform.o AstarSearch.o LPAstar.o dstar.o gridworld.o graphics.o finaldstar.o priorityQueue.o -l gdi32 
			
Main.o		:	Main.cpp graphics.h transform.h AstarSearch.h LPAstar.h dstar.h gridworld.h globalvariables.h
	g++ -c -std=c++11 -O2    Main.cpp
	
transform.o		:	 transform.cpp transform.h
	g++ -c -std=c++11 -O2    transform.cpp	
	
AstarSearch.o	:	 AstarSearch.cpp AstarSearch.h
	g++ -c -std=c++11 -O2    AstarSearch.cpp

LPAstar.o	:	 LPAstar.cpp LPAstar.h
	g++ -c -std=c++11 -O2    LPAstar.cpp

dstar.o	:	 dstar.cpp dstar.h
	g++ -c -std=c++11 -O2    dstar.cpp

finaldstar.o	: FinalDStar.cpp FinalDStar.h
	g++ -c -std=c++11 -O2	FinalDStar.cpp

gridworld.o	:	 gridworld.cpp gridworld.h
	g++ -c -std=c++11 -O2    gridworld.cpp

graphics.o		:	 graphics.cpp graphics.h
	g++ -c -std=c++11 -O2    graphics.cpp

priorityQueue.o:	priorityQueue.cpp priorityQueue.h
	g++ -c -std=c++11 -O2	priorityQueue.cpp

	
clean:
	del *.o
	del *.exe
