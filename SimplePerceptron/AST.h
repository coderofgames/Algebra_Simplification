#ifndef AST_H
#define AST_H

#include "Utils.h"
#include <typeinfo>

using std::vector;

class ASTNode
{
public:

	~ASTNode()
	{
		if (left && left->type == 0)
			delete left;
		if (right && right->type == 0)
			delete right;

		// cannot delete terminal nodes
		left = NULL;
		right = NULL;
	}

	virtual double value() = 0;


	ASTNode* TransformNegatives();

	bool IsThisTheLeftChild()
	{
		if (parent != 0)
		{
			if (parent->left == this)
				return true;
			else
				return false;
		}
		return false;
	}


	int type = 0;
	
	ASTNode *left = 0;
	ASTNode *right = 0;
	ASTNode* parent = 0;
};

class TerminalNode : public ASTNode
{
public:
	TerminalNode(){ sym = ""; num = 0.0;  type = 1; }
	TerminalNode(string s, bool bNum)
	{
		sym = s;
		//num = (isNumberC(s) ? string_to_double(s) : 0.0);
		numeric = bNum;
		if (bNum)
		{
			num = string_to_float(s);
		}
		else num = 0.0;
		type = 1;
		left = 0;
		right = 0;
		parent = 0;
	}

	double value(){
		return num;
	}

	double num;
	string sym;
	bool numeric = false;
};
enum FLOW_CONTROL {
	IFTHEN = 0,
	ALSO
};
string flow_control_array[2] = {
	"IFTHEN",
	"ALSO"
};

enum OPERATOR{ ADD = 0, 
			   SUB, 
			   MUL, 
			   DIV, 
			   POW, 
			   SQRT, 
			   SIN, 
			   COS, 
			   TAN, 
			   AND, 
			   OR,  
			   EQ1, 
			   EQ, 
			   GT, 
			   GE, 
			   LT, 
			   LE
};

string operator_array[18] = { 
	"ADD", 
	"SUB", 
	"MUL", 
	"DIV", 
	"POW", 
	"SQRT", 
	"SIN" , 
	"COS", 
	"TAN", 
	"AND", 
	"OR",  
	"EQ1", 
	"EQ", 
	"GT",
	"GE", 
	"LT",
	"LE"
};

class SyntaxNode : public ASTNode
{
public:
	SyntaxNode(){}
};

class FlowControlNode : public SyntaxNode
{
public:
	FlowControlNode(){
		type = 2;
	}

	FLOW_CONTROL the_flow_control;
	int num_operands = 2;
};

class IFTHENNode : public FlowControlNode
{
public:
	IFTHENNode()
	{
		the_flow_control = FLOW_CONTROL::IFTHEN;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			if (left->value())
				return right->value();
		}

		return 0.0;
	}
};

class ALSONode : public FlowControlNode
{
public:
	ALSONode()
	{
		the_flow_control = FLOW_CONTROL::ALSO;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		/*if (left && right)
		{
		return left->value() && right->value();
		}*/
		if (left) left->value();
		if (right) right->value();

		return 0.0;
	}
};


class OperatorNode : public SyntaxNode
{
public:
	OperatorNode(){
		type = 0;
		left = right = 0;
		parent = 0;
	}

	virtual double value() = 0;

	OPERATOR the_operator;
	int num_operands = 2;
};



class CommutativeOperator : public OperatorNode
{
public:
	CommutativeOperator(){}


	void AddChild(ASTNode* child)
	{
		m_childNodes.push_back(child);
	}

	vector<ASTNode*> m_childNodes;
};

class NonCommutativeOperator : public OperatorNode
{
public:
	NonCommutativeOperator(){}
};

class ADDNode : public CommutativeOperator
{
public:
	ADDNode()
	{
		the_operator = OPERATOR::ADD;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){

		if (m_childNodes.size() != 0)
		{
			double retVal = 0.0f;
			for (ASTNode *n : m_childNodes)
			{
				if (n)
				{
					retVal += n->value();
				}
			}
			return retVal;
		}
		else
		{
			if (left && right)
			{
				return left->value() + right->value();
			}
		}

		return 0.0;
	}
};

class SUBNode : public NonCommutativeOperator
{
public:
	SUBNode(){
		the_operator = OPERATOR::SUB;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			return left->value() - right->value();
		}

		return 0.0;
	}
};

class MULNode : public CommutativeOperator
{
public:
	MULNode(){
		the_operator = OPERATOR::MUL;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (m_childNodes.size() != 0)
		{
			double retVal = 0.0f;
			for (ASTNode *n : m_childNodes)
			{
				if (n)
				{
					retVal *= n->value();
				}
			}
			return retVal;
		}
		else
		{
			if (left && right)
			{
				return left->value() * right->value();
			}
		}
		return 0.0;
	}
};

class DIVNode : public NonCommutativeOperator
{
public:
	DIVNode(){
		the_operator = OPERATOR::DIV;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			return left->value() / right->value();
		}

		return 0.0;
	}
};

class POWNode : public NonCommutativeOperator
{
public:
	POWNode(){
		the_operator = OPERATOR::POW;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			return pow(left->value(), right->value());
		}

		return 0.0;
	}
};



class FunctionNode : public OperatorNode
{
public:
	FunctionNode()
	{

	}
};


class SQRTNode : public FunctionNode
{
public:
	SQRTNode(){
		the_operator = OPERATOR::SQRT;
		num_operands = 1;
		left = right = 0;
		parent = 0;
	}

	double value()
	{
		if (left)
		{
			return sqrt(left->value());
		}
		else if (right)
		{
			return sqrt(right->value());
		}
	}
};

class SINNode : public FunctionNode
{
public:
	SINNode(){
		the_operator = OPERATOR::SIN;
		num_operands = 1;
		left = right = 0;
		parent = 0;
	}

	double value()
	{
		if (left)
		{
			return sin(left->value());
		}
		else if (right)
		{
			return sin(right->value());
		}
	}
};

class COSNode : public FunctionNode
{
public:
	COSNode(){
		the_operator = OPERATOR::COS;
		num_operands = 1;
		left = right = 0;
		parent = 0;
	}

	double value()
	{
		if (left)
		{
			return cos(left->value());
		}
		else if (right)
		{
			return cos(right->value());
		}
	}
};

