

all: ./source/main.c ./source/add.c ./source/sub.c ./source/mul.c ./source/div.c
	gcc ./obj/main.o ./obj/add.o ./obj/sub.o ./obj/mul.o ./obj/div.o -o calc && mv calc ./bin

run:
	./bin/calc

build_lib: ./obj/main.o ./obj/add.o ./obj/sub.o ./obj/mul.o ./obj/div.o
	ar rcs ./lib/libcalc.a ./obj/*

build: ./lib/libcalc.a
	gcc ./source

clean: 
	rm -rf ./obj/* ./bin/* ./lib/*

