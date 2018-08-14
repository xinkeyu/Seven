Seven: seven.o game.o
	g++ -Wall -g -o Seven seven.o game.o
	
seven.o: seven.cpp seven.h game.h
	g++ -Wall -g -c seven.cpp
	
game.o: game.cpp game.h
	g++ -Wall -g -c game.cpp
	
clean: 
	rm -f *.o
