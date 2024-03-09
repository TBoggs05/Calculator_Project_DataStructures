#ifndef CALCULATOR_H_
#define CALCULATOR_H_
#include <string>
#include <iostream>

class calculator {
	//starting infix variable, postfix is returned after evaluation function, and result is printed to screen
private:
	std::string infix;//starting equation
	std::string postfix;//converted equation
	double result; //final calculation
	
	//Private Helper Functions
	bool check_valid();//checks two things: 1) # of operands is 1 greater than operators. 2)no invalid operators or operands are used: (A, ^, sin, etc)
	bool check_matching();//checks that parenthesis are balanced
	bool infix_to_postfix();//converts given infix to postfix format for operations to be performed (uses stack)
	void eval_postfix();//uses stacks to calculate answer 
	void printResult();//outputs result to screen if stack is empty(operation was completed succesfully)
	int op_priority(char o);//checks priority of operator(used for conversion to postfix)

public:
	calculator();//constructor
	void start_process();//starts the calculator logic.
	
};
#endif
