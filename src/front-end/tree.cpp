#include "tree.h"
#include "dot_dump.h"

////////////////////////////////////////////////////////
// Building tree
////////////////////////////////////////////////////////
Node* GetData                  (Parser* parser, size_t* ofs);
Node* GetDefinition            (Parser* parser, size_t* ofs);
Node* GetCompound              (Parser* parser, size_t* ofs);
Node* GetStatement             (Parser* parser, size_t* ofs);
Node* GetArrayDeclaration      (Parser* parser, size_t* ofs);
Node* GetAssignment            (Parser* parser, size_t* ofs);
Node* GetExpression            (Parser* parser, size_t* ofs);
Node* GetLValue                (Parser* parser, size_t* ofs);
Node* GetSimpleExpression      (Parser* parser, size_t* ofs);
Node* GetTerm                  (Parser* parser, size_t* ofs);
Node* GetPrimaryExpression     (Parser* parser, size_t* ofs);
Node* GetLoop                  (Parser* parser, size_t* ofs);
Node* GetCondition             (Parser* parser, size_t* ofs);
Node* GetJump                  (Parser* parser, size_t* ofs);
Node* GetMemoryAccess          (Parser* parser, size_t* ofs);
Node* GetArgs                  (Parser* parser, size_t* ofs);
Node* GetParams                (Parser* parser, size_t* ofs);

void  CheckOpeningBrace        (Parser* parser, size_t* ofs);
void  CheckClosingBrace        (Parser* parser, size_t* ofs);
void  CheckOpeningRoundBracket (Parser* parser, size_t* ofs);
void  CheckClosingRoundBracket (Parser* parser, size_t* ofs);
void  CheckOpeningSquareBracket(Parser* parser, size_t* ofs);
void  CheckClosingSquareBracket(Parser* parser, size_t* ofs);
void  CheckSemicolon           (Parser* parser, size_t* ofs);
void  CheckAssigment           (Parser* parser, size_t* ofs);
bool  IsComparison             (Parser* parser, size_t* ofs);
int   MathTokenToNode          (int op);

Node* ConstructNode (NodeType type, Value value, Node* left, Node* right);
Node* ConstructNode (NodeType type, Node* left, Node* right);
Node* ConstructNode (NodeType type, Value value);
Node* ConstructNode (NodeType type);
void  DestuctSubtree(Node* node);
Tree* NewTree       ();
Node* NewNode       ();

Tree* NewTree()
{
    Tree* tmp = (Tree*)calloc(1, sizeof(Tree));
    assert(tmp);

    return tmp;
}

Node* NewNode()
{
    Node* node = (Node*)calloc(1, sizeof(Node));
    assert(node);

    node->left   = nullptr;
    node->right  = nullptr;
    node->parent = nullptr;

    return node; 
}

Node* ConstructNode(NodeType type, Value value, Node* left, Node* right)
{
    Node* node = (Node*)calloc(1, sizeof(Node));
    assert(node);

    node->type  = type;
    node->value = value;

    node->left  = left;
    node->right = right;

    if (type == MATH_TYPE)
    {
        node->value.op = MathTokenToNode(value.op);
    }

    if (left != nullptr)
    {
        left->parent = node;
    }

    if (right != nullptr)
    {
        right->parent = node;
    }

    return node;
}

Node* ConstructNode(NodeType type, Node* left, Node* right)
{
    return ConstructNode(type, { .op = 0 }, left, right);
}

Node* ConstructNode(NodeType type, Value value)
{
    return ConstructNode(type, value, nullptr, nullptr);
}

Node* ConstructNode(NodeType type)
{
    return ConstructNode(type, { .op = 0 }, nullptr, nullptr);
}

void DestructTree(Tree* tree)
{
    assert(tree);

    DestuctSubtree(tree->root);
}

void DeleteTree(Tree* tree)
{
    assert(tree);

    free(tree);
}

void DestuctSubtree(Node* node)
{
    if (node == nullptr)
    {
        return;
    }

    DestuctSubtree(node->left);
    DestuctSubtree(node->right);
    free(node);
}

#define TYPE       parser->tokens[*ofs].type
#define VALUE      parser->tokens[*ofs].value
#define NEXT_TYPE  parser->tokens[(*ofs) + 1].type
#define NEXT_VALUE parser->tokens[(*ofs) + 1].value
#define ID(left, right) ConstructNode(ID_TYPE, { .name = parser->tokens[(*ofs)++].value.name }, left, right)

Tree* GetTree(Parser* parser)
{
    assert(parser);

    Tree* tree = NewTree();

    size_t ofs = 0;

    tree->root = GetData(parser, &ofs);

    return tree;
}

