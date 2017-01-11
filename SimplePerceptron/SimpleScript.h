#ifndef SIMPLE_SCRIPT_H
#define SIMPLE_SCRIPT_H


#include "StringUtils.h"
using std::string;
using std::vector;


#include "AST.h"


class SimpleScript
{
public:
	SimpleScript()
	{
		abstractTree = new AST();
	}

	string strMyOperators = "also,and,or,ne,=,eq,lt,le,gt,ge,+,sin,-,cos,sqrt,/,tan,mod,*,^,%,(,),{,},do,while,then,if,ifthen,else,ifthenelse,whiledo,;";
	//string strMyOperators = "ne,=,eq,lt,le,gt,ge,+,sin,-,cos,/,tan,mod,*,^,%,(,)";
	string lookahead = "";
	string tokenval = "";

	int lexancalls = 0;
	bool thenFound = false;                 // flag for tracking the "then" statement
	bool bracket_opened = false;                  // flag to state whether a bracket is open
	int bracket_level = 0;                       // integer for tracking the depth of the bracket nesting
	bool curly_bracket_opened = false;
	int curly_bracket_level = 0;
	bool ifthen = false;                          // flag to state whether an ifthen    statement has been found

	bool syntax_error = false;

	//string strInfixExpression = "if ((m eq 1) and ((n eq 2) or (mm eq 10))) then ((x = sqrt(sin(y + 5.25))) and (t = 1));";
	//string strInfixExpression  = "if (m eq 1) then (x = sqrt(sin(y + 5.25))) else ( x = 5 );";
	//string strInfixExpression = "if ((a eq b) and (c eq d)) then while(a eq b) do(x = x+1) else (c = sin(5 + y) - sqrt( y ^ 4 );";
	//string strInfixExpression = "if(a eq b) then {x=x+1} else {if(c eq d) then x=x-1}; x =x*2 + cos(2*y ); c=d+a; w = 2*sin(s*z);";
	//string strInfixExpression = "while(a le b) do { x=x + 0.1 also a=x+a also c=v*v }; z=sqrt(x^2 + y^2);";
	string strInfixExpression = "sqrt( x^2 + y^2 )";
	vector<string> arrayInfixExpression;
	vector<string> arrayPostfixExpression;

	VariableSizedMultiList< string, string > arraySymbolTable;

	void toggleBrackets(){
		if (lookahead == "("){         // check for an opening bracket
			bracket_opened = true;   // toggle the flag
			bracket_level++;         // increase the bracket depth 
		}
		else if (lookahead == ")"){
			bracket_opened = false;  // close the flag
			bracket_level--;         // decrease the depth
		}
		else if (lookahead == "{"){
			curly_bracket_opened = true;
			curly_bracket_level++;
		}
		else if (lookahead == "}"){
			curly_bracket_opened = false;
			curly_bracket_level--;
		}
	}

	void match(string t){
		toggleBrackets();
		if (lookahead == t) {
			if ((lookahead == "=") && bracket_opened == true){
				if (thenFound == true && bracket_level > 1){
					std::cout << t << " Syntax Error Match! " << lookahead << " at position " << lexancalls << endl;
					syntax_error = true;
				}
			}
			if ((lookahead == "=") && curly_bracket_opened == true){
				if (thenFound == true && curly_bracket_level > 1){
					std::cout << t << " Syntax Error Match! " << lookahead << " at position " << lexancalls << endl;
					syntax_error = true;
				}
			}
			lookahead = lexan();
			if (bracket_opened == false && thenFound == true){
				if (lookahead == "else") ifthen = false;
				else ifthen = true;
			}
			if (curly_bracket_opened == false && thenFound == true){
				if (lookahead == "else") ifthen = false;
				else ifthen = true;
			}
		}
		else std::cout << t << " Syntax Error Match!" << endl;
	}

