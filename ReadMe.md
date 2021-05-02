# PREDOVEDOMLYANE!!! BUSURMANSKY YAZIK
# NE TOLKO V KITAE COMPILATORЫ PISHUT
# Programming language of drevniy rus  :scream_cat:

## The idea
- I already implemented the compiler for my [drevniy-rus-language](https://github.com/vssense/Drevniy-rus-language). It outputs byte code for my (very slow) soft [CPU](https://github.com/vssense/Processor).
- In this project I added data arrays and ability to compile to x86-64 native code and to output nasm listings for debug purposes.
- I named it Slavik Historical Language (or shlang).
### Usage
```
$ git clone https://github.com/vssense/Compiler.git
$ make
$ shlang -h
```
>You will see a help message
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
>```
> For example, to open a picture with a tree of your program run:
```
$ shlang my_genius_prog.txt -o genius_prog -tree-dump
$ ./genius_prog
```
### Grammar :+1:

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

### Syntax :+1:

> You have to save your program with Windows 1251 encoding.

> if you are BUSURMANIN you can use english key-words instead of SLAVIK.

> There is no digits - you can use only drevniy rus arithmetics :smirk_cat: :
>
> - 0  - ноль        - nol
> - 1  - целковый    - celkovii
> - 2  - полушка     - polushka
> - 3  - четвертушка - chetvertushka
> - 4  - осьмушка    - osmyshka
> - 5  - пудовичок   - pudovichok
> - 6  - медячок     - medyachok
> - 7  - серебрячок  - serebryachok
> - 8  - золотничок  - zolotnichok
> - 9  - девятичок   - devyatichok
> - 10 - десятичок   - desyatichok
>
> As you could already understand
>
> - loop is "**зомбирование_идет**" or "**zombirovanie_idet**"
> - if is  "**коли**" ("*koli*")
> - else is "**иначе**" ("*inache*")
> - '=' is "**зомбируем**" ("*zombiruem*")
> - start of block is "**начнем**" ("*nachnem*")
> - end of block is "**закончим**" ("*zakonchim*")
>
> - To declare a function use key-word "**возьмем**" ("*vozmem*") or "**ничего**" ("*nichego*")
> - To declare a variable use key-word "**возьмем**" ("*vozmem*")

### Features :+1:

> - Everything is functions - you can't use global variables
> - There is only one variable scope - function
> - Also you have to use block's operators in each while and if/else statement
> - Two variable types - int64_t and int64_t arrays
> - You can use []operator to any of your variable - you can interpret it as a pointer
> - You can use pointer arithmetic - but do not forget that arrays are going backwards (kol predkami zapovedano vnochno debajit)
> - You must have "**main**" function - it will be called first
> - You have to finish each statement with semicolon
>
>#### Some std functions :
>
> 1. govoru(a) - prints a
> 2. a = nepravdoi() - takes a from stdin (you must finish your input with \n)

##### Now some examples
> P.S.: [example](https://github.com/vssense/Compiler/blob/master/examples/sort.txt) using BUSURMANSIY LANGUAGE

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

###### qsort(skoroporyadok) :

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
