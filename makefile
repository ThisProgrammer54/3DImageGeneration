CXX       := /usr/bin/g++ 
CXX_FLAGS := -O3 -std=c++17 -Wall -Wextra -Wpedantic -g 
 
SRC := src
INC := include
TARGET := build/main

SRC_CPP := $(wildcard $(SRC)/*.cpp)
LIB_SRC := $(filter-out $(SRC)/main.cpp, $(SRC_CPP))
TST_SRC := $(wildcard test/src/*.cpp)

OPENCV = `pkg-config --cflags --libs opencv4`


all: clean build_dir $(TARGET) 

build_dir:
	mkdir build
 
$(TARGET): $(SRC_CPP)
	$(CXX) $(CXX_FLAGS)  -Iinclude/eigen -I$(INC) $^ -o $@ $(OPENCV)
 
run: all 
	./$(TARGET) 
 
clean: 
	rm -rf build


test: $(LIB_SRC) $(TST_SRC) 
	$(CXX) $(CXX_FLAGS) -Iinclude/eigen -I$(INC) $^ -o build/maintest $(OPENCV)
	./build/maintest



