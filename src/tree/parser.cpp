#include "parser.h"

const char* SKIP_SYMBOLS = " \n\t\r";

Parser* Parse            (const char* input);
size_t  LenOfFile        (FILE* file);
Buffer* ReadFile         (FILE* file);
size_t  FillTokens       (Token* tokens, Buffer* buffer);
Token*  NewTokensBuffer  ();
void    PrintError       (Error error);
void    PrintBeforeSymbol(char* str, char symbol);
void    CompilationError (Parser* parser, size_t* idx);
void    GetNumber        (Token* tokens, size_t* ofs, Buffer* buffer);
void    GetName          (Token* tokens, size_t* ofs, Buffer* buffer);
void    GetTokens        (Token* tokens, size_t* ofs, Buffer* buffer);
void    Delete           (Buffer* buffer);
void    ParserDump       (Parser* parser);


size_t LenOfFile(FILE* file)
{
    assert(file);

    fseek(file, 0, SEEK_END);
    size_t pos = ftell(file);
    fseek(file, 0, SEEK_SET);

    return pos;
}

Buffer* ReadFile(FILE* file)
{
    assert(file);

    Buffer* buffer = (Buffer*)calloc(1, sizeof(Buffer)); 
 
    size_t size = LenOfFile(file);

    buffer->str = (char*)calloc(size + 1, sizeof(char));
    assert(buffer->str);

    buffer->lenght = fread(buffer->str, sizeof(char), size, file);
    buffer->original_str = buffer->str;

    buffer->str[buffer->lenght] = '\0';

    return buffer;
}

Buffer* GetBuffer(const char* input)
{
    assert(input);

    FILE* file = fopen(input, "r");
    if (!file) 
    {
        printf("error : no such file \"%s\"\n", input);
        assert(!"no input file\n");
    }

    return ReadFile(file); 
}

Token* NewTokensBuffer()
{
    Token* tokens = (Token*)calloc(MAX_SIZE, sizeof(Token));
    assert(tokens);

    return tokens;
}

void PrintError(Error error)
{
    switch (error)
    {
        case BUFFER_IS_OK :
        {
            printf("error was called, but not detected\n");
            break;
        }
        case TYPE_ERROR :
        {
            printf("unknown type\n");
            break;
        }
        case UNKNOWN_SYMBOL :
        {
            printf("unknown symbol\n");
            break;
        }
        case MISSED_BRACE :
        {
            printf("missed brace\n");
            break;
        }
        case MISSED_SMCLN :
        {
            printf("missed semicolon\n");
            break;   
        }
        case MISSED_BRACKET:
        {
            printf("missed square bracket\n");
            break;
        }
        case ASSG_ERROR :
        {
            printf("assignment error\n");
            break;   
        }
        default :
        {
            printf("unknown error\n");
            break;
        }
    }
}

void PrintBeforeSymbol(char* str, char symbol)
{
    int i = 0;
    while (str[i] != symbol && str[i] != '\0' && str[i] != EOF)
    {
        putchar(str[i++]);
    }
}

void CompilationError(Parser* parser, size_t* idx)
{
    assert(parser);

    printf("Compilation error : ");

    PrintError(parser->status);

    char* str_start = parser->tokens[*idx].token_str;
    printf("%s\n", str_start);
    
    while (str_start > parser->original_buffer && *str_start != '\n')
    {
        --str_start;
    }

    PrintBeforeSymbol(str_start, '\n');

    printf("\n");

    for (int i = 0; i <= parser->tokens[*idx].token_str - str_start; ++i)
    {
        printf(" ");
    }

    printf("^\n");
}

#define SYMBOL *buffer->str
#define TYPE    tokens[*ofs].type
#define VALUE   tokens[*ofs].value
#define STR     tokens[*ofs].token_str

void IgnoreSpaces(Buffer* buffer)
{
    assert(buffer);

    buffer->str += strspn(buffer->str, SKIP_SYMBOLS);
    if (SYMBOL == '~')
    {
        buffer->str += strcspn(buffer->str, "\n"); 
    }
}

