#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 构建 next (lps) 数组
void computeLPSArray(const string& pat, vector<int>& lps) {
    int m = pat.length();
    int len = 0;
    lps[0] = 0;
    int i = 1;
    while (i < m) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

int main() {
    string text, pat;
    if (!(cin >> text >> pat)) return 0;

    int n = text.length();
    int m = pat.length();
    
    if (m == 0) {
        cout << 0 << endl;
        return 0;
    }

    vector<int> lps(m);
    computeLPSArray(pat, lps);

    int i = 0; // text 的指针
    int j = 0; // pat 的指针
    int match_pos = -1;

    while (i < n) {
        if (pat[j] == text[i]) {
            j++;
            i++;
        }
        if (j == m) {
            match_pos = i - j;
            break; // 找到第一个匹配即退出
        } else if (i < n && pat[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i = i + 1;
            }
        }
    }

    // 输出匹配位置
    cout << match_pos << endl;
    
    return 0;
}