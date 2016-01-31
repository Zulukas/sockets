OBJECTS=server.o driver.o

a.out: $(OBJECTS)
	g++ $(OBJECTS) -o server

server.o: server.cpp server.h
	g++ -c server.cpp

driver.o: driver.cpp server.h
	g++ -c driver.cpp
	