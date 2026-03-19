#!/bin/bash

cd ./obj 
gcc -c ../source/*.c 
echo "1)Создались объектные файлы"
rm  test.o  
cd ../lib
ar rcs libcalc.a ../obj/*.o
echo "2)Создалась статическая библиотека"
cd ../bin
gcc -Wall -Werror ../source/main.c -o calc -L ../lib -lcalc 
echo "3)Создался бинарник"
cd ..
rm -rf ./lib/*.a
echo "4)Удалилась статическая библиотека"
