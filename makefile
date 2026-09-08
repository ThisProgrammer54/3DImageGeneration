make: 
	g++ -I include/eigen/ src/main.cpp -o build/main 
run: 
	./build/main
