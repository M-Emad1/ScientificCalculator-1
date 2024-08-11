#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <sstream>
#include <stack>
using namespace std;
//void print_q(queue<int> q)
//{
//    while (!q.empty())
//    {
//        cout << q.front() << " ";
//        q.pop();
//    }
//    cout << endl;
// }
//method to check if the current char is digit or not
static bool isDigit(char item)  
{
    if (item >= '0' && item <= '9')
        return true;
    return false;
}
static int ReadInt(int start , int end)
{
    cout << "enter number in range " << start << " - " << end << ": ";
    int choice;
    cin >> choice;
    cout << "\n\n";
    if (choice < start || choice > end)
    {
        cout << "\nInvalid selection , try again\n";
       return ReadInt(start, end);
    }
    return choice;
}
static int show_menu(vector<string> choices)
{
    cout << "Operations:\n";
    for (int i = 0; i < choices.size(); i++)
    {
        cout << "\t" << i + 1 << ": " << choices[i] << "\n";
    }
    return ReadInt(1, choices.size());
}
static int ToInt(string str) {
    istringstream iss(str);
    int num;
    iss >> num;

    return num;
}
//method to remove spaces of a string
static string remove_spaces(const string & str_spaces)
{
    string str_no_spaces = ""; //idk why i initialized it , it already has a constructor!
    for (auto& ch : str_spaces)
    {
        if (ch == ' ')
            continue;
        str_no_spaces += ch;
    }
    return str_no_spaces;
}
//method to check if the char is inside this vector
static bool is_inside(char ch, const vector<char> &chars)
{
    if (find(chars.begin(), chars.end(), ch) != chars.end())
        return true;
    return false;
}

class OP
{
protected:
    virtual string calcOP(string exp) = 0;
    int get_first_operand(string exp)
    {
        string first_operand = "";
        int first_operand_int = 0;
        for (int i = 0; i < exp.size(); i++)
        {
            if (!isDigit(exp[i]))
                break;
            first_operand += exp[i];
        }
        first_operand_int = ToInt(first_operand);
        return first_operand_int;

    }
    int get_second_operand(string exp)
    {
        string operators = "+-*/%"; //hard coded for sorry , maybe i will update it later :)
        int second_operand_int = 0;
        // Find the position of any operator in the expression
        size_t operatorPos = exp.find_first_of(operators);

        if (operatorPos != std::string::npos) {
            // Extract the second operand
            string secondOperand = exp.substr(operatorPos + 1);
            second_operand_int = ToInt(secondOperand);
        }
        return second_operand_int;
    }
public:
     string operator() (string exp)
    {
        return calcOP(exp);
    }
};
class AddOP : public OP
{
    virtual string calcOP(string exp)
    {
        ostringstream oss;
        int first_operand = get_first_operand(exp);
        int second_operand = get_second_operand(exp);
        oss << (first_operand + second_operand);
        return oss.str();
    }
};
class SubOP : public OP
{
    virtual string calcOP(string exp)
    {
        ostringstream oss;
        int first_operand = get_first_operand(exp);
        int second_operand = get_second_operand(exp);
        oss << (first_operand +-second_operand);
        return oss.str();
    }
};

class MultiplyOP : public OP
{
    virtual string calcOP(string exp)
    {
        ostringstream oss;
        int first_operand = get_first_operand(exp);
        int second_operand = get_second_operand(exp);
        oss << (first_operand * second_operand);
        return oss.str();
    }
};

class DivideOP : public OP
{
    virtual string calcOP(string exp)
    {
        ostringstream oss;
        int first_operand = get_first_operand(exp);
        int second_operand = get_second_operand(exp);
        oss << (first_operand / second_operand);
        return oss.str();
    }
};

class RemainderOP : public OP
{
    virtual string calcOP(string exp)
    {
        ostringstream oss;
        int first_operand = get_first_operand(exp);
        int second_operand = get_second_operand(exp);
        oss << (first_operand % second_operand);
        return oss.str();
    }
};