class TANNode : public FunctionNode
{
public:
	TANNode(){
		the_operator = OPERATOR::TAN;
		num_operands = 1;
		left = right = 0;
		parent = 0;
	}

	double value()
	{
		if (left)
		{
			return tan(left->value());
		}
		else if (right)
		{
			return tan(right->value());
		}
	}
};

// Commutative
class ANDNode : public OperatorNode
{
public:
	ANDNode()
	{
		the_operator = OPERATOR::AND;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			return left->value() && right->value();
		}

		return 0.0;
	}
};

// commutative
class LogicalNode : public OperatorNode
{
public:
	LogicalNode()
	{}
};

class ORNode : public LogicalNode
{
public:
	ORNode()
	{
		the_operator = OPERATOR::OR;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			return left->value() || right->value();
		}

		return 0.0;
	}
};



class EQ1Node : public OperatorNode
{
public:
	EQ1Node()
	{
		the_operator = OPERATOR::EQ1;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			if (left->type == 1)
			{
				return ((TerminalNode*)left)->num = right->value();
			}
		}

		return 0.0;
	}
};

class EQNode : public LogicalNode
{
public:
	EQNode()
	{
		the_operator = OPERATOR::EQ;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			return left->value() == right->value();
		}

		return 0.0;
	}
};

class ComparisonNode : public OperatorNode
{
public:
	ComparisonNode(){}
};

class GTNode : public ComparisonNode
{
public:
	GTNode()
	{
		the_operator = OPERATOR::GT;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			return left->value() > right->value();
		}

		return 0.0;
	}
};

class GENode : public ComparisonNode
{
public:
	GENode()
	{
		the_operator = OPERATOR::GE;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			return left->value() >= right->value();
		}

		return 0.0;
	}
};

class LTNode : public ComparisonNode
{
public:
	LTNode()
	{
		the_operator = OPERATOR::LT;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			return left->value() < right->value();
		}

		return 0.0;
	}
};

class LENode : public ComparisonNode
{
public:
	LENode()
	{
		the_operator = OPERATOR::LE;
		num_operands = 2;
		left = right = 0;
		parent = 0;
	}
	double value(){
		if (left && right)
		{
			return left->value() <= right->value();
		}

		return 0.0;
	}
};

ASTNode* ASTNode::TransformNegatives()
{
	string s = typeid(*this).name();



	if (s == "class SUBNode")
	{
		return this;
	}
	else
	{
		ASTNode * temp = 0;

/*		if (typeid(*this).name() == "TerminalNode")
		{
			if (this->value < 0.0f)
			{
				if (this->parent)
				{
					if (this == this->parent->left)
					{

					}
				}
				else
				{
					
					TerminalNode *n = new TerminalNode();
					n->value = this->value;
					n->name = this->name;

					TerminalNode *n2 = new TerminalNode();
					n2->value = -1;
					n2->name = "";

					

				}
			}
		}*/

		if (this->left)
			 temp = left->TransformNegatives();
		
		if (this->right)
			 temp = right->TransformNegatives();

		
		return temp;
	}

	return 0;
	
}

class ASTContainer
{
public:

	ASTContainer(){}
	~ASTContainer(){
		for (int i = 0; i < terminals.size(); i++)
		{
			delete terminals[i];
			terminals[i] = NULL; // what a mess, these stray and draggling nodes 
		}
		terminals.clear();
	}
	TerminalNode *FindNextUnusedTerminal()
	{
		for (int i = 0; i < terminals.size(); i++)
		{
			if (HasTerminalBeenUsed(terminals[i]->sym) == false)
				return terminals[i];
		}
		return terminals[0];
	}

	bool CheckAllTerminalsHaveBeenUsed()
	{
		for (int i = 0; i < terminals.size(); i++)
		{
			if (HasTerminalBeenUsed(terminals[i]->sym) == false)
				return false;
		}
		return true;
	}

	bool HasTerminalBeenUsed(string s)
	{
		for (int i = 0; i < terminals_used.size(); i++)
		{
			if (terminals_used[i] == s)
				return true;
		}

		return false;
	}

	void SetTerminalValue(string symbol, double value)
	{
		for (int i = 0; i < terminals.size(); i++)
		{
			if (terminals[i]->sym == symbol)
			{
				terminals[i]->num = value;
			}
		}
	}
	TerminalNode* AddTerminal(string s, bool bNum)
	{
		for (int i = 0; i < terminals.size(); i++)
		{
			if (terminals[i]->sym == s)
				return terminals[i];
		}
		TerminalNode* t = new TerminalNode(s, bNum);
		terminals.push_back(t);
		return t;
	}

	TerminalNode* RandomTerminal()
	{
		int selector = RandomInt(0, terminals.size() - 1);
		return terminals[selector];

	}
	vector<TerminalNode* > terminals;
	vector< string > terminals_used;
};

class AST
{
private:
	
public:
	ASTContainer* m_container_object = 0;
	OperatorNode* current_crossover_point_parent = 0;
	OperatorNode* current_crossover_point = 0;


	~AST()
	{
		Delete();
	}

	void Delete()
	{
		delete root;
		for (int i = 0; i < terminals.size(); i++)
		{
			delete terminals[i];
			terminals[i] = NULL; // what a mess, these stray and draggling nodes 
		}
		terminals.clear();
	}

	void DeleteTree(ASTNode *n)
	{
		if (n->type == 0)
		{

			if (n->left)
			{
				DeleteTree(n->left);
			}
			if (n->right)
			{
				DeleteTree(n->right);
			}
			delete n;
		}
		else
		{
			delete n;
		}
	}

	void CreateRoot(){
		root = CreateOperatorNode();
	}

	OperatorNode * CreateOperatorNode()
	{
		int selector = (int)RandomInt(0, 18);
		switch (selector)
		{
		case 0: return new ADDNode();
		case 1:return new SUBNode();
		case 2:return new MULNode();
		case 3:return new DIVNode();
		case 4:return new POWNode();
		case 5:return new SQRTNode();
		case 6:return new SINNode();
		case 7:return new COSNode();
		case 8:return new TANNode();
		case 9:return new ANDNode();
		case 10:return new ORNode();
		case 11:return new EQ1Node();
		case 12:return new EQNode();
		case 13:return new GTNode();
		case 14:return new GENode();
		case 15:return new LTNode();
		case 16:return new LENode();
		}

		return 0;
	}

	int depth = 0;

