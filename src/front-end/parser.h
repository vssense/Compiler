#pragma once

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const size_t MAX_SIZE = 1024;
static const size_t MAX_NAME_LEN = 32;

enum OperatorType
{
    DECLARATOR,
    VOID_FUNC,
    LOOP,
    RET,
    IF,
    ELSE,
    ASSIGNMENT,
    OPENING_BRACE,
    CLOSING_BRACE,
    EQUAL,
    PLUS,
    MINUS,
    MUL,
    DIV,
    EBELOW,
    EABOVE,
    BELOW,
    ABOVE,
    NOT_EQUAL,
    OPENING_ROUND_BRACKET,
    CLOSING_ROUND_BRACKET,
    OPENING_SQUARE_BRACKET,
    CLOSING_SQUARE_BRACKET,
    SEMICOLON,
    COMMA,

    NUM_OPERATORS
};

struct Operator
{
    const char* rus_name;
    size_t rus_len;
    const char* eng_name;
    size_t eng_len;
};

static const Operator OPERATORS[] =
{
    { "�������"           , 7 , "vozmem"           , 6 },
    { "������"            , 6 , "nichego"          , 7 },
    { "������������_����" , 17, "zombirovanie_idet", 17},
    { "�������"           , 7 , "polojim"          , 7 },
    { "����"              , 4 , "koli"             , 4 },
    { "�����"             , 5 , "inache"           , 6 },
    { "���������"         , 9 , "zombiruem"        , 9 },
    { "������"            , 6 , "nachnem"          , 7 },
    { "��������"          , 8 , "zakonchim"        , 9 },
    { "=="                , 2 ,  nullptr           , 0 },
    { "+"                 , 1 ,  nullptr           , 0 },
    { "-"                 , 1 ,  nullptr           , 0 },
    { "*"                 , 1 ,  nullptr           , 0 },
    { "/"                 , 1 ,  nullptr           , 0 },
    { "<="                , 2 ,  nullptr           , 0 },
    { ">="                , 2 ,  nullptr           , 0 },
    { "<"                 , 1 ,  nullptr           , 0 },
    { ">"                 , 1 ,  nullptr           , 0 },
    { "!="                , 2 ,  nullptr           , 0 },
    { "("                 , 1 ,  nullptr           , 0 },
    { ")"                 , 1 ,  nullptr           , 0 },
    { "["                 , 1 ,  nullptr           , 0 },
    { "]"                 , 1 ,  nullptr           , 0 },
    { ";"                 , 1 ,  nullptr           , 0 },
    { ","                 , 1 ,  nullptr           , 0 }
};

struct Digit
{
    const char* rus_digit;
    size_t rus_len;
    const char* eng_digit;
    size_t eng_len;
};

static const Digit DIGITS[] =
{
    { "����"       , 4 , "nol"          , 3  }, 
    { "��������"   , 8 , "celkovii"     , 8  }, 
    { "�������"    , 7 , "polushka"     , 8  }, 
    { "�����������", 11, "chetvertushka", 13 }, 
    { "��������"   , 8 , "osmyshka"     , 8  }, 
    { "���������"  , 9 , "pudovichok"   , 10 }, 
    { "�������"    , 7 , "medyachok"    , 9  }, 
    { "����������" , 10, "serebryachok" , 12 }, 
    { "����������" , 10, "zolotnichok"  , 11 }, 
    { "���������"  , 9 , "devyatichok"  , 11 },
    { "���������"  , 9 , "desyatichok"  , 11 } 
};

const int NUM_DIGITS = 11;

enum TokenType
{
    TYPE_NUMB,
    TYPE_ID,
    TYPE_OP
};

union Value
{
    int op;
    int number;
    char* name;
    bool is_void;
};

struct Token
{
    TokenType type;
    Value value;

    char* token_str;
};

enum Error
{
    BUFFER_IS_OK,
    TYPE_ERROR,
    MISSED_BRACE,
    MISSED_SMCLN,
    MISSED_BRACKET,
    GET_NUMBER_ERR,
    BRACKET_ERR,
    SQR_BRACKET_ERR,
    UNKNOWN_SYMBOL,
    ASSG_ERROR,
    RET_ERROR,
    VOID_FUNC_ERROR
};

struct Buffer
{
    char* str;
    char* original_str;

    size_t lenght;
};

struct Parser
{
    Token* tokens;
    size_t size;

    char* original_buffer;
    Error status;
};


Parser* Parse           (const char* input);
void    ParserDump      (Parser* parser);
void    CompilationError(Parser* parser, size_t* idx);
Buffer* GetBuffer       (const char* input);
void    IgnoreSpaces    (Buffer* buffer);
void    DestructParser  (Parser* parser);
void    DeleteParser    (Parser* parser);
