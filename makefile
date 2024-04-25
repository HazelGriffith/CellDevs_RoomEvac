CC=g++
CFLAGS=-std=c++17

INCLUDELIBRARIES=-Ilibraries

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

main.o: main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDELIBRARIES) main.cpp -o build/main.o
		
simulator: main.o
	$(CC) -g -o bin/MAIN build/main.o
		
#CLEAN COMMANDS
clean: 
	rm -f bin/* build/*