Node* GetData(Parser* parser, size_t* ofs)
{
    assert(parser);

    bool is_void = (VALUE.op == VOID_FUNC);

    Node* result = ConstructNode(D_TYPE, nullptr, GetDefinition(parser, ofs));
    Node* last = result;
    last->value.is_void = is_void;

    while (*ofs < parser->size)
    {
        is_void = (VALUE.op == VOID_FUNC);
        last->left = ConstructNode(D_TYPE, nullptr, GetDefinition(parser, ofs));

        last->left->parent = last;
        last = last->left;
        last->value.is_void = is_void;
    }

    return result;
}

Node* GetDefinition(Parser* parser, size_t* ofs)
{
    assert(parser);
    assert(ofs);

    if (TYPE != TYPE_OP || (VALUE.op != DECLARATOR && VALUE.op != VOID_FUNC))
    {
        parser->status = TYPE_ERROR;
        CompilationError(parser, ofs);
    }

    (*ofs)++;

    Node* tmp  = ID(nullptr, nullptr);
    tmp->right = GetArgs(parser, ofs);
    tmp->left  = GetCompound(parser, ofs);

    return tmp;
}

Node* GetArgs(Parser* parser, size_t* ofs)
{
    assert(parser);

    CheckOpeningRoundBracket(parser, ofs);

    Node* result = nullptr;

    if (TYPE == TYPE_ID)
    {
        result = ID(nullptr, nullptr);
        Node* last = result;

        while (TYPE == TYPE_OP && VALUE.op == COMMA)
        {
            (*ofs)++;

            last->right = ID(nullptr, nullptr);
            last->right->parent = last;
            last = last->right;
        }
    }

    CheckClosingRoundBracket(parser, ofs);

    return result;
}

Node* GetCompound(Parser* parser, size_t* ofs)
{
    assert(parser);

    CheckOpeningBrace(parser, ofs);

    Node* result = ConstructNode(COMP_TYPE);
    Node* last = result;

    while (TYPE != TYPE_OP || VALUE.op != CLOSING_BRACE)
    {
        last->right = ConstructNode(STAT_TYPE, GetStatement(parser, ofs), nullptr);
        last->right->parent = last;
        last = last->right;
    }
 
    CheckClosingBrace(parser, ofs);

    return result;
}

Node* GetStatement(Parser* parser, size_t* ofs)
{
    assert(parser);

    if (TYPE == TYPE_OP && VALUE.op == DECLARATOR) // Initialization
    {
        (*ofs)++;
        if (NEXT_TYPE == TYPE_OP && NEXT_VALUE.op == OPENING_SQUARE_BRACKET)
        {
            Node* tmp = GetArrayDeclaration(parser, ofs);
            CheckSemicolon(parser, ofs);
            return tmp;
        }

        Node* tmp = GetAssignment(parser, ofs);
        tmp->type = DECL_TYPE;
        CheckSemicolon(parser, ofs);
        return tmp;
    }

    if (TYPE == TYPE_ID && NEXT_TYPE == TYPE_OP &&
       (NEXT_VALUE.op == ASSIGNMENT || NEXT_VALUE.op == OPENING_SQUARE_BRACKET))
    {
        Node* tmp = GetAssignment(parser, ofs);
        CheckSemicolon(parser, ofs);
        return tmp;
    }

    if (TYPE == TYPE_OP && VALUE.op == RET) // return
    {
        Node* tmp = GetJump(parser, ofs);
        CheckSemicolon(parser, ofs);
        return tmp;
    }

    if (TYPE == TYPE_OP && VALUE.op == IF)
    {
        return GetCondition(parser, ofs);
    }

    if (TYPE == TYPE_OP && VALUE.op == LOOP)
    {
        return GetLoop(parser, ofs);
    }

    Node* tmp = GetExpression(parser, ofs);
    CheckSemicolon(parser, ofs);
    return tmp;
}

Node* GetArrayDeclaration(Parser* parser, size_t* ofs)
{
    assert(parser);

    Node* id = ID(nullptr, nullptr);
    
    CheckOpeningSquareBracket(parser, ofs);
    Node* expression = GetExpression(parser, ofs);
    CheckClosingSquareBracket(parser, ofs);
        
    return ConstructNode(ADECL_TYPE, id, expression);
}

Node* GetAssignment(Parser* parser, size_t* ofs)
{
    assert(parser);

    Node* lval = GetLValue(parser, ofs);//ID(nullptr, nullptr);
    CheckAssigment(parser, ofs);

    return ConstructNode(ASSG_TYPE, lval, GetExpression(parser, ofs));
}

