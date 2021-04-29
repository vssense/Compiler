# Programming language of drevniy rus  :scream_cat:

## Idea
> I already got compiler for my [drevniy-rus-language](https://github.com/vssense/Drevniy-rus-language) which compiles for very slow [CPU](https://github.com/vssense/Processor)
> In this project I added arrays and ability to compile to nasm and even to byte code
### Usage
>```
> $ git clone https://github.com/vssense/Compiler.git
> $ mkdir bin
> $ make
> $ bin/shlang -h
>```
>You will see a help message
>```
>use flags:
>      -h               display help        
>      -o               name output with the next command line arg (default b.out)
>      -parser-dump     console dump of all tokens and their types
>      -tree-dump       build and open tree of program using dot (must exists 'log' directory)
>      -nametable-dump  console dump info about functions and variables
>      -S               create file with nasm code of your program
>      -Os              speed optimization
>      -Om              memory optimization
>```
> For example, to open a picture with a tree of your program run
>```
> $ mkdir bin
> $ bin/shlang my_genius_prog.txt -o genius_prog -tree-dump
> $ chmod +x genius_prog
> $ ./genius_prog
>```

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
>       Cond ::= 'в_случае' '(' Expr ')' Comp | 'в_случае' '(' Expr ')' Comp 'иначе' Comp
>       Loop ::= 'зомбирование_идет' '(' Expr ')' Comp
>       Var  ::= ['A' - 'Z', 'a' - 'z', '_']+
>       Mem  ::= Var '[' Expr ']'
>       Num  ::= 'ноль' - 'десятичок'
>    ```

### Syntax :+1:

> There is no digits - you can use only drevniy rus arithmetics :smirk_cat: :
>
> - 0 - ноль
> - 1 - целковый
> - 2 - полушка
> - 3 - четвертушка
> - 4 - осьмушка
> - 5 - пудовичок
> - 6 - медячок
> - 7 - серебрячок
> - 8 - золотничок
> - 9 - девятичок
> - 10 - десятичок
>
> As you could already understand
>
> - loop is "**зомбирование_идет**"
> - if is "**в_случае**"
> - else is "**иначе**"
> - '=' is "**зомбируем**"
> - start of block is "**начнем**"
> - end of block is "**закончим**"
>
> To declare a function or variable use key-word "**возьмем**" or "**ничего**"

### Features :+1:

>Everything is functions - you can't use global variables
>
>There is only one variable scope - function
>
>Also you have to use block's operators in each while and if/else statement
>
>Two variable types - int64_t and int64_t arrays
>
>You can use []operator to any of your variable - you can interpret it as a pointer
>
>You can use pointer arithmetic - but do not forget that arrays are going backwards
>
>You must have "**main**" function - it will be called first
>
>You have to finish each statement with semicolon
>
>#### Some std functions :
>
>1. govoru(a) - prints a
>2. a = nepravdoi() - takes a from stdin (you must finish your input with \n)

##### Now some examples

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

###### qsort :

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
        QuickSort(array - (pos) * золотничок, size - pos);
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