	void CreateTree(OperatorNode *node)
	{

		depth++;
		int terminal_selector = (int)RandomInt(0, 80);
		terminal_selector += depth;
		if (terminal_selector > 40)
		{
			node->left = RandomTerminal();
			node->left->parent = node;
		}
		else
		{
			node->left = CreateOperatorNode();
			CreateTree((OperatorNode*)node->left);
			node->left->parent = node;
		}

		if (node->num_operands == 1)
			return;

		terminal_selector = (int)RandomInt(0, 80);
		terminal_selector += depth;
		if (terminal_selector > 40)
		{
			node->right = RandomTerminal();
			if (node->the_operator == OPERATOR::DIV)
			{
				while (node->right == node->left)
				{
					node->right = RandomTerminal();
				}
			}
			node->right->parent = node;
		}
		else
		{
			node->right = CreateOperatorNode();
			CreateTree((OperatorNode*)node->right);
			node->right->parent = node;
		}
	}

	OperatorNode* CrossPoint()
	{
		current_crossover_point_parent = root;
		return (OperatorNode*)CrossOverPoint(root);
	}

	ASTNode* CrossOverPoint(ASTNode *node)
	{
		int direction_selector = (int)RandomInt(0, 100);
		if (direction_selector  > 50)
		{
			if (node->right && node->right->type == 0)
			{
				int cross_selector = (int)RandomInt(0, 100);
				if (cross_selector > 50)
				{
					current_crossover_point_parent = (OperatorNode*)node;
					return node->right;
				}
				else {
					current_crossover_point_parent = (OperatorNode*)node;
					return CrossOverPoint(node->right);
				}
			}
			else if (node->left && node->left->type == 0)
			{
				int cross_selector = (int)RandomInt(0, 100);
				if (cross_selector > 50)
				{
					current_crossover_point_parent = (OperatorNode*)node;
					return node->left;
				}
				else {
					current_crossover_point_parent = (OperatorNode*)node;
					return CrossOverPoint(node->left);
				}
			}
			else return node;
		}
		else
		{
			if (node->left && node->left->type == 0)
			{
				int cross_selector = (int)RandomInt(0, 100);
				if (cross_selector > 50)
				{
					current_crossover_point_parent = (OperatorNode*)node;
					return node->left;
				}
				else {
					current_crossover_point_parent = (OperatorNode*)node;
					return CrossOverPoint(node->left);
				}
			}
			else if (node->right && node->right->type == 0)
			{
				int cross_selector = (int)RandomInt(0, 100);
				if (cross_selector > 50)
				{
					current_crossover_point_parent = (OperatorNode*)node;
					return node->right;
				}
				else
				{
					current_crossover_point_parent = (OperatorNode*)node;
					return CrossOverPoint(node->right);
				}
			}
			else return node;
		}

		return 0;
	}

	float Execute()
	{
		return root->value();
	}

	void PrintTree(ASTNode *n)
	{
		static int print_depth = 0;
		print_depth++;
		if (!n)
		{
			print_depth--;
			return;
		}

		//for (int i = 0; i < print_depth; i++)
		//	cout << " ";

		if (n->type == 0)
		{
			cout /*<< operator_array[((OperatorNode*)n)->the_operator]*/ << "(";

			string s2 = typeid(*n).name();
			if (s2 == "class ADDNode")
			{
				ADDNode* an = (ADDNode*)n;
				if (an->m_childNodes.size() != 0)
				{
					//for (ASTNode *p : an->m_childNodes)
					for (int k = 0; k < an->m_childNodes.size(); k++)
					{
						if (an->m_childNodes[k])
						{
							PrintTree(an->m_childNodes[k]);

							if (k < an->m_childNodes.size() - 1)
								cout << " + ";
						}
					}
					if (n->left)
					{
					//	PrintTree(n->left);
						delete n->left;
						n->left = NULL;
					}

					//cout << ", ";

					if (n->right)
					{
						//PrintTree(n->right);
						delete n->right;
						n->right = NULL;
					}
					cout << ")";
					//
				}
				else
				{
					if (n->left)
					{
						PrintTree(n->left);
					}

					cout << " + ";

					if (n->right)
					{
						PrintTree(n->right);
					}
				    cout << ")";
				}
			}
			if (s2 == "class MULNode")
			{
				MULNode* an = (MULNode*)n;
				if (an->m_childNodes.size() != 0)
				{

					for (int k = 0; k < an->m_childNodes.size(); k++)
					{
						if (an->m_childNodes[k])
						{
							PrintTree(an->m_childNodes[k]);

							if (k  < an->m_childNodes.size() - 1)
								cout << "*";
						}
					}
					/*if (n->left)
					{
							PrintTree(n->left);
					//	delete n->left;
					//	n->left = NULL;
					}

					cout << " * ";

					if (n->right)
					{
						PrintTree(n->right);
					//	delete n->right;
					//	n->right = NULL;
					}*/
					cout << ")";
					//
				}
				else
				{
					if (n->left)
					{
						PrintTree(n->left);
					}

					cout << "*";

					if (n->right)
					{
						PrintTree(n->right);
					}
					cout << ")";
				}
			}
			else
			{ 
				if (n->left)
				{
					PrintTree(n->left);
				}

				switch (((OperatorNode*)n)->the_operator)
				{

				case OPERATOR::DIV:
					cout << "/";
					break;
				case OPERATOR::POW:
					cout << "^";
					break;
				case OPERATOR::SQRT:
					cout << ", ";
					break;
				
				
				case OPERATOR::AND:
					cout << " AND ";
					break;
				case OPERATOR::OR:
					cout << " OR ";
					break;

				case OPERATOR::EQ1:
					cout << " = ";
					break;
				case OPERATOR::EQ:
					cout << " == ";
					break;
				case OPERATOR::GT:
					cout << " > ";
					break;
				case OPERATOR::GE:
					cout << " >= ";
					break;
				case OPERATOR::LT:
					cout << " < ";
					break;
				case OPERATOR::LE:
					cout << " <= ";
					break;
				}

				

				if (n->right)
				{
					PrintTree(n->right);
				}
				cout << ")";
			}
		}
		else if (n->type == 2)
		{
			
			cout << flow_control_array[((FlowControlNode*)n)->the_flow_control] << "(";
			
			if (n->left)
			{
				PrintTree(n->left);
			}

			cout << "; ";

			if (n->right) 
			{
				PrintTree(n->right);
			}
			
			cout << ")";
		}
		else
		{
			cout  << ((TerminalNode*)n)->sym ;
		}


		print_depth--;
	}