Node* GetLValue(Parser* parser, size_t* ofs)
{
    assert(parser);

    Node* id = ID(nullptr, nullptr);

    if (TYPE == TYPE_OP && VALUE.op == OPENING_SQUARE_BRACKET)
    {
        CheckOpeningSquareBracket(parser, ofs);
        Node* expression = GetExpression(parser, ofs);
        CheckClosingSquareBracket(parser, ofs);
        return ConstructNode(MEM_ACCESS_TYPE, id, expression);
    }

    return id;
}

Node* GetExpression(Parser* parser, size_t* ofs)
{
    assert(parser);

    Node* result = GetSimpleExpression(parser, ofs);

    while (IsComparison(parser, ofs))
    {
        Value op = VALUE;        
        (*ofs)++;

        result = ConstructNode(MATH_TYPE, op, result, GetSimpleExpression(parser, ofs));
    }

    return result;
}

Node* GetSimpleExpression(Parser* parser, size_t* ofs)
{
    assert(parser);

    Node* result = GetTerm(parser, ofs);

    while (TYPE == TYPE_OP && (VALUE.op == PLUS || VALUE.op == MINUS))
    {
        Value op = VALUE;
        (*ofs)++;

        result = ConstructNode(MATH_TYPE, op, result, GetTerm(parser, ofs));
    }

    return result;
}

Node* GetTerm(Parser* parser, size_t* ofs)
{
    assert(parser);

    Node* result = GetPrimaryExpression(parser, ofs);

    while (TYPE == TYPE_OP && (VALUE.op == MUL || VALUE.op == DIV))
    {
        Value op = VALUE;
        (*ofs)++;

        result = ConstructNode(MATH_TYPE, op, result, GetPrimaryExpression(parser, ofs));
    }

    return result;
}

Node* GetPrimaryExpression(Parser* parser, size_t* ofs)
{
    assert(parser);

    if (TYPE == TYPE_OP && VALUE.op == OPENING_ROUND_BRACKET)
    {
        CheckOpeningRoundBracket(parser, ofs);
        Node* tmp = GetExpression(parser, ofs);
        CheckClosingRoundBracket(parser, ofs);

        return tmp;
    }

    if (TYPE == TYPE_ID)
    {
        if (NEXT_TYPE == TYPE_OP && NEXT_VALUE.op == OPENING_ROUND_BRACKET) // call
        {
            Node* tmp = ID(nullptr, nullptr);
            return ConstructNode(CALL_TYPE, tmp, GetParams(parser, ofs));             
        }
        else if (NEXT_TYPE == TYPE_OP && NEXT_VALUE.op == OPENING_SQUARE_BRACKET)
        {
            return GetMemoryAccess(parser, ofs);
        }
        else
        {
            return ID(nullptr, nullptr);
        }
    }

    int val = VALUE.number;
    (*ofs)++;

    return ConstructNode(NUMB_TYPE, { .number = val });
}

Node* GetMemoryAccess(Parser* parser, size_t* ofs)
{
    assert(parser);

    Node* id = ID(nullptr, nullptr);
    CheckOpeningSquareBracket(parser, ofs);
    Node* expression = GetExpression(parser, ofs);
    CheckClosingSquareBracket(parser, ofs);

    return ConstructNode(MEM_ACCESS_TYPE, id, expression);
}

Node* GetJump(Parser* parser, size_t* ofs)
{
    assert(parser);

    (*ofs)++;

    Node* tmp = nullptr;
    if (TYPE != TYPE_OP && VALUE.op != SEMICOLON)
    {
        tmp = GetExpression(parser, ofs);
    }

    return ConstructNode(JUMP_TYPE, nullptr, tmp);
}

Node* GetParams(Parser* parser, size_t* ofs)
{
    assert(parser);

    CheckOpeningRoundBracket(parser, ofs);

    Node* result = nullptr;

    if (TYPE != TYPE_OP || VALUE.op != CLOSING_ROUND_BRACKET)
    {
        result = ConstructNode(ARG_TYPE, GetExpression(parser, ofs), nullptr);

        while (TYPE == TYPE_OP && VALUE.op == COMMA)
        {
            (*ofs)++;

            result->parent = ConstructNode(ARG_TYPE, GetExpression(parser, ofs), nullptr);
            result->parent->right = result;
            result = result->parent;
        }
    }

    CheckClosingRoundBracket(parser, ofs);

    return result;
}

