# ~~ACHTUNG!~~ BUSURMANSKY YAZIK!!
# NE TOLKO ~~V KITAE~~ COMPILATORŠ« PISHUT!!!
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
$ ./shlang my_genius_prog.shl -o genius_prog -femit-tree-dump
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

> There is [tree](https://github.com/vssense/Compiler/blob/master/factorial_potter_language_tree.txt) and [program](https://github.com/vssense/Compiler/blob/master/factorial_potter_language.txt) written in [potter tongue](https://github.com/tralf-strues/potter-tongue-x86). Thanks for sample, [tralf-strues](https://github.com/tralf-strues)!

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
>       FDec ::= ('Š²Š¾Š·ŃŠ¼ŠµŠ¼' | 'Š½ŠøŃŠµŠ³Š¾') Var '(' ')' | Var '(' Var  { ',' Var }* ')'
>       Comp ::= 'Š½Š°ŃŠ½ŠµŠ¼' Stat* 'Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼'
>       Stat ::= [Expr, Init, Assg, Jump] ';' | Cond | Loop
>       Expr ::= Simp {['<', '>', '==', '!=', '<=', '>='] Simp}*
>       Simp ::= Term {['+', '-'] Term}*
>       Term ::= Prim {['*', '/'] Prim}*
>       Prim ::= '(' Expr ')' | Num | Var | Call | Mem
>       Call ::= Var '(' ')' | Var '(' Expr { ',' Expr}* ')'
>       Init ::= 'Š²Š¾Š·ŃŠ¼ŠµŠ¼' Var 'Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼' Expr
>       Assg ::= (Var | Mem) 'Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼' Expr
>       Jump ::= 'ŠæŠ¾Š»Š¾Š¶ŠøŠ¼' Expr
>       Cond ::= 'ŠŗŠ¾Š»Šø' '(' Expr ')' Comp | 'ŠŗŠ¾Š»Šø' '(' Expr ')' Comp 'ŠøŠ½Š°ŃŠµ' Comp
>       Loop ::= 'Š·Š¾Š¼Š±ŠøŃŠ¾Š²Š°Š½ŠøŠµ_ŠøŠ“ŠµŃ' '(' Expr ')' Comp
>       Var  ::= ['A' - 'Z', 'a' - 'z', '_']+
>       Mem  ::= Var '[' Expr ']'
>       Num  ::= 'Š½Š¾Š»Ń' - 'Š“ŠµŃŃŃŠøŃŠ¾Šŗ'
>    ```

### The syntax :+1:

> Use Windows 1251 encoding to save your program.

> if you are BUSURMANIN you can use english key-words instead of SLAVIK.

> There is no digits - you can use only drevniy rus arithmetics :smirk_cat: :
>
> - 0  - [Š½Š¾Š»Ń](https://www.youtube.com/watch?v=BwPT8_bB8dY)        - [nol](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 1  - [ŃŠµŠ»ŠŗŠ¾Š²ŃŠ¹](https://www.youtube.com/watch?v=BwPT8_bB8dY)    - [celkovii](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 2  - [ŠæŠ¾Š»ŃŃŠŗŠ°](https://www.youtube.com/watch?v=BwPT8_bB8dY)     - [polushka](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 3  - [ŃŠµŃŠ²ŠµŃŃŃŃŠŗŠ°](https://www.youtube.com/watch?v=BwPT8_bB8dY) - [chetvertushka](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 4  - [Š¾ŃŃŠ¼ŃŃŠŗŠ°](https://www.youtube.com/watch?v=BwPT8_bB8dY)    - [osmyshka](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 5  - [ŠæŃŠ“Š¾Š²ŠøŃŠ¾Šŗ](https://www.youtube.com/watch?v=BwPT8_bB8dY)   - [pudovichok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 6  - [Š¼ŠµŠ“ŃŃŠ¾Šŗ](https://www.youtube.com/watch?v=BwPT8_bB8dY)     - [medyachok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 7  - [ŃŠµŃŠµŠ±ŃŃŃŠ¾Šŗ](https://www.youtube.com/watch?v=BwPT8_bB8dY)  - [serebryachok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 8  - [Š·Š¾Š»Š¾ŃŠ½ŠøŃŠ¾Šŗ](https://www.youtube.com/watch?v=BwPT8_bB8dY)  - [zolotnichok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 9  - [Š“ŠµŠ²ŃŃŠøŃŠ¾Šŗ](https://www.youtube.com/watch?v=BwPT8_bB8dY)   - [devyatichok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
> - 10 - [Š“ŠµŃŃŃŠøŃŠ¾Šŗ](https://www.youtube.com/watch?v=BwPT8_bB8dY)   - [desyatichok](https://traditio.wiki/%D0%AE%D1%80%D0%B8%D0%B9_%D0%A1%D1%82%D0%B5%D0%BF%D0%B0%D0%BD%D0%BE%D0%B2%D0%B8%D1%87_%D0%A0%D1%8B%D0%B1%D0%BD%D0%B8%D0%BA%D0%BE%D0%B2)
>
> As you could already understand
>
> - loop is ["**Š·Š¾Š¼Š±ŠøŃŠ¾Š²Š°Š½ŠøŠµ_ŠøŠ“ŠµŃ**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) or "**zombirovanie_idet**"
> - if is  ["**ŠŗŠ¾Š»Šø**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*koli*")
> - else is ["**ŠøŠ½Š°ŃŠµ**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*inache*")
> - '=' is ["**Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*zombiruem*")
> - start of block is ["**Š½Š°ŃŠ½ŠµŠ¼**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*nachnem*")
> - end of block is ["**Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*zakonchim*")
>
> - To declare a function use key-word ["**Š²Š¾Š·ŃŠ¼ŠµŠ¼**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*vozmem*") or ["**Š½ŠøŃŠµŠ³Š¾**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*nichego*")
> - To declare a variable use key-word ["**Š²Š¾Š·ŃŠ¼ŠµŠ¼**"](https://www.youtube.com/watch?v=BwPT8_bB8dY) ("*vozmem*")

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
Š²Š¾Š·ŃŠ¼ŠµŠ¼ main()
Š½Š°ŃŠ½ŠµŠ¼
    govoru(Fact(nepravdoi()));
    ŠæŠ¾Š»Š¾Š¶ŠøŠ¼ Š½Š¾Š»Ń;
Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼

Š²Š¾Š·ŃŠ¼ŠµŠ¼ Fact(a)
Š½Š°ŃŠ½ŠµŠ¼
    Š²_ŃŠ»ŃŃŠ°Šµ (a < ŠæŠ¾Š»ŃŃŠŗŠ°) 
    Š½Š°ŃŠ½ŠµŠ¼
        ŠæŠ¾Š»Š¾Š¶ŠøŠ¼ ŃŠµŠ»ŠŗŠ¾Š²ŃŠ¹;
    Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼
    ŠøŠ½Š°ŃŠµ
    Š½Š°ŃŠ½ŠµŠ¼
    ŠæŠ¾Š»Š¾Š¶ŠøŠ¼ a * Fact(a - ŃŠµŠ»ŠŗŠ¾Š²ŃŠ¹);
    Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼
Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼
```

###### qsort (skoroporyadok) :

```Š²Š¾Š·ŃŠ¼ŠµŠ¼ main()
Š½Š°ŃŠ½ŠµŠ¼
    Š²Š¾Š·ŃŠ¼ŠµŠ¼ size Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ nepravdoi();
    Š²Š¾Š·ŃŠ¼ŠµŠ¼ array[size];
    Š²Š¾Š·ŃŠ¼ŠµŠ¼ idx Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ Š½Š¾Š»Ń;

    Š·Š¾Š¼Š±ŠøŃŠ¾Š²Š°Š½ŠøŠµ_ŠøŠ“ŠµŃ (idx < size)
    Š½Š°ŃŠ½ŠµŠ¼
        array[idx] Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ nepravdoi();
        idx Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ idx + ŃŠµŠ»ŠŗŠ¾Š²ŃŠ¹;
    Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼

    QuickSort(array, size);

    idx Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ Š½Š¾Š»Ń;

    Š·Š¾Š¼Š±ŠøŃŠ¾Š²Š°Š½ŠøŠµ_ŠøŠ“ŠµŃ (idx < size)
    Š½Š°ŃŠ½ŠµŠ¼
        govoru(array[idx]);
        idx Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ idx + ŃŠµŠ»ŠŗŠ¾Š²ŃŠ¹;
    Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼

    ŠæŠ¾Š»Š¾Š¶ŠøŠ¼ Š½Š¾Š»Ń;
Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼

Š½ŠøŃŠµŠ³Š¾ QuickSort(array, size)
Š½Š°ŃŠ½ŠµŠ¼
    Š²_ŃŠ»ŃŃŠ°Šµ (size < ŠæŠ¾Š»ŃŃŠŗŠ°)
    Š½Š°ŃŠ½ŠµŠ¼
        ŠæŠ¾Š»Š¾Š¶ŠøŠ¼;
    Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼

    Š²Š¾Š·ŃŠ¼ŠµŠ¼ pos Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ Partition(array, size, array[Š½Š¾Š»Ń]);

    Š²_ŃŠ»ŃŃŠ°Šµ (pos == Š½Š¾Š»Ń)
    Š½Š°ŃŠ½ŠµŠ¼
        QuickSort(array - Š·Š¾Š»Š¾ŃŠ½ŠøŃŠ¾Šŗ, size - ŃŠµŠ»ŠŗŠ¾Š²ŃŠ¹);
    Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼
    ŠøŠ½Š°ŃŠµ
    Š½Š°ŃŠ½ŠµŠ¼
        QuickSort(array, pos);
        QuickSort(array - pos * Š·Š¾Š»Š¾ŃŠ½ŠøŃŠ¾Šŗ, size - pos);
    Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼
Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼

Š²Š¾Š·ŃŠ¼ŠµŠ¼ Partition(array, size, pivot)
Š½Š°ŃŠ½ŠµŠ¼
    Š²Š¾Š·ŃŠ¼ŠµŠ¼ i   Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ Š½Š¾Š»Ń;
    Š²Š¾Š·ŃŠ¼ŠµŠ¼ idx Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ Š½Š¾Š»Ń;
    Š²Š¾Š·ŃŠ¼ŠµŠ¼ tmp Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ Š½Š¾Š»Ń;

    Š·Š¾Š¼Š±ŠøŃŠ¾Š²Š°Š½ŠøŠµ_ŠøŠ“ŠµŃ (i < size)
    Š½Š°ŃŠ½ŠµŠ¼
        Š²_ŃŠ»ŃŃŠ°Šµ (pivot > array[i])
        Š½Š°ŃŠ½ŠµŠ¼
            tmp Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ array[idx];
            array[idx] Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ array[i];
            array[i] Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ tmp;
            idx Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ idx + ŃŠµŠ»ŠŗŠ¾Š²ŃŠ¹;
        Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼

        i Š·Š¾Š¼Š±ŠøŃŃŠµŠ¼ i + ŃŠµŠ»ŠŗŠ¾Š²ŃŠ¹;
    Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼

    ŠæŠ¾Š»Š¾Š¶ŠøŠ¼ idx;
Š·Š°ŠŗŠ¾Š½ŃŠøŠ¼
```