class FactoryOp
{
    //OP* operation;
public:
    static OP* GetOP(char op_c)
    {
        if (op_c == '+')
            return new AddOP();
        else if (op_c == '-')
            return new SubOP();
        else if (op_c == '*')
            return new MultiplyOP();
        else if (op_c == '/')
            return new DivideOP();
        else if (op_c == '%')
            return new RemainderOP();
    }
};
class Calculator
{
private:
        vector<vector<char>> operator_check_order;
        string expression;
public:
    Calculator()
    {
        operator_check_order.push_back({ '*' , '/' , '%' });
        operator_check_order.push_back({ '+' , '-' });
    }
    void ReadExpression(bool has_prev_ans = false , string current_exp = "")
    {
        ostringstream oss;
        oss << "Enter expression to evaluate: ";
        if (has_prev_ans)
            oss << "Ans ";
        cout << oss.str();
         getline(cin, expression);
         getline(cin, expression);
         expression = current_exp + expression;
        expression = remove_spaces(expression);
    }
    string CalculateNoParenthesisExpression( string expression)
    {
       // ReadExpression();
        //this calculates expression whatever it is

           //string expression;
           //no-space expression
        for (auto& op_set : operator_check_order) //all this will be implemented for every op set
        {
            for (auto it = expression.begin(); it != expression.end(); it++) // iterates on each character of the expression
            {
                if (is_inside(*it, op_set)) //checks if the current operator is in the operator set
                {
                    string simple_expression = "";
                    auto  it_temp = it;
                    //adds operand before the operator
                    while (it_temp != expression.begin() && isDigit(*(--it_temp)))
                    {
                        simple_expression = *(it_temp)+simple_expression;
                    }
                    it_temp = it;
                    simple_expression += *it_temp; //adds the operator itself

                    //adds the operand after the operator
                    while ((it_temp + 1) != expression.end() && isDigit(*(++it_temp)))
                    {
                        simple_expression += *(it_temp);
                    }
                    OP* current_op = FactoryOp::GetOP(*it);
                    size_t pos = expression.find(simple_expression);
                    if (pos != std::string::npos) {
                        // Erase the simple expression from the string
                        expression.erase(pos, simple_expression.length());
                        expression.insert(pos, current_op->operator()(simple_expression)); //insert result of the simple xpression
                        it = expression.begin();
                    }
                }
            }

        }

        return expression;
    }
    string CalculateFullExpression(string full_exp)
    {
        //here the logic where i handle parenthesis and the normal operations are handled in the no parenthesis one
    
        stack<int> open_parenthesis_positions;
        for (int i = 0; i < full_exp.size(); i++)
        {
            if (full_exp.find('(') == string::npos && full_exp.find(')') == string::npos)
            {
                full_exp = CalculateNoParenthesisExpression(full_exp);
                cout << "Result: " << full_exp << "\n";
                return full_exp;
            }
            if (full_exp[i] == '(')
                open_parenthesis_positions.push(i);
            else if (full_exp[i] == ')')
            {
                string result;
                string substr = full_exp.substr(open_parenthesis_positions.top() + 1, i - (open_parenthesis_positions.top() + 1));
                result = CalculateNoParenthesisExpression(substr);
                full_exp.erase(open_parenthesis_positions.top(), i + 1 - open_parenthesis_positions.top());
                full_exp.insert(open_parenthesis_positions.top() , result);
                i = open_parenthesis_positions.top() + 1;
                open_parenthesis_positions.pop();
              //  cout << full_exp << "\n";
            }
        }
        cout << "Result: " << full_exp << "\n";
        return full_exp;
    }
    void RunCalculator()
    {
        string current_expression = "-1";
        vector<string> calc_menu{ "AC" , "Continue" };
         /*   ReadExpression();
            current_expression = CalculateFullExpression(expression);*///for the first time (basmaga)
        while (true)
        {
            int choice = show_menu(calc_menu);
            if (choice == 1)
            {
                ReadExpression();
                current_expression =  CalculateFullExpression(expression);
            }
            else if (choice == 2)
            {
                if (current_expression == "-1")
                    continue;
                ReadExpression(true, current_expression);
                current_expression = CalculateFullExpression(expression);
            }
        }
        
    }
};

int main()
{
    Calculator calculator;
    calculator.RunCalculator();
}