	void SetParentObject(ASTContainer *container){
		this->m_container_object = container;
	}

	TerminalNode* RandomTerminal()
	{
		if (m_container_object)
		{
			return this->RandomTerminal2();
		}
		else
		{
			int selector = RandomInt(0, terminals.size() - 1);
			return terminals[selector];
		}
		return 0;
	} 

	TerminalNode* RandomTerminal2()
	{
		return this->m_container_object->RandomTerminal();
	}

	TerminalNode* AddTerminal(string s, bool bNum)
	{
		if (m_container_object == 0)
		{
			for (int i = 0; i < terminals.size(); i++)
			{
				if (terminals[i]->sym == s)
					return terminals[i];
			}
			TerminalNode* t = new TerminalNode(s, bNum);
			terminals.push_back(t);
			return t;
		}
		else
		{
			return m_container_object->AddTerminal(s, bNum);
		}
	}


	TerminalNode *FindNextUnusedTerminal()
	{
		if (m_container_object)
		{
			m_container_object->FindNextUnusedTerminal();
		}
		else
		{
			for (int i = 0; i < terminals.size(); i++)
			{
				if (HasTerminalBeenUsed(terminals[i]->sym) == false)
					return terminals[i];
			}
			return terminals[0];
		}
		return 0;
	}

	bool CheckAllTerminalsHaveBeenUsed()
	{
		if (m_container_object)
		{
			m_container_object->CheckAllTerminalsHaveBeenUsed();
		}
		else
		{
			for (int i = 0; i < terminals.size(); i++)
			{
				if (HasTerminalBeenUsed(terminals[i]->sym) == false)
					return false;
			}
		}
		return true;
	}

	bool HasTerminalBeenUsed(string s)
	{
		if (m_container_object)
		{
			m_container_object->HasTerminalBeenUsed(s);
		}
		else
		{
			for (int i = 0; i < terminals_used.size(); i++)
			{
				if (terminals_used[i] == s)
				return true;
			}
		}

		return false;
	}

	void SetTerminalValue(string symbol, double value)
	{
		if (m_container_object)
		{
			m_container_object->SetTerminalValue(symbol, value);
		}
		else
		{
			for (int i = 0; i < terminals.size(); i++)
			{
				if (terminals[i]->sym == symbol)
				{
					terminals[i]->num = value;
				}
			}
		}
	}

	AST* CopyTree()
	{
		if (!root)return 0;

		AST * tree = new AST();
		//tree->root = new ASTNode();
		tree->root = (OperatorNode*)copy_node(this->root);
		tree->SetParentObject(this->m_container_object);
		tree->fitness = this->fitness;

		return tree;
	}

	ASTNode* copy_node(ASTNode* tree_node)
	{
		//ASTNode* temp=0;
		if (tree_node->type == 0)
		{
			OperatorNode*  temp = (OperatorNode*)tree_node;
			//ADD = 0, SUB, MUL, DIV, POW, SQRT
			OperatorNode*  new_node = 0;
			switch (temp->the_operator)
			{
			case OPERATOR::ADD:
				new_node = new ADDNode();
				break;
			case OPERATOR::SUB:
				new_node = new SUBNode();
				break;
			case OPERATOR::MUL:
				new_node = new MULNode();
				break;
			case OPERATOR::DIV:
				new_node = new DIVNode();
				break;
			case OPERATOR::POW:
				new_node = new POWNode();
				break;
			case OPERATOR::SQRT:
				new_node = new SQRTNode();
				break;
			case OPERATOR::SIN:
				new_node = new SINNode();
				break;
			case OPERATOR::COS:
				new_node = new COSNode();
				break;
			case OPERATOR::TAN:
				new_node = new TANNode();
				break;
			case OPERATOR::AND:
				new_node = new ANDNode();
				break;
			case OPERATOR::OR:
				new_node = new ORNode();
				break;

			case OPERATOR::EQ1:
				new_node = new EQ1Node();
				break;
			case OPERATOR::EQ:
				new_node = new EQNode();
				break;
			case OPERATOR::GT:
				new_node = new GTNode();
				break;
			case OPERATOR::GE:
				new_node = new GENode();
				break;
			case OPERATOR::LT:
				new_node = new LTNode();
				break;
			case OPERATOR::LE:
				new_node = new LENode();
				break;
			}



			if (tree_node->left)
			{
				new_node->left = copy_node(tree_node->left);
				if (new_node->left)
					new_node->left->parent = new_node;
			}
			if (tree_node->right)
			{
				new_node->right = copy_node(tree_node->right);
				if (new_node->right)
					new_node->right->parent = new_node;
			}

			return new_node;
		}
		else if (tree_node->type == 2)
		{
			FlowControlNode*  temp = (FlowControlNode*)tree_node;
			FlowControlNode*  new_node = 0;

			switch (temp->the_flow_control)
			{
			case FLOW_CONTROL::IFTHEN:
				new_node = new IFTHENNode();
				break;
			case FLOW_CONTROL::ALSO:
				new_node = new ALSONode();
				break;

			}

			if (tree_node->left)
			{
				new_node->left = copy_node(tree_node->left);
				if (new_node->left)
					new_node->left->parent = new_node;
			}
			if (tree_node->right)
			{
				new_node->right = copy_node(tree_node->right);
				if (new_node->right)
					new_node->right->parent = new_node;
			}

			return new_node;
		}
		else
		{
			TerminalNode* new_node = (TerminalNode*)tree_node;
			return new_node;

		}

		return 0;
	}

	void Mutate()
	{
		int N = RandomInt(0, 100);
		if (N > 50)
		{
			OperatorNode* op = this->CreateOperatorNode();
			if ( op->the_operator == OPERATOR::SQRT)
			{ 
				op->left = this->root;
				this->root->parent = op;

				this->root = op;
			}
			else
			{
				op->left = this->root->left;

				if (op->left && op->left->type == 0)
					op->left->parent = op;
				op->right = this->root->right;

				if (op->right && op->right->type == 0)
					op->right->parent = op;

				this->root = op;
			}
		}
		else
		{
			Mutate_R(root);
		}
	}


