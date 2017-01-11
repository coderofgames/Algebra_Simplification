#ifndef EXPRESSION_TREE_H
#define EXPRESSION_TREE_H

#include "Utils.h"
#include "StringUtils.h"

enum NODE_TYPE { TERMINAL, OPERATOR };

class Node
{
public:

	// base type
	int type;
};

class TerminalNode : public Node
{
public:
	TerminalNode(){
		type = TERMINAL;
	}

	string data;
};

class MathOperator : public Node
{
public:
	MathOperator()
	{
		type = OPERATOR;
	}

	Node* GetLeft()
	{
		if (left_branch != NULL)
		{
			return left_branch;
		}
		else 
		{
			// log an error
			return NULL;
		}

		// should never reach this
		return NULL;
	}

	Node* GetRight()
	{
		if (right_branch != NULL)
		{
			return right_branch;
		}
		else
		{
			// log an error
			return NULL;
		}

		// should never reach this
		return NULL;
	}
	Node *left_branch;
	Node *right_branch;
};


class Add : public MathOperator
{
public:
	Add() :MathOperator(){
	}
};

class Subtract : public MathOperator
{
public:
	Subtract() :MathOperator(){}
};

class Multiply : public MathOperator
{
public:
	Multiply() :MathOperator() {}
};

class Divide : public MathOperator
{
public:
	Divide() :MathOperator() {}
};

class Modulus : public MathOperator
{
public:
	Modulus() :MathOperator() {}
};

class Power : public MathOperator
{
public:
	Power() :MathOperator() {}
};

class Sqrt : public MathOperator
{
public:
	Sqrt() :MathOperator() {}
};

class Sin : public MathOperator
{
public:
	Sin() :MathOperator() {}
};

class Cos : public MathOperator
{
public:
	Cos() :MathOperator() {}
};

class Tan : public MathOperator
{
public:
	Tan() :MathOperator() {}
};



class ExpressionTree
{
public:



	
	MathOperator *root;

	Node *current;


	Node* GetLeftNode(Node * op)
	{

		if (op!= NULL && op->type == OPERATOR)
		{
			
			Node* n = ((MathOperator*)op)->left_branch;
			if (n)
			{
				return n;
			}
			else
			{
				// log an error
				return NULL;
			}
			
			
		}
		else return NULL;
	}


	ExpressionTree(){}
	~ExpressionTree(){}



};

#endif