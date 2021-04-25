GPP = g++

options = -std=c++2a -Wall -Wextra
src = src
bin = bin

input = input.txt

compile : run
	$(bin)/assembler.exe asm_tmp.txt
	$(bin)/processor.exe $(bin)/work_file.bin

run : $(bin)/compilier.exe
	$(bin)/compilier.exe $(input)

$(bin)/compilier.exe : $(bin)/parser.o $(bin)/tree.o $(bin)/compiling.o $(src)/main.cpp
	$(GPP) $(src)/main.cpp $(bin)/parser.o $(bin)/tree.o $(bin)/compiling.o -o $(bin)/compilier.exe $(options)

$(bin)/parser.o : $(src)/tree/parser.cpp $(src)/tree/parser.h
	$(GPP) -c $(src)/tree/parser.cpp -o $(bin)/parser.o $(options)

$(bin)/compiling.o : $(src)/compiling/compiling.cpp $(src)/compiling/compiling.h $(src)/compiling/std_func.cpp
	$(GPP) -c $(src)/compiling/compiling.cpp -o $(bin)/compiling.o $(options)

$(bin)/tree.o : $(src)/tree/tree.cpp $(src)/tree/tree.h $(src)/tree/parser.h $(src)/tree/dot_dump.h
	$(GPP) -c $(src)/tree/tree.cpp -o $(bin)/tree.o $(options)
