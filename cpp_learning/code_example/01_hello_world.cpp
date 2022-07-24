#include "common.h"

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



int main(int argc, char** argv) {
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
    void (*pf)(int* x) = nullptr;

    
    return 0;
}