	void emit(string t, string tval = ""){
		if ((t == "+") || (t == "-") || (t == "*") || (t == "/")) arrayPostfixExpression.push_back(t);
		else if ((t == "^") || (t == "lt") || (t == "le") || (t == "gt")) arrayPostfixExpression.push_back(t);
		else if ((t == "ge") || (t == "eq") || (t == "=")) arrayPostfixExpression.push_back(t);
		else if ((t == "sin") || (t == "cos") || (t == "sqrt") || (t == "mod")) arrayPostfixExpression.push_back(t);
		else if ((t == "ne") || (t == "mod")) arrayPostfixExpression.push_back(t);
		else if ((t == "ifthen") || (t == "whiledo") || (t == "ifthenelse") || (t == "dowhile")){
			thenFound = false;
			ifthen = false;
			arrayPostfixExpression.push_back(t);
		}
		else if (t == "NUMBER") arrayPostfixExpression.push_back(tval);
		else if ((t == "and") || (t == "or")) arrayPostfixExpression.push_back(t);
		else if (t == ";") 
			arrayPostfixExpression.push_back("also");
		else if ((t == "also")) arrayPostfixExpression.push_back("also");
		else if (t == "ID") arrayPostfixExpression.push_back(arraySymbolTable.m_storage[atoi(tval.c_str())].key);
		else if (t == ";") std::cout << "Do nothing" << endl;
		else std::cout << "Do nothing" << endl;
	}

	// this function checks to see if the input symbol is an operator
	bool isThisMyOperator(string strSymbolAtHand) {
		vector<string> arrayOperator = string_to_vector(strMyOperators);// .split(/ \, / )

		for (int i = 0; i < arrayOperator.size(); i++) {
			if (arrayOperator[i] == strSymbolAtHand) {
				return true;
			}
		}
		return false;
	}

	void factor(){
		if (lookahead == "(") { match("("); expr_list(); match(")"); }
		else if (lookahead == "{") { match("{"); expr_list(); match("}"); }
		else if (lookahead == "NUMBER") {
			emit("NUMBER", tokenval); match("NUMBER"); 
		}
		else if (lookahead == "ID") { emit("ID", tokenval); match("ID"); }
		else if (lookahead == "sin") { match("sin"); factor(); emit("sin"); }
		else if (lookahead == "cos") { match("cos"); factor(); emit("cos"); }
		else if (lookahead == "tan") { match("tan"); factor(); emit("tan"); }
		else if (lookahead == "sqrt") { match("sqrt"); factor(); emit("sqrt"); }
		else if (lookahead == "if") {
			match("if"); match("("); expr_list(); match(")");
			match("then"); thenFound = true; match("{"); expr_list(); match("}");
			if (ifthen == true) { emit("ifthen"); return; }
			match("else"); match("{"); expr_list(); match("}");
			emit("ifthenelse");      // added code here
		}
		else if (lookahead == "while") { 
			match("while"); match("("); expr_list(); match(")");
			match("do"); match("{"); expr_list_statements(); match(";"); match("}");
			emit("whiledo");
		}
		else if (lookahead == "ifthenelse") std::cout << "???" << endl;
		else if (lookahead == ";") { match(";"); emit(";"); cout << "problem at: " << lookahead << " with lexancalls: " << lexancalls << endl; }
		//	match(";"); emit(";"); std::cout << " ? ? ? " << endl;
		//}
		else if (lookahead == "}")return;
		else std::cout << "Syntax Error Factor" << endl;
	}

	void termEndStatement(){
		string t;
		factor();
		while ((lookahead == ";")){
			t = lookahead;
			match(lookahead);
			//factor()
			expr();
			emit(t, "NONE");
		}
	}

	void termAssignmentOrRelational(){
		string t;
		factor();
		while ((lookahead == "=") || (lookahead == "eq") || (lookahead == "le") || (lookahead == "lt") ||
			(lookahead == "gt") || (lookahead == "ge") || (lookahead == "mod") || (lookahead == "ne")){
			t = lookahead;
			match(lookahead);
			//factor()
			expr();
			emit(t, "NONE");
		}
	}

