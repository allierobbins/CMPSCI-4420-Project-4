CC = g++
CFLAGS = -g -Wall
LIBS = `pkg-config --libs opencv`

all: add_rm_noise zoom sm_filter blur program5 program6

add_rm_noise: program1.cpp
	$(CC) $(CFLAGS) -o add_rm_noise program1.cpp $(LIBS)

zoom: program2.cpp
	$(CC) $(CFLAGS) -o zoom program2.cpp $(LIBS)

sm_filter: program3.cpp
	$(CC) $(CFLAGS) -o sm_filter program3.cpp $(LIBS)

blur: program4.cpp
	$(CC) $(CFLAGS) -o blur program4.cpp $(LIBS)

program5: program5.cpp
	$(CC) $(CFLAGS) -o program5 program5.cpp $(LIBS)

program6: program6.cpp
	$(CC) $(CFLAGS) -o program6 program6.cpp $(LIBS)

.cpp.o:
	g++ -c -g pkg-config --cflags opencv $<

.PHONY: clean

clean:
	rm -f core *.o add_rm_noise zoom sm_filter blur program5 program6 astronaut0*.jpg