	OperatorNode * CreateOperatorNode_ForMutation(OPERATOR old_op)
	{
		if (old_op == OPERATOR::SQRT)
		{
			int sel_single_branch = (int)RandomInt(0, 3);
			switch (sel_single_branch)
			{
			case 0:
				return new SQRTNode();
			case 1:
				return new SINNode();
			case 2:
				return new COSNode();
			case 3:
				return new TANNode();
			default:
				return new SQRTNode();
			}
			
		}

		int selector = (int)RandomInt(0, 10);
		switch (selector)
		{
		case 0: return new ADDNode();
		case 1:return new SUBNode();
		case 2:return new MULNode();
		case 3:return new DIVNode();
		case 4:return new POWNode();
		case 5:return new EQNode();
		
		case 6:return new GTNode();
		case 7:return new GENode();
		case 8:return new LTNode();
		case 9:return new LENode();
		case 10:return new EQ1Node(); // not currently emitting
		}

		return 0;
	}

	void Mutate_R(ASTNode* node)
	{
		if (!node )return;
		

		bool MUTATE_FLAG = false;

		bool MUTATE_LEFT_CHILD = false;
		bool MUTATE_RIGHT_CHILD = false;
		
		int N = RandomInt(0, 100);

		int LEFT_OR_RIGHT_FIRST = RandomInt(0, 100);

		if (N > 50)
		{
			
			
			if (LEFT_OR_RIGHT_FIRST > 50)
			{
				if (node->left)
				{
					if (node->left->type == 1)
					{

						TerminalNode* new_terminal = this->RandomTerminal();
						while (new_terminal == node->left)
						{
							new_terminal = this->RandomTerminal();
						}
						node->left = new_terminal;
					}
					else
					{
						MUTATE_LEFT_CHILD = true;
					}
				}
				else if (node->right )
				{
					if (node->right->type == 1)
					{
						TerminalNode* new_terminal = this->RandomTerminal();
						while (new_terminal == node->right)
						{
							new_terminal = this->RandomTerminal();
						}
						node->right= new_terminal;
					}
					else
					{
						MUTATE_RIGHT_CHILD = true;
					}
				}
				
			}

			else 
			{
				if (node->right)
				{
					if (node->right->type == 1)
					{
						TerminalNode* new_terminal = this->RandomTerminal();
						while (new_terminal == node->right)
						{
							new_terminal = this->RandomTerminal();
						}
						node->right = new_terminal;
					}
					else
					{
						MUTATE_RIGHT_CHILD = true;
					}
				}
				else if (node->left)
				{
					if (node->left->type == 1)
					{
						TerminalNode* new_terminal = this->RandomTerminal();
						while (new_terminal == node->left)
						{
							new_terminal = this->RandomTerminal();
						}
						node->left = new_terminal;
					}
					else
					{
						MUTATE_LEFT_CHILD = true;

						
					}
				}
				
				
			}
			
		}
		else
		{
			if ( LEFT_OR_RIGHT_FIRST > 50 )
			{
				if ((node->left) && (node->left->type == 0))
				{
					Mutate_R(node->left);
					MUTATE_FLAG = false;
				}
				else if ((node->right) && (node->right->type == 0))
				{
					Mutate_R(node->right);
					MUTATE_FLAG = false;
				}
				else {
					MUTATE_FLAG = true;
				}
			}
			else
			{
				if ((node->right) && (node->right->type == 0))
				{
					Mutate_R(node->right);
					MUTATE_FLAG = false;
				}
				else if ((node->left) && (node->left->type == 0))
				{
					Mutate_R(node->left);
					MUTATE_FLAG = false;
				}
				else {
					MUTATE_FLAG = true;
				}
			}

		}

		if ((MUTATE_FLAG==true) && node && node->type == 0 )
		{

			OperatorNode* op = this->CreateOperatorNode_ForMutation(((OperatorNode*)node)->the_operator);
			
			op->left = node->left;
			op->right = node->right;

			
			if ((node) && (node->parent) && (node->parent->left) && (node->parent->left == node))
			{
				node->parent->left = op;
				op->parent = node->parent;
				delete node;
			}
			else if ((node)&&(node->parent) && (node->parent->right) && (node->parent->right == node))
			{
				node->parent->right = op;
				op->parent = node->parent;
				delete node;
			}
		}

		if ((MUTATE_LEFT_CHILD==true) && node && node->left && node->left->type == 0)
		{
			
			OperatorNode* op = this->CreateOperatorNode_ForMutation(((OperatorNode*)node->left)->the_operator);
			

			if (node->left && node->left->left)
				op->left = node->left->left;
			if (node->left && node->left->right)
				op->right = node->left->right;

			op->parent = node;
			node->left->left = 0;
			node->left->right = 0;
			delete node->left;
			node->left = op;
			
			
		}

		if ((MUTATE_RIGHT_CHILD==true) && node && node->right && node->right->type == 0)
		{
			if (((OperatorNode*)node->right)->the_operator == OPERATOR::SQRT)
				return;

			OperatorNode* op = this->CreateOperatorNode_ForMutation(((OperatorNode*)node->right)->the_operator);
			

			if (node->right && node->right->left)
				op->left = node->right->left;
			if (node->right && node->right->right)
				op->right = node->right->right;

			
			op->parent = node;
			node->right->left = 0;
			node->right->right = 0;
			delete node->right;
			node->right = op;
			
		}




	}


	void SimplicationMode_01()
	{
		
	}

	void RemoveDegenerate(ASTNode* inputNode)
	{
		if (inputNode->type != 1)
		{

			string s2 = typeid(*inputNode).name();
			if (s2 == "class ADDNode")
			{
				
				if (((ADDNode*)inputNode)->m_childNodes.size()==0 && !(inputNode->left) && !(inputNode->right))
				{

					delete inputNode;
				}
				

				{
					for (ASTNode* n : ((ADDNode*)inputNode)->m_childNodes)
					{
						RemoveDegenerate(n);
					}
				}
			}
			else
			{ 
				if (inputNode->left)
				{
					RemoveDegenerate(inputNode->left);
				}
			
				if (inputNode->right)
				{
					RemoveDegenerate(inputNode->right);
				}
			}

		}
	}

	void RemoveNode(ASTNode *n)
	{

	}