	void term_exponent(){
		string t;
		termAssignmentOrRelational();
		while ((lookahead == "^")){
			t = lookahead;
			match(lookahead);
			//factor()
			termAssignmentOrRelational();
			emit(t, "NONE");
		}
	}

	void term(){
		string t;
		term_exponent();
		while ((lookahead == "*") || (lookahead == "/")){
			t = lookahead;
			match(lookahead);
			//factor()
			term_exponent();
			emit(t, "NONE");
		}
	}

	void expr(){
		string t;
		term();
		while ((lookahead == "+") || (lookahead == "-")){
			t = lookahead;
			match(lookahead);
			
			term();
			emit(t, "NONE");
		}
	}

	void expr_list(){
		string t;
		expr_list_and();
		while (lookahead == "or"){
			t = lookahead;
			match(lookahead);
			expr_list_and();
			emit(t);
		}
	}

	// This function is evaluated first because ( a and b or c ) is
	// equivalent to just saying c according to the laws of logic
	void expr_list_and(){
		string t;
		expr();
		while (lookahead == "and" ){
			t = lookahead;
			match(lookahead);
			expr();
			emit(t);
		}
	}

	void expr_list_statements(){
		string t;
		expr();
		while (lookahead == "also"){
			t = lookahead;
			match(lookahead);
			
			expr();
			emit(t);
		}
	}

	/*
	*/

	// push the items in the stack implemeneted as 2 dimensional array, then return the postion
	string insert(string Lexeme, string Token){
		return to_string(arraySymbolTable.Insert(Lexeme, Token));
	}

	// look for the lexeme in the symbol table, if in there, return the position, not return -1
	string lookup(string Lexeme){
		return to_string(arraySymbolTable.FindKey(Lexeme));
	}

	string lexan()
	{
		lexancalls++;
		// shift is like pop a thing out of a stack
		string t = "";
		if (arrayInfixExpression.size() >= 1) {
			t = *arrayInfixExpression.begin();
			arrayInfixExpression.erase(arrayInfixExpression.begin());
		}
		else return "DONE";
		// t is a number
		//if (t == ";") { return "DONE"; }
		if (string_to_number(t)) { tokenval = t; return "NUMBER"; }
		// t is a symbol (identifier), not an operator
		// when the lexeme is an ID, the lexeme can be already in the symbol table
		// or not in there.  If in there, get the position, if not, push it in and get the
		// position
		else if (!string_to_number(t) && !isThisMyOperator(t)) {
			string p = lookup(t);
			if (p == "-1") { p = insert(t, "ID"); }
			tokenval = p;
			return "ID";
		}
		// must be operator
		else {
			tokenval = "NONE";
			return t;
		}
	}



	void parse(){
		lookahead = lexan();
		while (!(lookahead == "DONE")){
			expr();
			//match(";")
		}
	}


