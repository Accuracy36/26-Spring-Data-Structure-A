	// run_bigint_tests.cpp
	// 功能：读取 Base64 编码的测试文件，解码并执行 BigInt 运算测试
	#include <iostream>
	#include <fstream>
	#include <sstream>
	#include <string>
	#include <vector>
	#include <algorithm>
	#include <iomanip>
    #include "big_int.cpp"

	static const std::string BASE64_CHARS = 
	    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	std::string base64_decode(const std::string& encoded_string) {
	    std::vector<int> T(256, -1);
	    for (int i = 0; i < 64; i++) T[BASE64_CHARS[i]] = i;
	    int val = 0, valb = -8;
	    std::string out;
	    for (unsigned char c : encoded_string) {
	        if (T[c] == -1) break;
	        val = (val << 6) + T[c];
	        valb += 6;
	        if (valb >= 0) {
	            out.push_back(char((val >> valb) & 0xFF));
	            valb -= 8;
	        }
	    }
	    return out;
	}
	// ==================== 测试运行器 ====================
	struct TestStats {
	    int total = 0;
	    int passed = 0;
	};
	// 辅助函数：去除字符串两端空白（处理生成时可能的换行）
	std::string trim(const std::string& s) {
	    size_t start = s.find_first_not_of(" \t\r\n");
	    if (start == std::string::npos) return "";
	    size_t end = s.find_last_not_of(" \t\r\n");
	    return s.substr(start, end - start + 1);
	}
	// 执行单个测试用例
	void run_single_test(const std::string& op, const std::string& n1, const std::string& n2, 
	                     const std::string& expected, TestStats& stats, bool& first_flag) {
	    BigInt a(n1);
	    BigInt b(n2);
	    BigInt res;
	    if (op == "ADD") res = a + b;
	    else if (op == "SUB") res = a - b;
	    else if (op == "MUL") res = a * b;
	    else return;
	    stats.total++;
	    std::string actual = res.toString();
	    std::string expected_clean = trim(expected);
	    bool pass = (actual == expected_clean);
	    if (pass) stats.passed++;
	    // 如果是第一次测试，打印详情
	    if (first_flag) {
	        std::cout << "--- " << op << " 第一次测试详情 ---" << std::endl;
	        std::cout << "输入: " << n1 << " " << (op == "ADD" ? "+" : (op == "SUB" ? "-" : "*")) << " " << n2 << std::endl;
	        std::cout << "预期: " << expected_clean << std::endl;
	        std::cout << "实际: " << actual << std::endl;
	        std::cout << "结果: " << (pass ? "✅ PASS" : "❌ FAIL") << std::endl;
	        std::cout << "------------------------" << std::endl << std::endl;
	        first_flag = false;
	    }
	}
	int main(int argc, char* argv[]) {
	    std::string input_file = "bigint_tests.b64";
	    if (argc > 1) input_file = argv[1];
	    // 1. 读取 Base64 文件
	    std::ifstream in(input_file);
	    if (!in.is_open()) {
	        std::cerr << "❌ 无法打开测试文件: " << input_file << std::endl;
	        return 1;
	    }
	    std::stringstream buffer;
	    buffer << in.rdbuf();
	    std::string encoded_content = buffer.str();
	    in.close();
	    // 2. Base64 解码
	    std::string raw_data = base64_decode(encoded_content);
	    // 3. 解析并测试
	    std::istringstream stream(raw_data);
	    std::string line;
	    TestStats stats;
	    bool first_add = true;
	    bool first_sub = true;
	    bool first_mul = true;
	    std::cout << "🚀 开始运行测试..." << std::endl << std::endl;
	    while (std::getline(stream, line)) {
	        if (line.empty() || line[0] == '#') continue;
	        // 解析格式: OP|NUM1|NUM2|RESULT
	        std::replace(line.begin(), line.end(), '|', ' ');
	        std::istringstream iss(line);
	        std::string op, n1, n2, expected;
	        if (iss >> op >> n1 >> n2 >> expected) {
	            if (op == "ADD") run_single_test(op, n1, n2, expected, stats, first_add);
	            else if (op == "SUB") run_single_test(op, n1, n2, expected, stats, first_sub);
	            else if (op == "MUL") run_single_test(op, n1, n2, expected, stats, first_mul);
	        }
	    }
	    // 4. 输出统计
	    std::cout << "============================" << std::endl;
	    std::cout << "📊 测试报告" << std::endl;
	    std::cout << "============================" << std::endl;
	    std::cout << "总测试数: " << stats.total << std::endl;
	    std::cout << "通过数:   " << stats.passed << std::endl;
	    std::cout << "失败数:   " << (stats.total - stats.passed) << std::endl;
	    double accuracy = (stats.total == 0) ? 0.0 : (static_cast<double>(stats.passed) / stats.total) * 100.0;
	    std::cout << "正确率:   " << std::fixed << std::setprecision(2) << accuracy << "%" << std::endl;
	    return 0;
	}