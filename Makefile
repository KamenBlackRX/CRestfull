#################################################
#
# Make file for Sarah
#
################################################# 

CXX=g++
CXXFLAGS=-std=c++11
LFLAGS=-L/usr/lib/x86_64-linux-gnu -lboost_system -lcrypto -lssl -lcpprest
MAIN=cppExemple
SOURCE=$(wildcard src/*.cpp)
INCLUDES =$(wildcard include/*.h)

all:  $(MAIN)
	@echo Program is sucessfull compiled;


$(MAIN): $(SOURCE)
	
	$(CXX) $^ $(CXXFLAGS) $(LFLAGS) -o $(MAIN) 

static: $(SOURCE)

	$(CXX) $(CXXFLAGS) $(LFLAGS) -o $(MAIN) -static $^
	
	
install:

	if [ "$(shell whoami)" = "root" ]; then \
		install -c -m 755  $(MAIN) /usr/bin; \
	else \
		sudo /usr/bin/install -c -m 755  $(MAIN) /usr/bin; \
	fi;

clean:
	rm -f $(MAIN) *.o

#g++ exemple.cpp -std=c++11  -L/usr/lib/x86_64-linux-gnu -lboost_system -lcrypto -lssl -lcpprest -o main
