// verify_test_data.cpp
// 功能：读取 Base64 编码的测试数据，解码后验算复数运算

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include "complex_number.cpp"

// ========== Base64 解码实现 ==========
static const std::string BASE64_CHARS = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int base64_char_value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

std::string base64_decode(const std::string& input) {
    std::string decoded;
    int val = 0, valb = -8;
    for (char c : input) {
        if (c == '=') break;
        int v = base64_char_value(c);
        if (v < 0) continue;
        val = (val << 6) + v;
        valb += 6;
        if (valb >= 0) {
            decoded.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return decoded;
}

// ========== 浮点数比较（考虑精度误差）==========
bool double_eq(double a, double b, double eps = 1e-2) {
    return std::abs(a - b) < eps;
}

// ========== 解析并验算单个测试用例 ==========
struct TestResult {
    std::string op;
    bool passed;
    std::string message;
};

TestResult verify_case(const std::string& line, const int total) {
    TestResult result;
    std::istringstream iss(line);
    std::string token;
    std::vector<double> nums;
    
    // 解析: op|num1|num2|...
    std::getline(iss, token, '|');
    result.op = token;
    
    double val;
    while (std::getline(iss, token, '|')) {
        try {
            val = std::stod(token);
            nums.push_back(val);
        } catch (...) {
            result.passed = false;
            result.message = "解析数字失败";
            return result;
        }
    }
    
    Complex c1, c2, computed;
    double expected_mod = 0.0;
    
    if (result.op == "ADD" && nums.size() >= 6) {
        c1 = Complex(nums[0], nums[1]);
        c2 = Complex(nums[2], nums[3]);
        computed = c1 + c2;
        if (double_eq(computed.real, nums[4]) && double_eq(computed.imag, nums[5])) {
            result.passed = true;
        } else {
            result.passed = false;
            std::ostringstream oss;
            oss << "input:" << "c1=" << nums[0] << (nums[1]>=0?"+":"") << nums[1] << "i + "
                << "c2=" << nums[2] << (nums[3]>=0?"+":"") << nums[3] << "i" << std::endl
                << "expected: " << nums[4] << (nums[5]>=0?"+":"") << nums[5] << "i" << std::endl
                << "but get: " << computed.real << (computed.imag>=0?"+":"") << computed.imag << "i" << std::endl;
            result.message = oss.str();
        }
    }
    else if (result.op == "SUB" && nums.size() >= 6) {
        c1 = Complex(nums[0], nums[1]);
        c2 = Complex(nums[2], nums[3]);
        computed = c1 - c2;
        if (double_eq(computed.real, nums[4]) && double_eq(computed.imag, nums[5])) {
            result.passed = true;
        } else {
            result.passed = false;
            std::ostringstream oss;
            oss << "input:" << "c1=" << nums[0] << (nums[1]>=0?"+":"") << nums[1] << "i - "
                << "c2=" << nums[2] << (nums[3]>=0?"+":"") << nums[3] << "i" << std::endl
                << "expected: " << nums[4] << (nums[5]>=0?"+":"") << nums[5] << "i" << std::endl
                << "but get: " << computed.real << (computed.imag>=0?"+":"") << computed.imag << "i" << std::endl;
            result.message = oss.str();
        }
    }
    else if (result.op == "MUL" && nums.size() >= 6) {
        c1 = Complex(nums[0], nums[1]);
        c2 = Complex(nums[2], nums[3]);
        computed = c1 * c2;
        if (double_eq(computed.real, nums[4]) && double_eq(computed.imag, nums[5])) {
            result.passed = true;
        } else {
            result.passed = false;
            std::ostringstream oss;
            oss << "input:" << "c1=" << nums[0] << (nums[1]>=0?"+":"") << nums[1] << "i * "
                << "c2=" << nums[2] << (nums[3]>=0?"+":"") << nums[3] << "i" << std::endl
                << "expected: " << nums[4] << (nums[5]>=0?"+":"") << nums[5] << "i" << std::endl
                << "but get: " << computed.real << (computed.imag>=0?"+":"") << computed.imag << "i" << std::endl;
            result.message = oss.str();
        }
    }
    else {
        result.passed = false;
        result.message = "不支持的操作或参数数量错误";
    }
    if (total < 3) {
    std::cout << "✅ [" << result.op << "] passed: "
                          << "c1=" << nums[0] << (nums[1]>=0?"+":"") << nums[1] << "i, "
                          << "c2=" << nums[2] << (nums[3]>=0?"+":"") << nums[3] << "i, "
                          << "result=" << computed.real << (computed.imag>=0?"+":"") << computed.imag << "i\n";
    }
    return result;
}

int main(int argc, char* argv[]) {
    std::string input_file = "test_data.b64";
    if (argc > 1) input_file = argv[1];
    
    std::ifstream in(input_file);
    if (!in) {
        std::cerr << "❌ 无法读取文件: " << input_file << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string encoded = buffer.str();
    in.close();
    
    // 解码
    std::string decoded = base64_decode(encoded);
    std::istringstream lines(decoded);
    std::string line;
    
    int total = 0, passed = 0, failed = 0;
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\n🔍 开始验算测试用例...\n";
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    
    while (std::getline(lines, line)) {
        // 跳过注释和空行
        if (line.empty() || line[0] == '#') continue;
        
        TestResult res = verify_case(line, total);
        total++;
        
        if (res.passed) {
            passed++;
        } 
        else {
            failed++;
            std::cout << "❌ [" << res.op << "] 失败: \n" << res.message << "\n";
        }
    }
    
    std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    std::cout << "📊 验算结果: " << passed << "/" << total << " 通过";
    if (failed > 0) {
        std::cout << " | " << failed << " 失败";
    }
    std::cout << " (" << (100.0 * passed / total) << "%)\n";
    
    return failed > 0 ? 1 : 0;
}