	vector<string> strSplit(string strInput)
	{
		vector<string> arrayT0;
		vector<string> arrayT1;
		vector<string> arrayT2;

		vector<string> arrayT3;// = vsplit(strInput, std::regex("/\b/"));
		string delim = strMyOperators + ", ";

		Appender v;
		tokenize4(v, strInput.c_str(), " ;*/%^!-+=><\r\n\n\t {}()");
		arrayT3 = v;

		for (int i = 0; i < arrayT3.size(); i++)
		{
			string arrayT4 = arrayT3[i];// .toString()
			while (indexOf(" ", arrayT4) >= 0)
			{
				arrayT4 = ReplaceAll(arrayT4, " ", "");
			}
			arrayT3[i] = arrayT4;

			if (arrayT3[i].size() > 0)
				arrayT2.push_back(arrayT3[i]);

		}

		arrayT3 = arrayT2;

		for (int i = 0; i < arrayT3.size(); i++){

			if (arrayT3[i].size() > 1){
				// \s* tell the split to break the string different way.  Observe the result by using alert function whenever you like
				string temp1 = separate_commas(arrayT3[i]);// .split(/ \s* / )
				if ((indexOf("(", arrayT3[i]) >= 0) || (indexOf(")", arrayT3[i]) >= 0) ||
					(indexOf(";", arrayT3[i]) >= 0)){

					arrayT3[i] = temp1;
				}
			}
		}

		for (int i = 0; i < arrayT3.size(); i++){
			// when I see a period, I assume that there would be a number on the left and a number on the right.
			if (arrayT3[i] == "."){
				arrayT0.pop_back();
				arrayT1[i] = "";
				if ((i - 1) >= 0){
					if (string_to_number(arrayT3[i - 1])){
						arrayT1[i] = (arrayT3[i - 1]);
						arrayT1[i] += ".";
						arrayT3[i] = "";
						arrayT3[i - 1] = "";
					}
					else {
						//alert("Numerical Syntax Error (should be a number) at" + arrayT3[i - 1]); 
						break;
					}
				}

				if ((i + 1) <= arrayT3.size()){
					if (string_to_number(arrayT3[i + 1])){
						arrayT1[i] = (arrayT3[i + 1]);
						arrayT3[i + 1] = "";
					}
					else {
						//alert("Numerical Syntax Error (should be a number) at " + arrayT3[i + 1]); 
						break;
					}
				}

				string arrayT4 = arrayT1[i];
				arrayT4 = ReplaceAll(arrayT4, ",", "");
				arrayT3[i] = arrayT4;

			}
			if (!(arrayT3[i].size() == 0)) arrayT0.push_back(arrayT3[i]);
		}

		return arrayT0;
	}

	vector<string> strEvalStack;
	vector<string> strEvalStackCpy;

	void CopyEvalStack()
	{
		for (int i = 0; i < strEvalStack.size(); i++)
		{
			strEvalStackCpy.push_back(strEvalStack[i]);
		}
	}

	string strEvalStackCpy_pop()
	{
		string top = strEvalStackCpy[strEvalStackCpy.size() - 1];
		strEvalStackCpy.pop_back();
		return top;
	}

	string strEvalStack_pop()
	{
		if (strEvalStack.size() == 0) return "";

		string top = strEvalStack[strEvalStack.size() - 1];
		strEvalStack.pop_back();
		return top;
	}

