#include "tree_optimize.h"

void CalculateOP    (Node* node);
void CalculateConsts(Tree* tree, Node* node);

void Optimize(Tree* tree)
{
	assert(tree);

	CalculateConsts(tree, tree->root);
}

void CalculateConsts(Tree* tree, Node* node)
{
    assert(tree);

    if (node == nullptr)
    {
    	return;
    }

    CalculateConsts(tree, node->right);
    CalculateConsts(tree, node->left);

    if (node->type == MATH_TYPE)
    {
        if (node->right->type == NUMB_TYPE && 
        	node->left ->type == NUMB_TYPE)
        {
            CalculateOP(node);
            node->type = NUMB_TYPE;

            free(node->right);
            node->right = nullptr;
            free(node->left);
            node->left = nullptr;
        }
    }
}

void CalculateOP(Node* node)
{
	assert(node);
	assert(node->type == MATH_TYPE);
	assert(node->left->type == NUMB_TYPE && node->right->type == NUMB_TYPE);

	switch (node->value.op)
	{
		case ADD_OP: node->value.number = node->left->value.number + node->right->value.number; break;
		case SUB_OP: node->value.number = node->left->value.number - node->right->value.number; break;
		case MUL_OP: node->value.number = node->left->value.number * node->right->value.number; break;
		case DIV_OP: node->value.number = node->left->value.number / node->right->value.number; break;

        case EQUAL_OP        : node->value.number = node->left->value.number == node->right->value.number; break;
        case NOT_EQUAL_OP    : node->value.number = node->left->value.number != node->right->value.number; break;
        case LESS_OP         : node->value.number = node->left->value.number <  node->right->value.number; break;
        case GREATER_OP      : node->value.number = node->left->value.number >  node->right->value.number; break;
        case LESS_EQUAL_OP   : node->value.number = node->left->value.number <= node->right->value.number; break;
        case GREATER_EQUAL_OP: node->value.number = node->left->value.number >= node->right->value.number; break;
	}
}