	ASTNode* TransformDivRule3(ASTNode* M)
	{
		if (!M) return 0;

		string s3 = typeid(*M).name();
		if (s3 == "class MULNode")
		{
			if (((MULNode*)M)->m_childNodes.size() == 0 && M->left && M->right)
			{ 
				ASTNode *M_left = M->left;
				ASTNode *M_right = M->right;

				string s4 = typeid(*M_left).name();
				string s5 = typeid(*M_right).name();
				if (s4 == "class DIVNode" && M_left->left && M_left->right)
				{
					ASTNode* numDiv = M_left->left;
					ASTNode* denDiv = M_left->right;

					MULNode *newMul = new MULNode();

					newMul->m_childNodes.push_back(numDiv);
					newMul->m_childNodes.push_back(M->right);

					numDiv->parent = newMul;
					M->right->parent = newMul;

					DIVNode *newDiv = new DIVNode();
					newDiv->left = newMul;
					newDiv->right = denDiv;

					newMul->parent = newDiv;
					newDiv->right->parent = newDiv;

					newDiv->parent = M->parent;

					M->right = NULL;
					
					M->left = NULL;

					if (M == root)
						root = newDiv;

					delete M;

				}
				else if (s5 == "class DIVNode" && M_right->left && M_right->right)
				{
					ASTNode* numDiv = M_right->left;
					ASTNode* denDiv = M_right->right;

					MULNode *newMul = new MULNode();

					newMul->m_childNodes.push_back(numDiv);
					newMul->m_childNodes.push_back(M->left);

					numDiv->parent = newMul;
					M->left->parent = newMul;

					DIVNode *newDiv = new DIVNode();
					newDiv->left = newMul;
					newDiv->right = denDiv;

					newMul->parent = newDiv;
					newDiv->right->parent = newDiv;

					newDiv->parent = M->parent;

					M->right = NULL;

					M->left = NULL;

					if (M == root)
						root = newDiv;

					delete M;
				}
			}
			else
			{
				ASTNode *firstFoundDiv = 0;
				for (ASTNode *n : ((MULNode*)M)->m_childNodes)
				{
					string s4 = typeid(*n).name();
					if (s4 == "class DIVNode")
					{
						firstFoundDiv = n;
						break;
					}
				}

				if (firstFoundDiv != 0 && firstFoundDiv->left && firstFoundDiv->right)
				{
					ASTNode* numDiv = firstFoundDiv->left;
					ASTNode* denDiv = firstFoundDiv->right;

					MULNode *newMul = new MULNode();
					for (ASTNode *p : ((MULNode*)M)->m_childNodes)
					{
						if (p != firstFoundDiv)
							newMul->m_childNodes.push_back(p);
					}

					newMul->m_childNodes.push_back(numDiv);

					DIVNode *newDiv = new DIVNode();
					newDiv->left = newMul;
					newMul->parent = newDiv;

					newDiv->right = denDiv;
					denDiv->parent = newDiv;
					
					newDiv->parent = M->parent;

					if ( M->parent)
					{
						string s_mp = typeid(*M->parent).name();
						if (s_mp == "class MULNode")
						{
							MULNode* M_parent = (MULNode*)M->parent;

							int indx = 0;
							bool found = false;
							for (ASTNode *n_mp : M_parent->m_childNodes)
							{
								if (n_mp == M)
								{
									found = true;
									break;
								}
								indx++;
							}
							if (found)
							{
								M_parent->m_childNodes.erase(M_parent->m_childNodes.begin() + indx);
								M_parent->m_childNodes.push_back(newDiv);
							}
							else
							{
								if (M->IsThisTheLeftChild())
								{
									M->parent->left = NULL;

									M->parent->left = newDiv;

								}
								else
								{
									M->parent->right = NULL;
									M->parent->right = newDiv;

								}
							}



						}
						else if (s_mp == "class ADDNode")
						{
							ADDNode* M_parent = (ADDNode*)M->parent;

							int indx = 0;
							bool found = false;
							for (ASTNode *n_mp : M_parent->m_childNodes)
							{
								if (n_mp == M)
								{
									found = true;
									break;
								}
								indx++;
							}
							if (found)
							{
								M_parent->m_childNodes.erase(M_parent->m_childNodes.begin() + indx);
								M_parent->m_childNodes.push_back(newDiv);
							}
							else
							{
								if (M->IsThisTheLeftChild())
								{
									M->parent->left = NULL;

									M->parent->left = newDiv;

								}
								else
								{
									M->parent->right = NULL;
									M->parent->right = newDiv;

								}
							}
						}
						else
						{ 
							if (M->IsThisTheLeftChild())
							{
								M->parent->left = NULL;

								M->parent->left = newDiv;
							
							}
							else
							{
								M->parent->right = NULL;
								M->parent->right = newDiv;
							
							}
						}

					}

					M->left = NULL;
					M->right = NULL;

					((MULNode*)M)->m_childNodes.clear();

					if (M == root)
						root = newDiv;

					delete M;
					
				}

			}

		}
		else
		{
			if (s3 == "class ADDNode")
			{
				if (((ADDNode*)M)->m_childNodes.size() != 0)
				{
					for (ASTNode *i : ((ADDNode*)M)->m_childNodes)
					{
						TransformDivRule3(i);
					}
				}
				else
				{
					if (M->left)
						TransformDivRule3(M->left);

					if (M->right)
						TransformDivRule3(M->right);
				}
			}
			else
			{
				if (M->left)
					TransformDivRule3(M->left);

				if (M->right)
					TransformDivRule3(M->right);
			}
		}


		return 0;
	}


	ASTNode* TransformDivRule2(ASTNode* d1)
	{
		if (!d1) return 0;

		if (d1->right)
		{
			string s3 = typeid(*d1->right).name();
			if (s3 == "class DIVNode")
			{
				ASTNode *d2 = d1->right;

				if (d2->left)
				{
					d1->right = d2->left;
					d1->right->parent = d1;

					MULNode *m = new MULNode;
					d1->left->parent = m;
					d2->left->parent = m;
					m->m_childNodes.push_back(d2->right);
					m->m_childNodes.push_back(d1->left);

					d1->left = m;

					m->parent = d1;

					d2->left = 0;
					d2->right = 0;

					delete d2;
				}
			}
			else
			{
				if (d1->left)
					TransformDivRule2(d1->left);

				if (d1->right)
					TransformDivRule2(d1->right);

			}

		}
		else
		{
			string s4 = typeid(*d1).name();
			if ((s4 == "class MULNode") && ((MULNode*)d1)->m_childNodes.size() != 0)
			{
				for (int p = 0; p < ((MULNode*)d1)->m_childNodes.size(); p++)
				{
					if (d1 && ((MULNode*)d1)->m_childNodes[p])
						TransformDivRule2(((MULNode*)d1)->m_childNodes[p]);
				}
			}
			else if ((s4 == "class ADDNode") && ((ADDNode*)d1)->m_childNodes.size() != 0)
			{
				for (int p = 0; p <((ADDNode*)d1)->m_childNodes.size(); p++)
				{
					if (d1 && ((ADDNode*)d1)->m_childNodes[p])
						TransformDivRule2(((ADDNode*)d1)->m_childNodes[p]);
				}
			}
			else
				if (d1->left)
					TransformDivRule2(d1->left);



		}
		return 0;
	}

