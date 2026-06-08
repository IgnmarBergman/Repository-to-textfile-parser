output: main.o RepoParser.o
	g++ main.o RepoParser.o -o ToText

main.o: main.cpp
	g++ -c main.cpp

RepoParser.o: RP/RepoParser.cpp RP/RepoParser.h
	g++ -c RP/RepoParser.cpp

clean:
	rm *.o ToText