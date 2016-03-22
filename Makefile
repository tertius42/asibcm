CXXFLAGS=-Wall -g
CXX=g++
DEBUG=

.SUFFIXES: .cpp .o

OFILES= asibcm.o
asibcm: $(OFILES)
	$(CXX) $(CXXFLAGS) $(DEBUG) $(OFILES) -o asibcm

clean:
	-rm *.o asibcm

prefix=/usr

install: asibcm
	install -m 0755 asibcm $(prefix)/bin

uninstall: asibcm
	rm $(prefix)/bin/asibcm

asibcm.o: asibcm.cpp 
