
all: parvekeAntenni.exe

Debug: all

Release: all

cleanDebug: clean

cleanRelease: clean

#------------------------------------------------

parvekeAntenni.exe: main.cpp libwire.cpp Makefile
	g++ main.cpp libwire.cpp -o parvekeAntenni.exe

clean:
	del parvekeAntenni.exe