	ASTNode* TransformDivRule1(ASTNode* d1)
	{
		if (!d1) return 0;

		if (d1->left)
		{
			string s2 = typeid(*d1->left).name();
			if (s2 == "class DIVNode")
			{
				ASTNode *d2 = d1->left;

				if (d2->left)
				{
					d1->left = d2->left;
					d1->left->parent = d1;

					MULNode *m = new MULNode;
					d2->right->parent = m;
					m->m_childNodes.push_back(d2->right);
					m->m_childNodes.push_back(d1->right);
					
					d1->right->parent = m;
					d1->right = m;

					m->parent = d1;

					d2->left = 0;
					d2->right = 0;

					delete d2;
				}
			}
			else
			{
				string s4 = typeid(*d1).name();
				
				if ((s4 == "class MULNode") && ((MULNode*)d1)->m_childNodes.size() != 0)
				{
					for (auto p : ((MULNode*)d1)->m_childNodes)
						TransformDivRule1(p);
				}
				else if ((s4 == "class ADDNode") && ((ADDNode*)d1)->m_childNodes.size() != 0)
				{
					for (auto p : ((ADDNode*)d1)->m_childNodes)
						TransformDivRule1(p);
				}
				else
				{ 
					if ( d1 && d1->left )
						TransformDivRule1(d1->left);
				
					if (d1 && d1->right)
						TransformDivRule1(d1->right);
				}
			}
			
		}
		else
		{
			string s4 = typeid(*d1).name();
			if ((s4 == "class MULNode") && ((MULNode*)d1)->m_childNodes.size() != 0)
			{
				for (int p = 0; p < ((MULNode*)d1)->m_childNodes.size(); p++)
				{
					if (d1 && ((MULNode*)d1)->m_childNodes[p])
						TransformDivRule1(((MULNode*)d1)->m_childNodes[p]);
				}
			}
			else if ((s4 == "class ADDNode") && ((ADDNode*)d1)->m_childNodes.size() != 0)
			{
				for (int p = 0; p <((ADDNode*)d1)->m_childNodes.size(); p++)
				{
					if (d1 && ((ADDNode*)d1)->m_childNodes[p])
						TransformDivRule1(((ADDNode*)d1)->m_childNodes[p]);
				}
			}
			else
			{


			if (d1->right)
				TransformDivRule1(d1->right);
			}
		}
		return 0;
	}

	ASTNode* TransformAdd(ASTNode* inputNode, ADDNode* addParent)
	{
		if (inputNode == addParent)
		{
			if (addParent->m_childNodes.size() != 0)
			{
				//for (ASTNode *n : addParent->m_childNodes)
				for (int i = 0; i < addParent->m_childNodes.size(); i++)
				{
					TransformAdd(addParent->m_childNodes[i], addParent);
				}
			}
			else
			{
				if( inputNode->left )
					TransformAdd(inputNode->left, addParent);
			
				if( inputNode->right)
					TransformAdd(inputNode->right, addParent);
			}
		}
		else
		{
			string s2 = typeid(*inputNode).name();
			if (s2 == "class ADDNode")
			{

				if (inputNode->left)
					TransformAdd(inputNode->left, addParent);

				if (inputNode->right)
					TransformAdd(inputNode->right, addParent);

			}
			else
			{
				if (inputNode->parent)
				{
					if (inputNode->IsThisTheLeftChild())
						inputNode->parent->left = NULL;
					else inputNode->parent->right = NULL;
				}

				inputNode->parent = addParent;
				addParent->m_childNodes.push_back(inputNode);
				
				if (inputNode->left)
					TransformComm(inputNode->left);

				if (inputNode->right)
					TransformComm(inputNode->right);

			}

		}

		return inputNode;
	}

	ASTNode* TransformMul(ASTNode* inputNode, MULNode* addParent)
	{
		if (inputNode == addParent)
		{
			if (addParent->m_childNodes.size() != 0)
			{
				bool found = false;
				int indx = 0;
				//for (ASTNode *n : addParent->m_childNodes)
				for (int i = 0; i < addParent->m_childNodes.size(); i++)
				{
					string s2 = typeid(*addParent->m_childNodes[i]).name();
					
					if (s2 == "class MULNode")
					{
						found = true;
						break;
					}
					//
					indx++;
				}

				if ( found )
				{
					TransformMul(addParent->m_childNodes[indx], addParent);
					addParent->m_childNodes.erase(addParent->m_childNodes.begin() + indx);
					}
			}
			else
			{
				if (inputNode->left)
					TransformMul(inputNode->left, addParent);

				if (inputNode->right)
					TransformMul(inputNode->right, addParent);
			}
		}
		else
		{
			string s2 = typeid(*inputNode).name();
			if (s2 == "class MULNode")
			{
				if (((MULNode*)inputNode)->m_childNodes.size() != 0)
				{
					int indx = 0;
					bool found = false;
					//for (ASTNode *n : addParent->m_childNodes)
					for (auto i : ((MULNode*)inputNode)->m_childNodes)
					{
						addParent->m_childNodes.push_back(i);
						i->parent = addParent;
						if (i == inputNode)
						{
							found = true;

							break;
						}//TransformMul(i, addParent);
						indx++;
					}
					if (found)
					{
						addParent->m_childNodes.erase(addParent->m_childNodes.begin() + indx);

					}
					//((MULNode*)inputNode)->m_childNodes.clear();
				}
			
				else
				{
				if (inputNode->left)
					TransformMul(inputNode->left, addParent);

				if (inputNode->right)
					TransformMul(inputNode->right, addParent);
				}
			}
			else
			{
				if (inputNode->parent)
				{
					if (inputNode->IsThisTheLeftChild())
						inputNode->parent->left = NULL;
					else inputNode->parent->right = NULL;
				}

				inputNode->parent = addParent;
				addParent->m_childNodes.push_back(inputNode);

				if (inputNode->left)
					TransformCommMUL(inputNode->left);

				if (inputNode->right)
					TransformCommMUL(inputNode->right);

			}

		}

		return inputNode;
	}

