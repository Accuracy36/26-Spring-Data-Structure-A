#include <iostream>
#include <stdexcept>
#include <cmath>
#include <string>

class Complex {
public:
    double real = 0.0;
    double imag = 0.0;
    // funtion 1
    Complex(double r, double i){
        real = r;
        imag = i;
    }

    //function 2
    Complex(double r){
        real = r;
        imag = r;
    }

    //function 3
    Complex(){
        real = 0.0;
        imag = 0.0;
    }

    //function 4
    Complex(const std::string &s){
        size_t plus_pos = s.find('+');
        size_t minus_pos = s.find('-', 1);
        size_t i_pos = s.find('i');

        if (i_pos == std::string::npos) {
            throw std::invalid_argument("字符串格式错误，缺少 'i'");
        }

        try {
            if (plus_pos != std::string::npos) {
                real = std::stod(s.substr(0, plus_pos));
                imag = std::stod(s.substr(plus_pos + 1, i_pos - plus_pos - 1));
            } else if (minus_pos != std::string::npos) {
                real = std::stod(s.substr(0, minus_pos));
                imag = -std::stod(s.substr(minus_pos + 1, i_pos - minus_pos - 1));
            } else {
                throw std::invalid_argument("字符串格式错误，缺少 '+' 或 '-'");
            }
        } catch (...) {
            throw std::invalid_argument("字符串格式错误，无法解析数字");
        }
    }

    void print_complex(){
        std::cout << real;
        if (imag >= 0) std::cout << " + " << imag << "i";
        else std::cout << " - " << -imag << "i" << std::endl;
    }

    Complex add(const Complex &o){
        return Complex(real + o.real, imag + o.imag);
    }

    Complex operator+(Complex &o){
        return Complex(real + o.real, imag + o.imag);
    }

    Complex operator-(Complex &o){
        // TODO: begin here, you should implement the subtraction operator for Complex numbers in 1 line
        return Complex(real - o.real, imag - o.imag);
        // end here
    }

    Complex operator*(Complex &o){
        // TODO: begin here, you should implement the multiplication operator for Complex numbers in 1 line
        return Complex(real * o.real - imag * o.imag, real * o.imag + imag * o.real);
        // end here
    }
};