Node* GetLoop(Parser* parser, size_t* ofs)
{
    assert(parser);

    (*ofs)++;

    CheckOpeningRoundBracket(parser, ofs);
    Node* result = ConstructNode(LOOP_TYPE, GetExpression(parser, ofs), nullptr);
    CheckClosingRoundBracket(parser, ofs);

    result->right = GetCompound(parser, ofs);
    result->right->parent = result;

    return result;
}

Node* GetCondition(Parser* parser, size_t* ofs)
{
    assert(parser);

    (*ofs)++;
    CheckOpeningRoundBracket(parser, ofs);

    Node* result = ConstructNode(COND_TYPE, GetExpression(parser, ofs), nullptr);

    CheckClosingRoundBracket(parser, ofs);

    result->right = ConstructNode(IFEL_TYPE, GetCompound(parser, ofs), nullptr);

    if (TYPE == TYPE_OP && VALUE.op == ELSE)
    {
        (*ofs)++;
        result->right->right = GetCompound(parser, ofs);
        result->right->right->parent = result->right;
    }

    return result;
}

void CheckOpeningBrace(Parser* parser, size_t* ofs)
{
    assert(parser);

    if (TYPE != TYPE_OP || VALUE.op != OPENING_BRACE)
    {
        parser->status = MISSED_BRACE;
        CompilationError(parser, ofs);
    }

    (*ofs)++;
}

void CheckClosingBrace(Parser* parser, size_t* ofs)
{
    assert(parser);
 
    if (TYPE != TYPE_OP || VALUE.op != CLOSING_BRACE)
    {
        parser->status = MISSED_BRACE;
        CompilationError(parser, ofs);
    }

    (*ofs)++;
}

void CheckOpeningRoundBracket(Parser* parser, size_t* ofs)
{
    assert(parser);

    if (TYPE != TYPE_OP || VALUE.op != OPENING_ROUND_BRACKET)
    {
        parser->status = MISSED_BRACE;
        CompilationError(parser, ofs);
    }

    (*ofs)++;
}

void CheckClosingRoundBracket(Parser* parser, size_t* ofs)
{
    assert(parser);
 
    if (TYPE != TYPE_OP || VALUE.op != CLOSING_ROUND_BRACKET)
    {
        parser->status = MISSED_BRACE;
        CompilationError(parser, ofs);
    }

    (*ofs)++;
}

void CheckOpeningSquareBracket(Parser* parser, size_t* ofs)
{
    assert(parser);
 
    if (TYPE != TYPE_OP || VALUE.op != OPENING_SQUARE_BRACKET)
    {
        parser->status = MISSED_BRACKET;
        CompilationError(parser, ofs);
    }

    (*ofs)++;
}

void CheckClosingSquareBracket(Parser* parser, size_t* ofs)
{
    assert(parser);
 
    if (TYPE != TYPE_OP || VALUE.op != CLOSING_SQUARE_BRACKET)
    {
        parser->status = MISSED_BRACKET;
        CompilationError(parser, ofs);
    }

    (*ofs)++;
}

void CheckSemicolon(Parser* parser, size_t* ofs)
{
    assert(parser);
 
    if (TYPE != TYPE_OP || VALUE.op != SEMICOLON)
    {
        parser->status = MISSED_SMCLN;
        CompilationError(parser, ofs);
    }

    (*ofs)++;
}

void CheckAssigment(Parser* parser, size_t* ofs)
{
    assert(parser);
 
    if (TYPE != TYPE_OP || VALUE.op != ASSIGNMENT)
    {
        parser->status = ASSG_ERROR;
        CompilationError(parser, ofs);
    }

    (*ofs)++;
}

bool IsComparison(Parser* parser, size_t* ofs)
{
    assert(parser);

    return TYPE == TYPE_OP && (VALUE.op == EQUAL || VALUE.op == NOT_EQUAL ||
                               VALUE.op == ABOVE || VALUE.op == EABOVE ||
                               VALUE.op == BELOW || VALUE.op == EBELOW);
}

int MathTokenToNode(int op)
{
    switch (op)
    {
        case PLUS      : { return ADD_OP          ; }
        case MINUS     : { return SUB_OP          ; }
        case MUL       : { return MUL_OP          ; }
        case DIV       : { return DIV_OP          ; }
        case NOT_EQUAL : { return NOT_EQUAL_OP    ; }
        case EQUAL     : { return EQUAL_OP        ; }
        case BELOW     : { return LESS_OP         ; }
        case ABOVE     : { return GREATER_OP      ; }
        case EBELOW    : { return LESS_EQUAL_OP   ; }
        case EABOVE    : { return GREATER_EQUAL_OP; }

        default     : { return op              ; }
    }
}