	ASTNode* TransformCommMUL(ASTNode* inputNode)
	{

		string s = typeid(*inputNode).name();


		if (s == "class MULNode")
		{
			TransformMul(inputNode, (MULNode*)inputNode);
		}
		else
		{
			if (inputNode->left)
				TransformCommMUL(inputNode->left);

			if (inputNode->right)
				TransformCommMUL(inputNode->right);

		}
		return 0;
	}
	ASTNode* TransformComm(ASTNode* inputNode)
	{
		
		string s = typeid(*inputNode).name();

		if (s == "class ADDNode")
		{
			TransformAdd(inputNode, (ADDNode*)inputNode);
		}
		else
		{
			if (inputNode->left)
				TransformComm(inputNode->left);

			if (inputNode->right)
				TransformComm(inputNode->right);

		}
		return 0;
	}

	void TransformCommutative()
	{
		ASTNode * temp = TransformComm(root);// root->TransformNegatives();
	}

	void TransformCommutativeMUL()
	{
		ASTNode * temp = TransformCommMUL(root);// root->TransformNegatives();
	}

	ASTNode* TransformNeg(ASTNode* inputNode)
	{
		string s = typeid(*inputNode).name();

		if (s == "class SUBNode")
		{
			if (inputNode->parent != 0)
			{
				if (inputNode->IsThisTheLeftChild())
				{
					ASTNode *inputNodeParent = inputNode->parent;

					ASTNode *inputNodeLeft = 0;
					if (inputNode->left)
						inputNodeLeft = inputNode->left;

					ASTNode *inputNodeRight = 0;
					if (inputNode->right)
						inputNodeRight = inputNode->right;

					MULNode *mulNode = new MULNode();
					mulNode->AddChild(this->AddTerminal("-1", true));
					mulNode->AddChild ( inputNodeRight);

					for (ASTNode* n : mulNode->m_childNodes)
						n->parent = mulNode;

					ADDNode *addNode = new ADDNode();
					addNode->AddChild(inputNodeLeft);
					addNode->AddChild( mulNode);

					for (ASTNode* n : addNode->m_childNodes)
						n->parent = addNode;

					inputNodeParent->left = addNode;

					addNode->parent = inputNodeParent;


					inputNode->left = NULL;
					inputNode->right = NULL;

					delete inputNode;
				}
				else
				{
					ASTNode *inputNodeParent = inputNode->parent;

					ASTNode *inputNodeLeft = 0;
					if (inputNode->left)
						inputNodeLeft = inputNode->left;

					ASTNode *inputNodeRight = 0;
					if (inputNode->right)
						inputNodeRight = inputNode->right;

					MULNode *mulNode = new MULNode();
					mulNode->AddChild(this->AddTerminal("-1", true));
					mulNode->AddChild (inputNodeRight);

					for (ASTNode* n : mulNode->m_childNodes)
						n->parent = mulNode;


					ADDNode *addNode = new ADDNode();
					addNode->AddChild(inputNodeLeft);
					addNode->AddChild( mulNode);

					for (ASTNode* n : addNode->m_childNodes)
						n->parent = addNode;

					addNode->parent = inputNodeParent;

					inputNode->left = NULL;
					inputNode->right = NULL;

					delete inputNode;

					inputNodeParent->right = addNode;

				}
			}
			else
			{
				if (inputNode->left)
				{
					ASTNode *inputNodeLeft = inputNode->left;

					ASTNode *inputNodeRight = 0;
					if (inputNode->right)
						inputNodeRight = inputNode->right;

					MULNode *mulNode = new MULNode();
					mulNode->AddChild(this->AddTerminal("-1", true));
					//mulNode->right = inputNodeRight;
					mulNode->AddChild(inputNodeRight);

					for (ASTNode* n : mulNode->m_childNodes)
						n->parent = mulNode;
					
					
					ADDNode *addNode = new ADDNode();
					addNode->AddChild(inputNodeLeft);
					addNode->AddChild(mulNode);

					for (ASTNode* n : addNode->m_childNodes)
						n->parent = addNode;

					inputNode->left = NULL;
					inputNode->right = NULL;

					delete inputNode;

					root = (OperatorNode*)addNode;
				}
				else
				{
					ASTNode *inputNodeRight = 0;
					if (inputNode->right)
						inputNodeRight = inputNode->right;

					MULNode *mulNode = new MULNode();
					mulNode->AddChild(this->AddTerminal("-1", true));
					mulNode->AddChild(inputNodeRight);

					for (ASTNode* n : mulNode->m_childNodes)
						n->parent = mulNode;

					//mulNode->right->parent = mulNode;
					//mulNode->left->parent = mulNode;


					inputNode->left = NULL;
					inputNode->right = NULL;

					delete inputNode;

					root = (OperatorNode*)mulNode;
				}
			}

			return inputNode;
		}
		else
		{
			ASTNode * temp = 0;
			string sv = typeid(*inputNode).name();
			if ((sv == "class ADDNode") || (sv == "class MULNode"))
			{
				MULNode *com_n = ((MULNode *)inputNode);
				if (((MULNode *)inputNode)->m_childNodes.size() != 0)
				{
					for (ASTNode* n : com_n->m_childNodes)
					{
						if (n)
							temp = TransformNeg(n);
					}
				}
			}
			else
			{ 
				if (inputNode->left)
					temp = TransformNeg(inputNode->left);// ->TransformNegatives();

				if (inputNode->right)
					temp = TransformNeg(inputNode->right);
			}
			return temp;
		}

		return 0;
	}


	void TransformNegatives()
	{
		ASTNode * temp = TransformNeg(root);// root->TransformNegatives();
	}

	float fitness=0.0f;
	bool swapped = false;

	vector< OperatorNode* > operators;

	vector< TerminalNode* > terminals;

	vector< string > terminals_used;

	OperatorNode *root;
};
#endif