targets=main.cpp melobeep.cpp notes.cpp
all:
	g++ -std=c++11 -Wall -g -pedantic $(targets) -o melobeep_cpp
	#g++ -std=c++11 -Wall -Werror -pedantic $(targets) -o melobeep_cpp
