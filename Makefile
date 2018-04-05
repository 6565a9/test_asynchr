COMPILER=g++
FILES=Server.cpp main.cpp socket.cpp util.cpp
NAMEPROG=test

compil: Server.o main.o socket.o util.o
	$(COMPILER) *.o -o $(NAMEPROG)

Server.o: Server.cpp
	$(COMPILER) Server.cpp -c
main.o: main.cpp
	$(COMPILER) main.cpp -c
socket.o: socket.cpp
	$(COMPILER) socket.cpp -c
util.o: util.cpp
	$(COMPILER) util.cpp -c

clean:
	rm *.o
	rm $(NAMEPROG)
