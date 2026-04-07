#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int N = 8;
vector<int> pos;

void printBoard() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (pos[i] == j) cout << "Q ";
            else cout << ". ";
        }
        cout << "\n";
    }
}

bool isValid(int row, int col) {
    for (int i = 0; i < row; i++) {
        if (pos[i] == col || abs(pos[i] - col) == abs(i - row)) {
            return false;
        }
    }
    return true;
}

bool solve(int row) {
    if (row == N) {
        printBoard();
        return true;
    }
    
    for (int col = 0; col < N; col++) {
        if (isValid(row, col)) {
            pos[row] = col;
            if (solve(row + 1)) {
                return true;
            }
        }
    }
    return false;
}

int main() {
    pos.resize(N);
    solve(0);
    return 0;
}