# ~~ACHTUNG!~~ BUSURMANSKY YAZIK!!
# NE TOLKO ~~V KITAE~~ COMPILATORЫ PISHUT!!!
# Tre programming language of [Drevniy Rus](https://www.youtube.com/watch?v=BwPT8_bB8dY)  :scream_cat:

## The idea
- Recently I've implemented the compiler for my [drevniy-rus-language](https://github.com/vssense/Drevniy-rus-language). It outputs byte code for my (very slow) [Soft-CPU](https://github.com/vssense/Processor).
- In this project I added data arrays and ability to compile to x86-64 native code and to output nasm listings for debug purposes.
- I named it Slavi**k** Historical Language (or **shlang**).
### Usage
```
$ git clone https://github.com/vssense/Compiler.git
$ make
$ ./shlang -h
```
>You will see BLM output (Beautiful Language Message) explaining run syntax
>```
>use flags:
>      -h                    display help        
>      -o                    output file name (default b.out)
>      -S                    create file with nasm code of your program
>      -Os                   optimization for speed
>      -Om                   optimization for memory usage
>      -femit-parser-dump    emit console dump of all tokens and their types
>      -femit-tree-dump      emit tree diagram in 'log' subdir and open it using dot tool
>      -femit-nametable-dump emit console dump info about functions and variables
>      -femit-tree           emit tree in written form for translation
>      -translate            emit drevniy-rus-language code received from tree
>```
> For example, to open a picture with a tree of your program run:
```
$ ./shlang my_genius_prog.shl -o genius_prog -tree-dump
```
> To save tree of your program run
```
$ ./shlang my_genius_prog.shl -femit-tree -o saved_tree.txt
```
### Translation
> To get drevniy-rus-language code from tree run:
``` 
$ ./shlang saved_tree.txt -translate -o translated_prog.txt
```
> This feature doesn't fully work with newest vesion of Drevniy Rus Language. Instead of unrecognised constructions you will see a message "can't interpret this construction".

