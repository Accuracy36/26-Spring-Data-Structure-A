#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// 定义正则表达式的 Token 结构
struct Token {
    char c;
    bool is_d;     // 是否是 \d
    bool is_plus;  // 是否带有 +
};

// 检查单个字符是否匹配当前 Token
bool match_char(char text_c, const Token& t) {
    if (t.is_d) return isdigit(text_c);
    return text_c == t.c;
}

// 回溯法匹配核心逻辑
int matchTokens(const string& text, int t_idx, const vector<Token>& tokens, int token_idx) {
    // 如果所有 token 都匹配完毕，返回文本中匹配结束的位置
    if (token_idx == tokens.size()) return t_idx;

    const Token& t = tokens[token_idx];
    
    // 如果该模式带有 '+' 元字符
    if (t.is_plus) {
        int max_k = 0;
        // 贪心匹配：找出最多能匹配多少个
        while (t_idx + max_k < text.length() && match_char(text[t_idx + max_k], t)) {
            max_k++;
        }
        // 回溯：从最多匹配次数开始往下尝试，保证 1 次及以上
        for (int k = max_k; k >= 1; k--) {
            int res = matchTokens(text, t_idx + k, tokens, token_idx + 1);
            if (res != -1) return res;
        }
    } 
    // 如果是普通字符或单独的 \d
    else {
        if (t_idx < text.length() && match_char(text[t_idx], t)) {
            return matchTokens(text, t_idx + 1, tokens, token_idx + 1);
        }
    }
    return -1;
}

int main() {
    string text, pat;
    // 读取文本串和模式串
    if (!(cin >> text >> pat)) return 0;

    vector<Token> tokens;
    // 解析正则表达式字符串
    for (int i = 0; i < pat.length(); ) {
        Token t = {'\0', false, false};
        if (pat[i] == '\\' && i + 1 < pat.length() && pat[i + 1] == 'd') {
            t.is_d = true;
            i += 2;
        } else {
            t.c = pat[i];
            i += 1;
        }
        if (i < pat.length() && pat[i] == '+') {
            t.is_plus = true;
            i += 1;
        }
        tokens.push_back(t);
    }

    int match_start = -1;
    int match_end = -1;
    
    // 遍历文本串的每一个起始位置进行尝试
    for (int i = 0; i < text.length(); i++) {
        int res = matchTokens(text, i, tokens, 0);
        if (res != -1) {
            match_start = i;
            match_end = res;
            break;
        }
    }

    // 第一行输出第一个匹配位置，第二行输出子串
    if (match_start != -1) {
        cout << match_start << endl;
        cout << text.substr(match_start, match_end - match_start) << endl;
    } else {
        cout << "No match" << endl;
    }
    
    return 0;
}
