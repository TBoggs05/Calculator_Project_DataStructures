#include "calculator.h"
/*
TODO:
TEST README RUN INSTRUCTIONS ON ADA
REMEMBER TO POST OUTPUT TOO!
*/

#include<stack>
//Constructor
calculator::calculator() {
    infix = postfix = "";
    result = 0.0;
}
//Start function.
void calculator::start_process()
{
    //declaration of calculator object and condition for loop functionality.
    char condition[2];
    //print title
    std::cout << "CALCULATOR PROJECT" << std::endl;
    std::cout << "------------------" << std::endl;
    //loop until user terminates
    while (true) {
        //ask for input until valid input is given
        do {
            //set infix to blank, grab user's infix
            infix = "";
            std::cout << "Enter an infix-equation: (\"EXIT\" TO CANCEL)" << std::endl;
            getline(std::cin, infix);
            if (infix == "EXIT") {
                std::cout << "EXITING PROGRAM..." << std::endl;
                return;
            }
        //check if valid input(parentheses check, # of operands is 1 greater than operators and no invalid operators or operands (^, sin(), etc))
        } while (!check_matching() || !check_valid());

        //convert infix_to_postifx (if case is an extra safeguard)
        if (infix_to_postfix()) {
            //evaluate postfix
            eval_postfix();
            //print result
            printResult();
        }
        std::cout << "Go again?(y/n)" << std::endl;
        std::cin >> condition;
        //function to deal with getline/cin conflict.
        std::cin.ignore();
        //end condition
        if (!(tolower(condition[0]) == 'y'))
            break;
    }

}
//Check the priority of operators for the conversion
int calculator::op_priority(char o) {
    //- and + are lower priority than * or /
    if (o == '-' || o == '+')
        return 1;
    else if (o == '*' || o == '/') {
        return 2;
    }
    else{
        //error case:
        return -1;
    }
}
//Check that there is one more operand than operator. Has a built in check for invalid character and operators
bool calculator::check_valid() {
    //if a number is found, push it onto the stack 
    std::stack<char> s;
    for (int i = 0; i < infix.length();i++) {
        //check valid character
        if (infix[i] == ' ' || infix[i] == '(' || infix[i] == ')' || isdigit(infix[i]) || infix[i] == '+' || infix[i] == '-' || infix[i] == '/' || infix[i] == '*') {

            //if digit, push first number onto stack (we can ignore rest of number since it does not matter.) IF: stack is empty, or an operator is before value.
            if (isdigit(infix[i])) {
                if (s.empty() || !isdigit(s.top())) {
                    s.push(infix[i]);
                    while (isdigit(infix[i])) { i++; }
                }
                else {
                    std::cout << "\nTOO MANY OPERANDS OR INCORRECT FORMATTING. ERROR.\n" << std::endl;
                    return false;
                }
            }
            //if operator, push if stack is NOT empty AND if previous is digit.
            if (infix[i] == '+' || infix[i] == '-' || infix[i] == '/' || infix[i] == '*') {
                if (!s.empty() && isdigit(s.top())) {
                    s.push(infix[i]);
                }
                else {
                    std::cout << "\nTOO MANY OPERATORS OR INCORRECT FORMATTING. ERROR.\n" << std::endl;
                    return false;
                }
            }
        }
        else {
            std::cout << "\nINVALID CHARACTER. ERROR\n" << std::endl;
            return false;
        }
    }
    return true;
}
//Checks that parenthesis are balanced
bool calculator::check_matching() {
    std::stack<char> pStack;
    //push opens onto stack, then pop when ) is found. if stack is empty by end, the equation is fine. otherwise, there is a problem.
    for (int i = 0; i < infix.length(); i++) {
        if (infix[i] == '(')
            pStack.push(infix[i]);
        else if (infix[i] == ')') {
            if (pStack.empty()) {
                std::cout << "\nPARENTHESIS UNBALANCED!\n" << std::endl;
                return false;
            }
            else { pStack.pop(); }
        }
       
    }
    //if the stack is empty, its balanced!
    if (pStack.empty()) {
        return true;
    }
    else {
        std::cout << "\nPARENTHESIS UNBALANCED!\n" << std::endl;
        return false;
    }
}
//converts the given infix equation to postfix. Has a built in check for invalid character and operators(extra safeguard)
bool calculator::infix_to_postfix() {
    std::stack<char> s;
    int i = 0;
    postfix = "";
        //loop through the infix
    for (int i = 0; i < infix.length();) {
        char ch = infix[i];
        switch (ch) {
        //if whitespace, just ignore.
        case ' ':
            i++;
            break;
        //if character is a digit, append
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            postfix += ch;
            i++;
            //account for multi digit values, loop until space or until not digit.
            while (isdigit(infix[i])) { postfix += infix[i++]; }
            postfix += " "; //append a space after
            break;
        //open group operator, push to stack.
        case '(':s.push(ch); i++; break;
        //closed group operator, append and pop until we reach open group operator, then discard it.
        case ')':
            while (s.top() != '(') {
                postfix += s.top();
                postfix += " ";
                s.pop();
            }
            s.pop();
            i++;
            break;
            //operator: check precedence and push/pop accordingly.
        case '+':
        case '-':
        case '*':
        case '/':
            //safeguards to stop popping an ( operator, or from popping from an empty stack. If incoming operator priority is <= operator at top of stack,
            //append/pop until condition changes.
           while (!s.empty() && (op_priority(ch) <= op_priority(s.top())) && s.top() != '(') {
                postfix += s.top();
                postfix += " ";
                s.pop();
            }
            s.push(ch);
            i++;
            break;
        //INVALID CHARACTER. THROW ERROR.
        default:
        std::cout << "\nINVALID CHARACTER. ERROR\n" << std::endl;
        return false;
        break;

        }

    }
    //append/pop remaining operators.
    while (!s.empty()) {
        postfix += s.top();
        postfix += " ";
        s.pop();
    }
    //std::cout <<"postfix: " << postfix << std::endl;
    return true;

}
//Evaluate the equation (calculate answer)
void calculator::eval_postfix() {
    //initialize variables
    double op1, op2 = 0.0;
    int temp = 0;
    char ch;
    result = 0;
    std::stack<double> resultStack;
    //loop through the postfix equation.
    for (int i = 0; i < postfix.length(); i++) {
          temp = 0;
          ch = postfix[i];
        
            //if current ch is whitespace, do nothing.
        if (ch == ' ') {
            
        }
       //if current ch is an integer, account for multi digit values and push onto stack.
        else if (isdigit(postfix[i])) {
            while (isdigit(postfix[i])) {
                //takes advantage of ASCII logic
                temp = temp * 10 + (postfix[i] - '0');
                i++;
            }
            resultStack.push(temp);
        }
        //if operator, grab the first two digits from the stack, pop them, perform the correct operation then push the resulting digit back onto stack
        else {
            //first value
            if (!resultStack.empty()) {
                op1 = resultStack.top();
                resultStack.pop();
            }
            else {
                std::cout << "\nNO OPERANDS TO CALCULATE! ERROR.\n"<< std::endl;
            }
            //second value
            if (!resultStack.empty()) {
                op2 = resultStack.top();
                resultStack.pop();
            }
            else {
                std::cout << "\nONLY ONE OPERAND TO CALCULATE! ERROR.\n" << std::endl;
            }

            //now based on the operator, perform the operation and push result back onto stack.
            switch (ch) {
            case '+':resultStack.push(op2 + op1); break;
            case '-': resultStack.push(op2 - op1); break;
            case '*': resultStack.push(op2 * op1); break;
            case '/': //account for dividing by 0.
                if (op1 != 0)
                    resultStack.push(op2 / op1);
                else {
                    std::cout << "\nCANNOT DIVIDE BY 0. ERROR\n" << std::endl;
                    i = postfix.length();
                }
                break;
            }
        }
    }
    //Checks to make sure the result is accurate. If it is, store it as result. Otherwise, display error.
    if (!resultStack.empty())
    {
        result = resultStack.top();
        resultStack.pop();
    }
    if (!resultStack.empty())
        std::cout << "\nOOPS, the result is NOT correct.\n";

    std::cout << std::endl;
}
//Print answer to screen.
void calculator::printResult() {
    std::cout << "\nAnswer: " << result << std::endl;
}

