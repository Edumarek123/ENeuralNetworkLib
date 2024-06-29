.PHONY: all


SOURCEFILES = $(shell find -type f -regex ".*\.cpp")
OBJECTFILES = $(patsubst %.cpp, %.o, $(SOURCEFILES))
TARGET = main

CXX = g++
CXXFLAGS = -g -Wall
NETFLAGS = -lenet

all: clean build run

build: $(TARGET)

run: 
	./$(TARGET)

leak:
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -v ./$(TARGET)

debug:
	gdb ./$(TARGET)

$(TARGET): $(OBJECTFILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(NETFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

.PHONY: clean
clean:
	rm -f $(OBJECTFILES) $(TARGET) 