void GetNumber(Token* tokens, size_t* ofs, Buffer* buffer)
{
    int len = 0;

    sscanf(buffer->str, "%d%n", &VALUE.number, &len);
    
    TYPE = TYPE_NUMB;
    STR = buffer->str;
    (*ofs)++;

    buffer->str += len;
}

void GetName(Token* tokens, size_t* ofs, Buffer* buffer)
{
    assert(tokens);
    assert(ofs);
    assert(buffer);

    for (int i = 0; i < NUM_DIGITS; ++i)
    {
        if (strncmp(buffer->str, DIGITS[i].digit, DIGITS[i].len) == 0)
        {
            VALUE.number = i;
            TYPE         = TYPE_NUMB;
            STR          = buffer->str;

            buffer->str += DIGITS[i].len;
            (*ofs)++;
            return;
        }
    }

    for (int i = 0; i < NUM_OPERATORS; ++i)
    {
        if (strncmp(buffer->str, OPERATORS[i].name, OPERATORS[i].len) == 0)
        {
            VALUE.op = i;
            TYPE     = TYPE_OP;
            STR      = buffer->str;

            buffer->str += OPERATORS[i].len;
            (*ofs)++;
            return;
        }
    }

    int len = 0;

    VALUE.name = (char*)calloc(MAX_NAME_LEN, sizeof(char));
    sscanf(buffer->str, "%[A-Za-z_]%n", VALUE.name, &len);

    TYPE = TYPE_ID;
    STR = buffer->str;
    buffer->str += len;

    (*ofs)++;
    // printf("%s\n", buffer->str);
    // getchar();
}

void GetTokens(Token* tokens, size_t* ofs, Buffer* buffer)
{
    assert(tokens);
    assert(buffer);

    while (size_t(buffer->str - buffer->original_str) < buffer->lenght && SYMBOL != EOF)
    {
        IgnoreSpaces(buffer);

        if (isdigit(SYMBOL))
        {
            printf("I HATE DIGITS!!!!\n");
            buffer->str++;
        }
        else
        {
            GetName(tokens, ofs, buffer);
        }

        IgnoreSpaces(buffer);
    }
}

size_t FillTokens(Token* tokens, Buffer* buffer)
{
    assert(tokens);
    assert(buffer);

    size_t ofs = 0;

    GetTokens(tokens, &ofs, buffer);

    return ofs;
}

void Delete(Buffer* buffer)
{
    assert(buffer);

    free(buffer);
}

Parser* Parse(const char* input)
{
    assert(input);

    Buffer* buffer = GetBuffer(input);

    Token* tokens = NewTokensBuffer();

    size_t ofs = FillTokens(tokens, buffer);

    Parser* parser = (Parser*)calloc(1, sizeof(Parser));

    parser->tokens = tokens;
    parser->size = ofs;
    parser->original_buffer = buffer->original_str;

    Delete(buffer);

    return parser;
}

void DestructParser(Parser* parser)
{
    assert(parser);
    assert(parser->original_buffer);
    assert(parser->tokens);

    free(parser->original_buffer);
    free(parser->tokens);
}

void DeleteParser(Parser* parser)
{
    assert(parser);

    free(parser);
}

void ParserDump(Parser* parser)
{
    assert(parser);

    for (size_t i = 0; i < parser->size; ++i)
    {
        printf("[%lu] ", i);
        switch (parser->tokens[i].type)
        {
            case TYPE_NUMB :
            {
                printf("type number,   value = %d\n", parser->tokens[i].value.number);
                break;
            }
            case TYPE_ID :
            {
                printf("type ID,       value = '%s'\n", parser->tokens[i].value.name);
                break;
            }
            case TYPE_OP :
            {
                printf("type operator, value = '%s'[%d]\n", OPERATORS[parser->tokens[i].value.op].name, parser->tokens[i].value.op);
                break;
            }
            default :
            {
                printf("Unknown type, line = %d\n", __LINE__);
            }
        }
    }
}