	string postfixEvaluate_1(string strOperator){
		if (strOperator == "+"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "add(" + strLeftOperand + ", " + strRightOperand + ")";
		}

		else if (strOperator == "-"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "subtract(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "*"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "multiply(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "/"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "divide(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "^"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "power(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "="){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "assign(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "eq"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "equal(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "gt"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "greaterThan(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "ge"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "greaterThanOrEqual(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "lt"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "lessThan(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "le"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "lessThanOrEqual(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "ne"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "notEqual(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "mod"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "remainder(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "sin"){
			string strRightOperand = strEvalStack_pop();
			return "sin(" + strRightOperand + ")";
		}
		else if (strOperator == "cos"){
			string strRightOperand = strEvalStack_pop();
			return "cos(" + strRightOperand + ")";
		}
		else if (strOperator == "tan"){
			string strRightOperand = strEvalStack_pop();
			return "tan(" + strRightOperand + ")";
		}
		else if (strOperator == "sqrt"){
			string strRightOperand = strEvalStack_pop();
			return "sqrt(" + strRightOperand + ")";
		}
		else if (strOperator == "ifthenelse"){                        // added code here
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			string strLeft2Operand = strEvalStack_pop();
			return "ifThenElse(" + strLeft2Operand + ", " + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "ifthen"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();

			return "ifThen(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "whiledo"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "whileDo(" + strLeftOperand + ", " + strRightOperand + ")";
		}

		else if (strOperator == "and"){
			string strRightOperand = strEvalStack_pop();
			string strLeftOperand = strEvalStack_pop();
			return "andConnective(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "or"){
			string strRightOperand = strEvalStack_pop();
			string  strLeftOperand = strEvalStack_pop();
			return "orConnective(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else if (strOperator == "also"){
			string strRightOperand = strEvalStack_pop();
			string  strLeftOperand = strEvalStack_pop();
			return "stackConnective(" + strLeftOperand + ", " + strRightOperand + ")";
		}
		else{
			cout << "Illegal Operation " << endl;
			strEvalStack.clear();
			return "";
		}

		return "";
	}

	SyntaxNode* postfixEvaluate_Tree(string strOperator){
		if (strOperator == "+"){

			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new ADDNode();
			n->right = n_right;
			n->left = n_left;

			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}

		else if (strOperator == "-"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new SUBNode();
			n->right = n_right;
			n->left = n_left;

			if ( n_right )
				n_right->parent = n;
			
			if ( n_left )
				n_left->parent = n;

			return n;

		}
		else if (strOperator == "*"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new MULNode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}
		else if (strOperator == "/"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new DIVNode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;

		}
		else if (strOperator == "^"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new POWNode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;

		}
		else if (strOperator == "="){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new EQ1Node();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}
		else if (strOperator == "eq"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new EQNode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}
		else if (strOperator == "gt"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new GTNode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}
		else if (strOperator == "ge"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new GENode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}
		else if (strOperator == "lt"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new LTNode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}
		else if (strOperator == "le"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new LENode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}
		else if (strOperator == "ne"){

		}
		else if (strOperator == "mod"){

		}
		else if (strOperator == "sin"){
			ASTNode* n_right = node_stack_pop();

			OperatorNode *n = new SINNode();
			n->right = n_right;
			if (n_right)
				n_right->parent = n;

			return n;
		}
		else if (strOperator == "cos"){
			ASTNode* n_right = node_stack_pop();

			OperatorNode *n = new COSNode();
			n->right = n_right;
			if (n_right)
				n_right->parent = n;


			//n->left = n_left;
			return n;
		}
		else if (strOperator == "tan"){
			ASTNode* n_right = node_stack_pop();

			OperatorNode *n = new TANNode();
			n->right = n_right;
			if (n_right)
				n_right->parent = n;


	
			//n->left = n_left;
			return n;
		}
		else if (strOperator == "sqrt"){
			ASTNode* n_right = node_stack_pop();

			OperatorNode *n = new SQRTNode();
			n->right = n_right;
			if (n_right)
				n_right->parent = n;

			return n;

		}
		else if (strOperator == "ifthenelse"){                        // added code here

		}
		else if (strOperator == "ifthen"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			FlowControlNode *n = new IFTHENNode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}
		else if (strOperator == "whiledo"){

		}

		else if (strOperator == "and"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new ANDNode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}
		else if (strOperator == "or"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			OperatorNode *n = new ORNode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}
		else if (strOperator == "also"){
			ASTNode* n_right = node_stack_pop();
			ASTNode* n_left = node_stack_pop();
			FlowControlNode *n = new ALSONode();
			n->right = n_right;
			n->left = n_left;
			if (n_right)
				n_right->parent = n;

			if (n_left)
				n_left->parent = n;
			return n;
		}
		else{

			return 0;
		}

		return 0;
	}

	vector< string > postfixEvaluateList(vector<string> arrayInput)
	{
		for (int i = 0; i < arrayInput.size(); i++){
			if (!(isThisMyOperator(arrayInput[i]))){
				strEvalStack.push_back(arrayInput[i]);
			}
			else{
				string strIntermediate = postfixEvaluate_1(arrayInput[i]);
				strEvalStack.push_back(strIntermediate);
			}
		}
		return strEvalStack;
	}

	vector<ASTNode*> node_stack;

	ASTNode* node_stack_pop()
	{
		if (node_stack.size() == 0) return NULL;
		ASTNode* n = node_stack[node_stack.size() - 1];
		node_stack.pop_back();
		return n;
	}

	vector<ASTNode*> postfixEvaluateListTree(vector<string> arrayInput)
	{
		

		for (int i = 0; i < arrayInput.size(); i++){
			if (!(isThisMyOperator(arrayInput[i]))){

				bool bnumber = isNumberC(arrayInput[i]);
				TerminalNode *t = this->abstractTree->AddTerminal(arrayInput[i], bnumber);
				node_stack.push_back(t);
			}
			else{
				SyntaxNode* n = this->postfixEvaluate_Tree(arrayInput[i]);
				node_stack.push_back(n);

			}
		}
		return node_stack;
	}

	AST *abstractTree=0;

	AST* WalkStack(vector<string> arrayInput)
	{
		if (abstractTree){
			delete abstractTree;
			abstractTree = NULL;
		}

		abstractTree = new AST();

		abstractTree->root = 0;
		ASTNode *temp = abstractTree->root;


	}

	AST* WalkStackRec()
	{
	/*	if (!abstractTree->root){
			CreateOperator(abstractTree->root, arrayInput[i]);
		}
		else
		{
			if (temp->left){
				CreateOperator(temp->right, arrayInput[i]);
				temp->right->parent = temp;
				if (temp->left->type == 0)
				{
					temp = temp->left;
				}
				else temp = temp->right;
			}
			else {
				CreateOperator(temp->left, arrayInput[i]);
				temp->right->parent = temp;
			}

		}*/
	}

	string postfixEvaluate(vector<string> arrayInput){
		

		for (int i = 0; i < arrayInput.size(); i++){
			if (!(isThisMyOperator(arrayInput[i]))){
				// add terminal node
				bool isNum = isNumberC(arrayInput[i]);

	
				
				strEvalStack.push_back(arrayInput[i]);
			}
			else{
				// add operator node

				string strIntermediate = postfixEvaluate_1(arrayInput[i]);
				strEvalStack.push_back(strIntermediate);
			}
		}
		return strEvalStack_pop();
	}

	void CreateOperator(ASTNode* n, string r)
	{
		if ( r == "+")
		{
			n = new ADDNode();
		}
		else if (r=="-")
		{
			n = new SUBNode();
		}
		else if (r == "*")
		{
			n = new MULNode();
		}
		else if (r == "/")
		{
			n = new DIVNode();
		}
		else if (r == "^")
		{
			n = new POWNode();
		}
		else if (r == "sqrt")
		{
			n = new SQRTNode();
		}
		else if (r == "sin")
		{
			//abstractTree->root = new SINNo();
		}
		else if (r == "cos")
		{

		}
		else if (r == "tan")
		{

		}
		else if (r == "mod")
		{

		}
		else if (r == "log")
		{

		}
		else if (r == "ln")
		{

		}
		else if (r == "exp")
		{

		}
	}
};


class SimpleVM
{
public:

	enum variable_type
	{
		BOOL,
		CHAR,
		INT16,
		INT32,
		INT64,
		FLOAT,
		DOUBLE,
		
	};

	class VariableNode
	{
	public:

		variable_type type;
		string data;
	};

/*	auto GetVariableType(VariableNode *n)
	{
		switch (n->type)
		{
		case BOOL: return (bool)atoi(n->data.c_str());
		case CHAR: return n->data.c_str();
		case INT16: return atoi(n->data.c_str());
		case INT32: return atoi(n->data.c_str());
		case INT64: return atoi(n->data.c_str());
		case FLOAT: return atof(n->data.c_str());
		case DOUBLE: return (double)atof(n->data.c_str());
		default:
			return 0;
		}
		return 0;
	}*/

 
	class Op
	{
	public:

		

		Op* prev;
		Op* next;
	};
};
#endif