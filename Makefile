CXX: cc

all: build run

build:
	$(CXX) src/lea.c -o ./build/lea
 
run:
	./build/lea --file ./data/book.lea

.PHONY: all