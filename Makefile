
all: balconyAntenna.exe

Debug: all

Release: all

cleanDebug: clean

cleanRelease: clean

#------------------------------------------------

balconyAntenna.exe: main.cpp libwire.cpp Makefile
	g++ -Wall -Wextra main.cpp libwire.cpp -o balconyAntenna.exe

clean:
	del balconyAntenna.exe
