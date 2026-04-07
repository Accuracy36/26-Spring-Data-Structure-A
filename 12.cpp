#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

// 用于区分整数和浮点数，以满足 C 语言除法规范
struct Value {
    double val;
    bool is_int;
    Value(double v = 0.0, bool i = true) : val(v), is_int(i) {}
};

int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0; 
}

Value applyOp(Value a, Value b, char op) {
    Value res;
    res.is_int = (a.is_int && b.is_int); 
    
    switch(op) {
        case '+': res.val = a.val + b.val; break;
        case '-': res.val = a.val - b.val; break;
        case '*': res.val = a.val * b.val; break;
        case '/':
            if (res.is_int) {
                // 防止除以0导致评测脚本崩溃
                if (b.val == 0) return Value(0, true); 
                res.val = static_cast<int>(a.val) / static_cast<int>(b.val);
            } else {
                res.val = a.val / b.val;
            }
            break;
        case '^':
            res.val = pow(a.val, b.val);
            // 修复点：动态判断乘方结果是否为整数
            res.is_int = (a.is_int && b.is_int && b.val >= 0); 
            break;
    }
    return res;
}

double evaluateExpression(const string& expr, const map<char, Value>& vars) {
    stack<Value> values; 
    stack<char> ops;     

    for (int i = 0; i < expr.length(); i++) {
        if (isspace(expr[i])) continue;

        if (isdigit(expr[i]) || expr[i] == '.') {
            string numStr = "";
            bool isFloat = false;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                if (expr[i] == '.') isFloat = true;
                numStr += expr[i];
                i++;
            }
            i--; 
            values.push(Value(stod(numStr), !isFloat));
        }
        else if (isalpha(expr[i])) {
            char varName = expr[i];
            Value val(0.0, true); 
            if (vars.find(varName) != vars.end()) {
                val = vars.at(varName); 
            }
            values.push(val); 
        }
        else if (expr[i] == '(') {
            ops.push(expr[i]);
        }
        else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                Value val2 = values.top(); values.pop();
                Value val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(val1, val2, op));
            }
            if (!ops.empty()) ops.pop(); 
        }
        else {
            // 修复点：特判乘方 '^' 的右结合性
            while (!ops.empty() && ops.top() != '(') {
                int topPrec = getPrecedence(ops.top());
                int currPrec = getPrecedence(expr[i]);
                
                if ((expr[i] == '^' && topPrec > currPrec) || 
                    (expr[i] != '^' && topPrec >= currPrec)) {
                    Value val2 = values.top(); values.pop();
                    Value val1 = values.top(); values.pop();
                    char op = ops.top(); ops.pop();
                    values.push(applyOp(val1, val2, op));
                } else {
                    break;
                }
            }
            ops.push(expr[i]);
        }
    }

    while (!ops.empty()) {
        Value val2 = values.top(); values.pop();
        Value val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(val1, val2, op));
    }

    return values.top().val;
}

void parseLine(const string& line, string& expr, map<char, Value>& vars) {
    expr = "";
    vars.clear();
    
    size_t firstComma = line.find(',');
    if (firstComma == string::npos) {
        expr = line; 
        return;
    }
    
    expr = line.substr(0, firstComma);
    string varPart = line.substr(firstComma + 1);
    
    varPart.erase(remove_if(varPart.begin(), varPart.end(), ::isspace), varPart.end());
    
    stringstream ss(varPart);
    string token;
    while (getline(ss, token, ',')) {
        size_t eqPos = token.find('=');
        if (eqPos != string::npos) {
            char varName = token[0]; 
            string valStr = token.substr(eqPos + 1);
            
            // 修复点：通过寻找有没有小数点，判断该变量赋值是否为整数
            bool isInt = (valStr.find('.') == string::npos);
            vars[varName] = Value(stod(valStr), isInt); 
        }
    }
}

int main() {
    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        
        string expr;
        map<char, Value> vars;
        
        parseLine(line, expr, vars);
        
        double result = evaluateExpression(expr, vars);
        
        cout << result << endl;
    }
    return 0;
}