> There is [tree](https://github.com/vssense/Compiler/blob/master/factorial_potter_language_tree.txt) and [program](https://github.com/vssense/Compiler/blob/master/factorial_potter_language.txt) written in [potter language](https://github.com/tralf-strues/potter-tongue-x86). Thanks for sample, [tralf-strues](https://github.com/tralf-strues)!

> Lets run translator:
```
$ ./shlang -translate factorial_potter_language_tree.txt
```
> We got program on Drevniy Rus Language (think about translating from THE BUSURMANCKIY SYNTAX of Drevniy Rus Language)
### The grammar :+1:

>``` ruby
>Abbreviation:
>       Gram | grammar
>       Prog | program
>       Defi | definition
>       FDec | function declaration
>       Comp | compound
>       Stat | statement
>       Expr | expression
>       Simp | simple expression
>       Term | term
>       Prim | primary grammar(factor)
>       Call | function call
>       Init | initialization
>       Assg | assignment
>       Jump | jump
>       Cond | conditional statement
>       Loop | loop
>       Var  | variable (char*) or function ID
>       Mem  | memory access
>       Num  | number(constant)
>    
>    
>    Rules:
>       Gram ::= Prog 'EOF'
>       Prog ::= {Defi}+
>       Defi ::= FDec Comp
>       FDec ::= ('возьмем' | 'ничего') Var '(' ')' | Var '(' Var  { ',' Var }* ')'
>       Comp ::= 'начнем' Stat* 'закончим'
>       Stat ::= [Expr, Init, Assg, Jump] ';' | Cond | Loop
>       Expr ::= Simp {['<', '>', '==', '!=', '<=', '>='] Simp}*
>       Simp ::= Term {['+', '-'] Term}*
>       Term ::= Prim {['*', '/'] Prim}*
>       Prim ::= '(' Expr ')' | Num | Var | Call | Mem
>       Call ::= Var '(' ')' | Var '(' Expr { ',' Expr}* ')'
>       Init ::= 'возьмем' Var 'зомбируем' Expr
>       Assg ::= (Var | Mem) 'зомбируем' Expr
>       Jump ::= 'положим' Expr
>       Cond ::= 'коли' '(' Expr ')' Comp | 'коли' '(' Expr ')' Comp 'иначе' Comp
>       Loop ::= 'зомбирование_идет' '(' Expr ')' Comp
>       Var  ::= ['A' - 'Z', 'a' - 'z', '_']+
>       Mem  ::= Var '[' Expr ']'
>       Num  ::= 'ноль' - 'десятичок'
>    ```

### The syntax :+1:

> Use Windows 1251 encoding to save your program.

> if you are BUSURMANIN you can use english key-words instead of SLAVIK.

> There is no digits - you can use only drevniy rus arithmetics :smirk_cat: :
>
> - 0  - [ноль](https://www.youtube.com/watch?v=BwPT8_bB8dY)        - [nol](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 1  - [целковый](https://www.youtube.com/watch?v=BwPT8_bB8dY)    - [celkovii](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 2  - [полушка](https://www.youtube.com/watch?v=BwPT8_bB8dY)     - [polushka](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 3  - [четвертушка](https://www.youtube.com/watch?v=BwPT8_bB8dY) - [chetvertushka](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 4  - [осьмушка](https://www.youtube.com/watch?v=BwPT8_bB8dY)    - [osmyshka](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 5  - [пудовичок](https://www.youtube.com/watch?v=BwPT8_bB8dY)   - [pudovichok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 6  - [медячок](https://www.youtube.com/watch?v=BwPT8_bB8dY)     - [medyachok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 7  - [серебрячок](https://www.youtube.com/watch?v=BwPT8_bB8dY)  - [serebryachok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 8  - [золотничок](https://www.youtube.com/watch?v=BwPT8_bB8dY)  - [zolotnichok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 9  - [девятичок](https://www.youtube.com/watch?v=BwPT8_bB8dY)   - [devyatichok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 10 - [десятичок](https://www.youtube.com/watch?v=BwPT8_bB8dY)   - [desyatichok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
>
> As you could already understand
>
> - loop is ["**зомбирование_идет**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) or "**zombirovanie_idet**"
> - if is  ["**коли**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*koli*")
> - else is ["**иначе**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*inache*")
> - '=' is ["**зомбируем**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*zombiruem*")
> - start of block is ["**начнем**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*nachnem*")
> - end of block is ["**закончим**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*zakonchim*")
>
> - To declare a function use key-word ["**возьмем**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*vozmem*") or ["**ничего**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*nichego*")
> - To declare a variable use key-word ["**возьмем**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*vozmem*")

### Language features :+1:

> - Everything is functions - you can't use global variables
> - There is only one variable scope - function
> - Also you have to use block operators in each ```while``` and ```if/else``` statement
> - Two variable types - ```int64_t``` and ```int64_t arrays```
> - You can use ```[]```operator to any of your variable - you can interpret it as a pointer
> - You can use pointer arithmetic - but do not forget that arrays are going **backwards** (kol predkami zapovedano vnochno debajit)
> - You must have ```main``` function - it will be called first
> - You have to finish each statement with ```;```
>
>#### Some std functions :
>
> 1. ```govoru(a)``` - prints ```a```
> 2. ```a = nepravdoi()``` - takes ```a``` from stdin (you must finish your input with ```\n```)

##### Now some examples
> P.S.: [example](https://github.com/vssense/Compiler/blob/master/examples/sort.shl) using THE BUSURMANSIY LANGUAGE

###### factorial :
```
возьмем main()
начнем
    govoru(Fact(nepravdoi()));
    положим ноль;
закончим

возьмем Fact(a)
начнем
    в_случае (a < полушка) 
    начнем
        положим целковый;
    закончим
    иначе
    начнем
    положим a * Fact(a - целковый);
    закончим
закончим
```

###### qsort (skoroporyadok) :

```возьмем main()
начнем
    возьмем size зомбируем nepravdoi();
    возьмем array[size];
    возьмем idx зомбируем ноль;

    зомбирование_идет (idx < size)
    начнем
        array[idx] зомбируем nepravdoi();
        idx зомбируем idx + целковый;
    закончим

    QuickSort(array, size);

    idx зомбируем ноль;

    зомбирование_идет (idx < size)
    начнем
        govoru(array[idx]);
        idx зомбируем idx + целковый;
    закончим

    положим ноль;
закончим

ничего QuickSort(array, size)
начнем
    в_случае (size < полушка)
    начнем
        положим;
    закончим

    возьмем pos зомбируем Partition(array, size, array[ноль]);

    в_случае (pos == ноль)
    начнем
        QuickSort(array - золотничок, size - целковый);
    закончим
    иначе
    начнем
        QuickSort(array, pos);
        QuickSort(array - pos * золотничок, size - pos);
    закончим
закончим

возьмем Partition(array, size, pivot)
начнем
    возьмем i   зомбируем ноль;
    возьмем idx зомбируем ноль;
    возьмем tmp зомбируем ноль;

    зомбирование_идет (i < size)
    начнем
        в_случае (pivot > array[i])
        начнем
            tmp зомбируем array[idx];
            array[idx] зомбируем array[i];
            array[i] зомбируем tmp;
            idx зомбируем idx + целковый;
        закончим

        i зомбируем i + целковый;
    закончим

    положим idx;
закончим
```
