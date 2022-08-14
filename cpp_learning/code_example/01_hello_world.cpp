#include "common.h"
#include <cmath>
class A {
    int i;
    int j;
public:
    // A(int val): j(val), i(j) {
        // std::cout << "i " << i << " j " << j << std::endl;
    // }
    A() = default;
    A(int val): i(val), j(i) {
        std::cout << "i " << i << " j " << j << std::endl;
    }

    void print() {}
};


class Base {
public:
    virtual void do_something() {
        std::cout << "base" << std::endl;
    }
};

class Derived : public Base{
public:
    virtual void do_something() override {
         std::cout << "derived" << std::endl;
    }
};

int  cal_val(int x) {
    return std::abs(x);
}


int main(int argc, char** argv) {
    const int val = cal_val(-3);
    std::cout << val << std::endl;
    // std::cout << "Enter two numbers:" << std::endl;
    // int v1 = 0, v2 = 0;
    // while(std::cin >> v1) {
    //     std::cout << "v1 " << v1 << std::endl; 
    // }
    // char a1[] = {'C', 'b', 'a'};
    // std::cout << a1 << std::endl;
    // std::end(a1);
    // int i = 0;
    // std::cout << i << " " << ++i << " " << i++ << std::endl;
    // try {
    //     throw "aaa";
    // } catch (const char* a) {
    //     std::cout << a << std::endl;
    // }
    // A b();
    // A a(2);
    // b.print();
    int a = 2;
    int b = std::move(a);
    std::cout << a << " " << b << std::endl;
    Derived d;
    d.Base::do_something();
    return 0;
}