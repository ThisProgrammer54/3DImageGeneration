CXX       := /usr/bin/g++ 
CXX_FLAGS := -std=c++17 -Wall -Wextra -Wpedantic -g 
 
SRC := src
INC := include
TARGET := build/main
 
all: clean build_dir $(TARGET) 

build_dir:
	mkdir build
 
$(TARGET): $(SRC)/*.cpp 
	$(CXX) $(CXX_FLAGS) -Iinclude/eigen -I$(INC) $^ -o $@
 
run: all 
	./$(TARGET)
 
clean: 
	rm -rf build
