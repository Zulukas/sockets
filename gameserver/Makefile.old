OBJECTS=server.o gameserver.o rpshost.o driver.o

a.out: $(OBJECTS)
	g++ $(OBJECTS)

server.o: server.cpp server.h
	g++ -c server.cpp

gameserver.o: gameserver.cpp gameserver.h server.h
	g++ -c gameserver.cpp


rpshost.o: rpshost.cpp rpshost.h game.h
	g++ -c rpshost.cpp