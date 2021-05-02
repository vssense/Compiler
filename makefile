GPP = g++

options = -Wall -Wextra
shldir  = src/shlang
fedir   = src/front-end
medir   = src/middle-end
bedir   = src/back-end
build   = build/intermediates

.PHONY : builddir default

default : builddir shlang build/front-end build/middle-end build/back-end

builddir : 
	mkdir -p build/intermediates

shlang : $(shldir)/main.cpp $(build)/flag_manager.o
	$(GPP) $(shldir)/main.cpp $(build)/flag_manager.o -o shlang $(options)

$(build)/flag_manager.o : $(shldir)/flag_manager.cpp $(shldir)/flag_manager.h
	$(GPP) -c $(shldir)/flag_manager.cpp -o $(build)/flag_manager.o $(options)

build/front-end : $(fedir)/front-end_main.cpp $(build)/iotree.o $(build)/parser.o $(build)/tree.o $(build)/translator.o
	$(GPP) $(fedir)/front-end_main.cpp $(build)/iotree.o $(build)/parser.o $(build)/tree.o $(build)/translator.o -o build/front-end

$(build)/translator.o : $(fedir)/translator.cpp $(fedir)/translator.h $(fedir)/iotree.h
	$(GPP) -c $(fedir)/translator.cpp -o $(build)/translator.o $(options)

$(build)/iotree.o : $(fedir)/iotree.cpp $(fedir)/iotree.h
	$(GPP) -c $(fedir)/iotree.cpp -o $(build)/iotree.o $(options)

$(build)/parser.o : $(fedir)/parser.cpp $(fedir)/parser.h
	$(GPP) -c $(fedir)/parser.cpp -o $(build)/parser.o $(options)

$(build)/tree.o : $(fedir)/tree.cpp $(fedir)/tree.h $(fedir)/dot_dump.h $(fedir)/parser.h
	$(GPP) -c $(fedir)/tree.cpp -o $(build)/tree.o $(options)

build/middle-end : $(medir)/middle-end_main.cpp $(build)/iotree.o $(build)/tree_optimize.o $(build)/parser.o $(build)/tree.o
	$(GPP) $(medir)/middle-end_main.cpp $(build)/iotree.o $(build)/tree_optimize.o $(build)/parser.o $(build)/tree.o -o build/middle-end

$(build)/tree_optimize.o : $(medir)/tree_optimize.cpp $(medir)/tree_optimize.h $(fedir)/iotree.h
	$(GPP) -c $(medir)/tree_optimize.cpp -o $(build)/tree_optimize.o $(options)

build/back-end : $(bedir)/back-end_main.cpp $(build)/compiling.o $(build)/byte_code.o $(build)/elf_writer.o $(build)/parser.o $(build)/tree.o $(build)/iotree.o
	$(GPP) $(bedir)/back-end_main.cpp $(build)/compiling.o $(build)/byte_code.o $(build)/elf_writer.o $(build)/parser.o $(build)/tree.o -o build/back-end $(build)/iotree.o $(options)

$(build)/compiling.o : $(bedir)/compiling.cpp $(bedir)/compiling.h $(fedir)/tree.h $(bedir)/std_func.cpp
	$(GPP) -c $(bedir)/compiling.cpp -o $(build)/compiling.o $(options)

$(build)/byte_code.o : $(bedir)/byte_code.cpp $(bedir)/byte_code.h $(bedir)/compiling.h
	$(GPP) -c $(bedir)/byte_code.cpp -o $(build)/byte_code.o $(options)

$(build)/elf_writer.o : $(bedir)/elf_writer.cpp $(bedir)/elf_writer.h
	$(GPP) -c $(bedir)/elf_writer.cpp -o $(build)/elf_writer.o $(options)
