GPP = g++

options = -Wall -Wextra
src = src
bin = bin

input = input.txt

default : $(bin)/compiler

run : $(bin)/compiler
	$(bin)/compiler $(input)

$(bin)/compiler : $(bin)/parser.o $(bin)/tree.o $(bin)/byte_code.o $(bin)/flag_manager.o $(bin)/compiling.o $(src)/main.cpp
	$(GPP) $(src)/main.cpp $(bin)/parser.o $(bin)/byte_code.o $(bin)/flag_manager.o $(bin)/tree.o $(bin)/compiling.o -o $(bin)/compiler $(options)

$(bin)/byte_code.o : $(src)/compiling/byte_code.cpp $(src)/compiling/byte_code.h
	$(GPP) -c $(src)/compiling/byte_code.cpp -o $(bin)/byte_code.o $(options)

$(bin)/flag_manager.o : $(src)/compiling/flag_manager.cpp $(src)/compiling/flag_manager.h
	$(GPP) -c $(src)/compiling/flag_manager.cpp -o $(bin)/flag_manager.o $(options)

$(bin)/parser.o : $(src)/tree/parser.cpp $(src)/tree/parser.h
	$(GPP) -c $(src)/tree/parser.cpp -o $(bin)/parser.o $(options)

$(bin)/compiling.o : $(src)/compiling/compiling.cpp $(src)/compiling/compiling.h $(src)/compiling/std_func.cpp $(src)/compiling/byte_code.h
	$(GPP) -c $(src)/compiling/compiling.cpp -o $(bin)/compiling.o $(options)

$(bin)/tree.o : $(src)/tree/tree.cpp $(src)/tree/tree.h $(src)/tree/parser.h $(src)/tree/dot_dump.h
	$(GPP) -c $(src)/tree/tree.cpp -o $(bin)/tree.o $(options)
