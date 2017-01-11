// SimplePerceptron.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"
#include <iostream>

#include <windows.h>  // for MS Windows
#include "Utils.h"

#include "GeneticProgramming.h"
#include "AST.h"

#include <typeinfo>




int width = 640;
int height = 480;





/*=================================================================
Main
=================================================================*/

int _tmain(int argc, char* argv[])
{
	srand(10);   /// TWEAK


	/*=================================================================
	tests of the parser, lexer, ast builder and REPL
	=================================================================*/
	string s2 = "x%y;";
	//string s4 = "if ((m eq 1) and((n eq 2) or(mm eq 10))) then { (t = 1) and(x = sqrt(sin(y + 5.25))) }; ";
	string s44 = "-5; a = b-c; y = x*2 - x + 5*(c-d); a+b+c+d+e;5*d*c*e*f;(k/6)/a;k/(6/a);a*(b/(c*d*e));";
	string s4 = "(k/6)/a;k/(6/a);a*(b/(c*d*e));(a*(c/d))*(e/(f/(g*h)));";
	string s5 = "if (m eq 1) then(x = sqrt(sin(y + 5.25))) else (x = 5); ";


	std::vector<std::string> x = split(s4, ';');
	for (int j = 0; j < x.size(); j++)
		cout << x[j] << endl;

	SimpleScript *simple_script = new SimpleScript[x.size()];
	ASTContainer *container = new ASTContainer();
	for (int j = 0; j < x.size(); j++)
	{
		simple_script[j].abstractTree->SetParentObject(container);

		simple_script[j].strInfixExpression = x[j];
		simple_script[j].arrayInfixExpression = simple_script[j].strSplit(simple_script[j].strInfixExpression);

		simple_script[j].parse();

		cout << "-------------------------------------------" << endl;
		cout << "printing input expression: " << endl;
		cout << simple_script[j].strInfixExpression << endl << endl;
		cout << "printing array postfix expression:" << endl;

		for (int i = 0; i < simple_script[j].arrayPostfixExpression.size(); i++) 
			std::cout << simple_script[j].arrayPostfixExpression[i] << " ";
		
		cout << endl;

		vector<string> output = simple_script[j].postfixEvaluateList(simple_script[j].arrayPostfixExpression);
		vector<ASTNode*> node_stack = simple_script[j].postfixEvaluateListTree(simple_script[j].arrayPostfixExpression);

		simple_script[j].abstractTree->root = (OperatorNode*)node_stack[0];

		// apply the simplification ops several times
		for (int ii = 0; ii < 4; ii++)
		{
			simple_script[j].abstractTree->TransformNegatives();
			simple_script[j].abstractTree->TransformCommutative();
			simple_script[j].abstractTree->TransformCommutativeMUL();

			simple_script[j].abstractTree->TransformDivRule1(simple_script[j].abstractTree->root);
			simple_script[j].abstractTree->TransformDivRule2(simple_script[j].abstractTree->root);

			simple_script[j].abstractTree->TransformDivRule2(simple_script[j].abstractTree->root);
			
			simple_script[j].abstractTree->TransformDivRule3(simple_script[j].abstractTree->root);
			
		}

		
		cout << endl;
		cout << "printing simplified expression: " << endl;
		simple_script[j].abstractTree->PrintTree(simple_script[j].abstractTree->root);
		cout << endl;
		cout << endl << "-------------------------------------------" << endl;
		/*cout << endl << endl << "executing tree: = " << simple_script[j].abstractTree->Execute() << endl;
		//cout << endl;

		for (int i = 0; i < simple_script[j].abstractTree->m_container_object->terminals.size(); i++)
			cout << "Terminal " << i << " Symbol: " << simple_script[j].abstractTree->m_container_object->terminals[i]->sym << " Value:" << simple_script[j].abstractTree->m_container_object->terminals[i]->num << endl;
		*/

	}
	cout << endl << endl << endl << endl;

	delete[] simple_script;
	if( container )
		delete container;
	//SimpleScript *simple_script = new SimpleScript[x.size()];
	//ASTContainer *container = new ASTContainer();
	
/*	string s;
	bool loop_done = false;

	while (!loop_done)
	{
		char c;
		cin >> c;
		if (c == 'q'){ loop_done = true; break; }
		else s += c;
	}
	SimpleScript simple_script1;
	simple_script1.strInfixExpression = s;
	simple_script1.arrayInfixExpression = simple_script1.strSplit(simple_script1.strInfixExpression);

	//	cout << "s3: " << s3 << endl;
	simple_script1.parse();
	vector<ASTNode*> node_stack2 = simple_script1.postfixEvaluateListTree(simple_script1.arrayPostfixExpression);
	simple_script1.abstractTree->root = (OperatorNode*)node_stack2[0];
  
	cout << endl << endl << "m=1, n=2 mm = 10 value = " << simple_script1.abstractTree->Execute();
	cout << endl;

	for (int i = 0; i < simple_script1.abstractTree->terminals.size(); i++)
		cout << "Terminal " << i << " Symbol: " << simple_script1.abstractTree->terminals[i]->sym << " Value:" << simple_script1.abstractTree->terminals[i]->num << endl;


	cout << endl << endl << endl << endl;*/
	/*=================================================================
	test of the now redundant binary tree 
	=================================================================*/





	return 0;
}

