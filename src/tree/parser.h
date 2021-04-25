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
    EQUAL,
    ASSIGNMENT,
    PLUS,
    MINUS,
    MUL,
    DIV,
    EBELOW,
    EABOVE,
    BELOW,
    ABOVE,
    NOT_EQUAL,
    OPENING_BRACE,
    CLOSING_BRACE,
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
    const char* name;
    size_t len;
};

static const Operator OPERATORS[] =
{
    { "возьмем"           , 7  },
    { "ничего"            , 6  },
    { "зомбирование_идет" , 17 },
    { "положим"           , 7  },
    { "в_случае"          , 8  },
    { "иначе"             , 5  },
    { "=="                , 2  },
    { "зомбируем"         , 9  },
    { "+"                 , 1  },
    { "-"                 , 1  },
    { "*"                 , 1  },
    { "/"                 , 1  },
    { "<="                , 2  },
    { ">="                , 2  },
    { "<"                 , 1  },
    { ">"                 , 1  },
    { "!="                , 2  },
    { "начнем"            , 6  },
    { "закончим"          , 8  },
    { "("                 , 1  },
    { ")"                 , 1  },
    { "["                 , 1  },
    { "]"                 , 1  },
    { ";"                 , 1  },
    { ","                 , 1  }
};

struct Digit
{
    const char* digit;
    size_t len;
};

static const Digit DIGITS[] =
{
    { "ноль"       , 4  }, 
    { "целковый"   , 8  }, 
    { "полушка"    , 7  }, 
    { "четвертушка", 11 }, 
    { "осьмушка"   , 8  }, 
    { "пудовичок"  , 9  }, 
    { "мед€чок"    , 7  }, 
    { "серебр€чок" , 10 }, 
    { "золотничок" , 10 }, 
    { "дев€тичок"  , 9  },
    { "дес€тичок"  , 9  } 
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
